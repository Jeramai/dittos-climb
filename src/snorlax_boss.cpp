#include "snorlax_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_projectiles.h"
#include "bn_sprite_items_snorlax.h"

#include "directions.h"
#include "projectile_frames.h"
#include "species.h"

namespace
{
    constexpr bn::fixed walk_speed = 0.35;
    constexpr int waking_frames = 50;
    constexpr int windup_frames = 30;
    constexpr int jump_frames = 36;
    constexpr int landed_frames = 45;
    constexpr int resting_frames = 150;
    constexpr int rest_heal = 40;
    constexpr int jump_height = 40;
    constexpr int shockwave_count = 8;
    constexpr bn::fixed shockwave_speed = 1.4;
    constexpr int shockwave_life = 70;

    attack wild(move_id move, int power)
    {
        return attack{ move, pokemon_type::normal, power };
    }
}

snorlax_boss::snorlax_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    _sprite(bn::sprite_items::snorlax.create_sprite(position, species_frames::asleep)),
    _camera(camera),
    _position(position),
    _slam_attack(wild(move_id::body_slam, 85)),
    _shockwave_attack(wild(move_id::body_slam, 40))
{
    _sprite.set_camera(camera);
    _update_sprite(0);
}

void snorlax_boss::wake(message_box& messages)
{
    if(_state != state::asleep)
    {
        return;
    }

    messages.show("SNORLAX woke up!");
    _state = state::waking;
    _state_frames = waking_frames;
}

void snorlax_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                          message_box& messages)
{
    _landed_this_frame = false;
    bn::fixed height = 0;

    if(_flash_frames)
    {
        --_flash_frames;
    }

    switch(_state)
    {

    default:
        break;

    case state::waking:
        if(! --_state_frames)
        {
            _state = state::walking;
        }
        break;

    case state::walking:
        if(! _rested && _hp * 100 < max_hp * 35)
        {
            _rested = true;
            messages.show("SNORLAX used REST!");
            messages.show("SNORLAX fell asleep!");
            _state = state::resting;
            _state_frames = resting_frames;
            break;
        }

        _walk(target);

        if(--_slam_timer <= 0)
        {
            _state = state::windup;
            _state_frames = windup_frames;
            _jump_start = _position;
            _jump_target = target;

            bn::sprite_ptr marker = bn::sprite_items::projectiles.create_sprite(target, projectile_frames::impact);
            marker.set_camera(_camera);
            marker.set_z_order(-900);
            _marker = bn::move(marker);
        }
        break;

    case state::windup:
        if(! --_state_frames)
        {
            messages.show("SNORLAX used BODY SLAM!");
            _state = state::jumping;
            _state_frames = jump_frames;
        }
        break;

    case state::jumping:
    {
        --_state_frames;
        bn::fixed progress = bn::fixed(jump_frames - _state_frames) / jump_frames;
        _position = _jump_start + (_jump_target - _jump_start) * progress;
        height = bn::degrees_lut_sin(progress * 180) * jump_height;

        if(! _state_frames)
        {
            _land(projectiles);
        }
        break;
    }

    case state::landed:
        if(! --_state_frames)
        {
            _state = state::walking;
            _slam_timer = 150 + random.get_int(60);
        }
        break;

    case state::resting:
        if(! --_state_frames)
        {
            _hp = bn::min(_hp + rest_heal, max_hp);
            messages.show("SNORLAX woke up!");
            _state = state::walking;
        }
        break;
    }

    if(_marker)
    {
        _marker->set_visible((_state_frames / 3) % 2 == 0);
    }

    _update_sprite(height);
}

bool snorlax_boss::contains(const bn::fixed_point& point, int half_size) const
{
    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < 12 + half_size && bn::abs(delta.y()) < 13 + half_size;
}

hit_result snorlax_boss::take_hit(const attack& hit)
{
    if(! awake() || _state == state::jumping)
    {
        return hit_result{ 0, types::neutral };
    }

    const species_data& data = species::get(species_id::snorlax);
    hit_result result = combat::resolve(hit, data.type_1, data.type_2);
    _hp -= result.damage;
    _flash_frames = 4;
    return result;
}

bool snorlax_boss::hit_by_area(int serial)
{
    if(_last_area_serial == serial)
    {
        return false;
    }

    _last_area_serial = serial;
    return true;
}

void snorlax_boss::_walk(const bn::fixed_point& target)
{
    bn::fixed_point step = directions::toward(_position, target) * walk_speed;
    bn::fixed_point next(_position.x() + step.x(), _position.y());

    if(! room::area_is_blocked(next.x() - 10, next.y() + 6, next.x() + 10, next.y() + 14))
    {
        _position = next;
    }

    next = bn::fixed_point(_position.x(), _position.y() + step.y());

    if(! room::area_is_blocked(next.x() - 10, next.y() + 6, next.x() + 10, next.y() + 14))
    {
        _position = next;
    }

    ++_walk_frames;
}

void snorlax_boss::_land(enemy_projectiles& projectiles)
{
    _landed_this_frame = true;
    _marker.reset();
    _state = state::landed;
    _state_frames = landed_frames;

    for(int index = 0; index < shockwave_count; ++index)
    {
        bn::fixed_point direction = bn::fixed_point(bn::degrees_lut_cos(index * 45), bn::degrees_lut_sin(index * 45));
        bn::fixed_point origin = _position + direction * 10 + bn::fixed_point(0, 8);
        projectiles.spawn(bn::sprite_items::projectiles.create_sprite(origin, projectile_frames::enemy_shot), origin,
                          direction * shockwave_speed, _shockwave_attack, shockwave_life, 5, true);
    }
}

void snorlax_boss::_update_sprite(bn::fixed height)
{
    int frame = species_frames::walk;

    if(_state == state::asleep || _state == state::resting)
    {
        frame = species_frames::asleep;
    }
    else if(_flash_frames || (_state == state::windup && (_state_frames / 3) % 2))
    {
        frame = species_frames::white;
    }
    else if(_state == state::walking)
    {
        frame = species_frames::walk + (_walk_frames / 16) % 2;
    }

    bn::fixed shake = _state == state::waking ? bn::fixed((_state_frames / 2) % 2 ? 1 : -1) : bn::fixed(0);
    _sprite.set_tiles(bn::sprite_items::snorlax.tiles_item(), frame);
    _sprite.set_position(_position + bn::fixed_point(shake, -height));
    _sprite.set_z_order(-_position.y().round_integer() - 8);
}
