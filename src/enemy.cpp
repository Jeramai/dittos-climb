#include "enemy.h"

#include "attacks.h"
#include "directions.h"

namespace
{
    constexpr int melee_range = 30;
    constexpr int dash_range = 90;
    constexpr int shot_range = 150;
    constexpr int shooter_keep_distance = 60;
    constexpr int recover_frames = 30;
    constexpr int cooldown_scale_a = 3;
    constexpr int cooldown_scale_b = 5;
    constexpr bn::fixed walk_speed_scale = 0.4;
    constexpr bn::fixed dash_speed_scale = 0.85;
    constexpr bn::fixed shot_speed_scale = 0.6;
    constexpr int reveal_distance = 44;
    constexpr int paralysis_frames = 240;
    constexpr int poison_frames = 360;
    constexpr int poison_tick_frames = 60;
    constexpr int sleep_frames = 150;
    constexpr int confusion_frames = 180;
    constexpr int burrow_surface_frames = 150;
    constexpr int burrow_travel_frames = 120;
    constexpr int burrow_surface_distance = 20;
    constexpr bn::fixed burrow_speed_scale = 1.5;
    constexpr bn::fixed wobble_strength = 0.9;

    attack wild_attack(move_id move, const species_data& user)
    {
        attack result = combat::make_attack(move, user.type_1, user.type_2);
        result.power = result.power * 2 / 3;
        return result;
    }

    int windup_frames(move_pattern pattern)
    {
        switch(pattern)
        {

        case move_pattern::melee:
            return 14;

        case move_pattern::dash:
            return 22;

        default:
            return 16;
        }
    }

    int attack_range(move_pattern pattern)
    {
        switch(pattern)
        {

        case move_pattern::melee:
            return melee_range;

        case move_pattern::dash:
        case move_pattern::dig:
            return dash_range;

        case move_pattern::shot:
            return shot_range;

        case move_pattern::cloud:
            return shot_range * 2 / 3;

        case move_pattern::beam:
            return shot_range;

        default:
            return 0;
        }
    }

    int distance(const bn::fixed_point& a, const bn::fixed_point& b)
    {
        bn::fixed_point delta = a - b;
        return (bn::abs(delta.x()) + bn::abs(delta.y())).round_integer();
    }
}

enemy::enemy(species_id id, const bn::fixed_point& position, const bn::camera_ptr& camera, bn::random& random) :
    _sprite(species::get(id).sprite->create_sprite(position, species_frames::walk)),
    _position(position),
    _id(id),
    _hp(species::get(id).hp),
    _cooldowns{ 30 + random.get_int(60), 90 + random.get_int(90) },
    _dash_attack(combat::make_attack(move_id::tackle, pokemon_type::normal, pokemon_type::none))
{
    _sprite.set_camera(camera);
}

void enemy::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random)
{
    ++_frame_counter;

    if(_state != state::spawning && ! _update_status())
    {
        _update_sprite(false);
        return;
    }

    bool slowed = _status == status_effect::paralysis;

    for(int& cooldown : _cooldowns)
    {
        if(cooldown && (! slowed || _frame_counter % 2))
        {
            --cooldown;
        }
    }

    if(_flash_frames)
    {
        --_flash_frames;
    }

    bool moving = false;

    switch(_state)
    {

    case state::spawning:
        _sprite.set_visible((_state_frames / 3) % 2);

        if(! --_state_frames)
        {
            _sprite.set_visible(true);
            _state = state::moving;
        }

        _sprite.set_position(_position);
        return;

    case state::moving:
        _update_hidden(target);

        if(_update_burrow(target))
        {
            moving = true;
            break;
        }

        if(_hidden || ! _try_start_attack(target))
        {
            bool shooter = moves::get(_move(0)).pattern == move_pattern::shot ||
                           moves::get(_move(1)).pattern == move_pattern::shot;
            int keep_distance = shooter ? shooter_keep_distance : 8;

            if(distance(target, _position) > keep_distance)
            {
                bn::fixed speed = data().speed * walk_speed_scale;
                _walk(_movement(target, random) * (slowed ? speed / 2 : speed));
                moving = true;
            }

            _facing_left = target.x() < _position.x();
        }
        break;

    case state::windup:
        if(! --_state_frames)
        {
            _execute(projectiles, random);
        }
        break;

    case state::dashing:
        _walk(_attack_direction * moves::get(_move(_pending_move)).speed * dash_speed_scale);
        moving = true;

        if(! --_state_frames)
        {
            _state = state::recovering;
            _state_frames = recover_frames;
        }
        break;

    case state::recovering:
        if(! --_state_frames)
        {
            _state = state::moving;
        }
        break;

    default:
        break;
    }

    _update_sprite(moving);
}

bool enemy::contains(const bn::fixed_point& point, int half_size) const
{
    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < 4 + half_size && bn::abs(delta.y()) < 5 + half_size;
}

hit_result enemy::take_hit(const attack& hit)
{
    const species_data& current = data();
    hit_result result = combat::resolve(hit, current.type_1, current.type_2);
    _hp -= result.damage;
    _flash_frames = 4;
    return result;
}

void enemy::apply_status(status_effect effect)
{
    if(effect == status_effect::none || _status != status_effect::none)
    {
        return;
    }

    _status = effect;
    _status_frames = effect == status_effect::paralysis ? paralysis_frames :
                     effect == status_effect::poison ? poison_frames :
                     effect == status_effect::confusion ? confusion_frames : sleep_frames;

    if(effect == status_effect::sleep && _state != state::spawning)
    {
        _state = state::moving;
    }
}

bool enemy::_update_status()
{
    if(_status == status_effect::none)
    {
        return true;
    }

    if(_status == status_effect::poison && _frame_counter % poison_tick_frames == 0)
    {
        --_hp;
        _flash_frames = 2;
    }

    bool asleep = _status == status_effect::sleep;

    if(--_status_frames <= 0)
    {
        _status = status_effect::none;
    }

    return ! asleep;
}

bool enemy::_update_burrow(const bn::fixed_point& target)
{
    if(data().behavior != species_behavior::burrower)
    {
        return false;
    }

    ++_burrow_frames;

    if(! _underground)
    {
        if(_burrow_frames > burrow_surface_frames)
        {
            _underground = true;
            _burrow_frames = 0;
        }

        return false;
    }

    _walk(directions::toward(_position, target) * data().speed * walk_speed_scale * burrow_speed_scale);

    if(distance(target, _position) < burrow_surface_distance || _burrow_frames > burrow_travel_frames)
    {
        _underground = false;
        _burrow_frames = 0;
        _cooldowns[0] = 0;
    }

    return true;
}

bn::fixed_point enemy::_movement(const bn::fixed_point& target, bn::random& random)
{
    if(_status == status_effect::confusion)
    {
        if(++_wobble_frames % 30 == 0)
        {
            _attack_direction = directions::vectors[random.get_int(8)];
        }

        return _attack_direction;
    }

    bn::fixed_point toward = directions::toward(_position, target);

    if(data().behavior != species_behavior::flyer)
    {
        return toward;
    }

    ++_wobble_frames;
    bn::fixed wobble = bn::degrees_lut_sin((_wobble_frames * 6) % 360) * wobble_strength;
    return toward + bn::fixed_point(-toward.y(), toward.x()) * wobble;
}

void enemy::_update_hidden(const bn::fixed_point& target)
{
    bool in_grass = room::at(_position.x(), _position.y() + 4) == room::cells::grass;
    _hidden = in_grass && distance(target, _position) > reveal_distance && ! _flash_frames;

    if(_was_hidden && ! _hidden)
    {
        _just_revealed = true;
    }

    _was_hidden = _hidden;
}

bool enemy::dash_hits(const bn::fixed_point& point)
{
    if(_state != state::dashing || _dash_connected || ! contains(point, 3))
    {
        return false;
    }

    _dash_connected = true;
    return true;
}

bool enemy::hit_by_area(int serial)
{
    if(_last_area_serial == serial)
    {
        return false;
    }

    _last_area_serial = serial;
    return true;
}

move_id enemy::_move(int index) const
{
    return index ? data().move_b : data().move_a;
}

bool enemy::_try_start_attack(const bn::fixed_point& target)
{
    int target_distance = distance(target, _position);

    for(int index = 1; index >= 0; --index)
    {
        const move_data& move = moves::get(_move(index));

        if(! _cooldowns[index] && target_distance <= attack_range(move.pattern))
        {
            _pending_move = index;
            _attack_direction = directions::toward(_position, target);
            _facing_left = target.x() < _position.x();
            _state = state::windup;
            _state_frames = windup_frames(move.pattern);
            return true;
        }
    }

    return false;
}

void enemy::_execute(enemy_projectiles& projectiles, bn::random& random)
{
    const species_data& current = data();
    move_id id = _move(_pending_move);
    const move_data& move = moves::get(id);
    attack hit = wild_attack(id, current);
    int scale = _pending_move ? cooldown_scale_b : cooldown_scale_a;
    _cooldowns[_pending_move] = move.cooldown * scale + random.get_int(30);
    _state = state::recovering;
    _state_frames = recover_frames;

    switch(move.pattern)
    {

    case move_pattern::shot:
        attacks::shoot(projectiles, hit, _position, _attack_direction, shot_speed_scale);
        break;

    case move_pattern::melee:
        attacks::slash(projectiles, hit, _position, _attack_direction);
        break;

    case move_pattern::cloud:
        attacks::cloud(projectiles, hit, _position, _attack_direction, 1);
        break;

    case move_pattern::beam:
        attacks::beam(projectiles, hit, _position, _attack_direction);
        break;

    case move_pattern::dash:
    case move_pattern::dig:
        _dash_attack = hit;
        _dash_connected = false;
        _state = state::dashing;
        _state_frames = move.life * 2;
        break;

    default:
        break;
    }
}

void enemy::_walk(const bn::fixed_point& step)
{
    bn::fixed_point next(_position.x() + step.x(), _position.y());

    if(! room::feet_are_blocked(next))
    {
        _position = next;
    }

    next = bn::fixed_point(_position.x(), _position.y() + step.y());

    if(! room::feet_are_blocked(next))
    {
        _position = next;
    }
}

void enemy::_update_sprite(bool moving)
{
    int frame = species_frames::walk;

    if(_underground)
    {
        frame = species_frames::mound;
    }
    else if(_flash_frames || (_state == state::windup && (_state_frames / 3) % 2))
    {
        frame = species_frames::white;
    }
    else if(moving)
    {
        ++_walk_frames;
        frame = species_frames::walk + (_walk_frames / 10) % 2;
    }

    _sprite.set_tiles(data().sprite->tiles_item(), frame);
    _sprite.set_visible(! _hidden && _in_light);
    _sprite.set_position(_position);
    _sprite.set_horizontal_flip(_facing_left);
    _sprite.set_z_order(-_position.y().round_integer());
}
