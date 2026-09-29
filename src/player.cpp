#include "player.h"

#include "bn_keypad.h"
#include "bn_sprite_affine_mat_ptr.h"

#include "bn_sprite_items_ditto.h"
#include "bn_sprite_items_wave.h"

#include "attacks.h"
#include "directions.h"

namespace
{
    constexpr bn::fixed dodge_speed = 2.5;
    constexpr bn::fixed dodge_end_speed = 1;
    constexpr int dodge_frames = 26;
    constexpr int dodge_fast_frames = 16;
    constexpr int dodge_invulnerable_frames = 18;
    constexpr int dodge_cooldown_frames = 12;
    constexpr bn::fixed dodge_squash = 0.5;
    constexpr int hurt_invulnerable_frames = 60;
    constexpr int transform_frames = 30;
    constexpr int switch_flash_frames = 8;
    constexpr int dash_half_size = 6;
    constexpr int wave_half_size = 14;
    constexpr int explode_half_size = 30;
    constexpr int beam_charge_frames = 40;
    constexpr int paralysis_frames = 180;
    constexpr int poison_frames = 300;
    constexpr int poison_tick_frames = 45;
    constexpr int sleep_frames = 60;
    constexpr int confusion_frames = 150;
    constexpr bn::fixed current_speed = 0.6;

    const species_data& ditto()
    {
        return species::get(species_id::ditto);
    }
}

player::player(const bn::camera_ptr& camera, const bn::fixed_point& position) :
    _camera(camera),
    _sprite(bn::sprite_items::ditto.create_sprite(position, species_frames::own_walk)),
    _position(position),
    _hp(ditto().hp),
    _area_attack(combat::make_attack(move_id::struggle, pokemon_type::normal, pokemon_type::none)),
    _charge_attack(_area_attack)
{
    _sprite.set_camera(camera);
}

bool player::update(player_projectiles& projectiles, message_box& messages, const species_id* outline_below)
{
    bool used_outline = false;

    if(_area_frames && ! --_area_frames && _self_destructing)
    {
        _self_destructing = false;
        _faint_form(messages);
    }

    _update_wave();

    if(_invulnerable_frames)
    {
        --_invulnerable_frames;
    }

    if(_switch_flash_frames)
    {
        --_switch_flash_frames;
    }

    if(_transform_frames)
    {
        if(! --_transform_frames)
        {
            _finish_transform(messages);
        }

        _update_sprite(false);
        return false;
    }

    ++_frame_counter;
    _update_water(messages);

    if(! _update_status(messages))
    {
        _update_sprite(false);
        return false;
    }

    if(_charge_frames)
    {
        if(! --_charge_frames)
        {
            attacks::beam(projectiles, _charge_attack, _position, directions::vectors[_aim]);
        }

        _update_sprite(false);
        return false;
    }

    bool slowed = _status == status_effect::paralysis;

    int input_x = int(bn::keypad::right_held()) - int(bn::keypad::left_held());
    int input_y = int(bn::keypad::down_held()) - int(bn::keypad::up_held());

    if(_status == status_effect::confusion)
    {
        input_x = -input_x;
        input_y = -input_y;
    }
    int move_direction = directions::from_input(input_x, input_y);
    bool moving = move_direction >= 0;

    if(moving && ! bn::keypad::r_held() && ! _dash_frames)
    {
        _aim = move_direction;
    }

    if(_cooldown && (! slowed || _frame_counter % 2))
    {
        --_cooldown;
    }

    if(_dodge_cooldown)
    {
        --_dodge_cooldown;
    }

    if(_dash_frames)
    {
        --_dash_frames;
        _move(_dash_velocity);
        _digging = _digging && _dash_frames;
    }
    else if(_dodge_frames)
    {
        --_dodge_frames;
        bn::fixed speed = _dodge_frames > dodge_frames - dodge_fast_frames ? dodge_speed : dodge_end_speed;
        _move(directions::vectors[_dodge_direction] * speed);

        if(! _dodge_frames)
        {
            _dodge_cooldown = dodge_cooldown_frames;
        }
    }
    else
    {
        if(bn::keypad::l_pressed() && ! _dodge_cooldown)
        {
            _dodge_frames = dodge_frames;
            _dodge_direction = moving ? move_direction : _aim;
        }
        else
        {
            if(moving)
            {
                _move(directions::vectors[move_direction] * (slowed ? body().speed / 2 : body().speed));
            }

            if(outline_below && ! _form && bn::keypad::b_pressed())
            {
                start_transform(*outline_below);
                used_outline = true;
            }
            else if(! _cooldown)
            {
                if(bn::keypad::a_held())
                {
                    _use_move(true, projectiles, messages);
                }
                else if(bn::keypad::b_pressed())
                {
                    _use_move(false, projectiles, messages);
                }
            }
        }
    }

    _update_sprite(moving);
    return used_outline;
}

bool player::vulnerable() const
{
    if(_invulnerable_frames || _transform_frames || _digging)
    {
        return false;
    }

    return ! _dodge_frames || _dodge_frames <= dodge_frames - dodge_invulnerable_frames;
}

const species_data& player::body() const
{
    const form* current = active_form();
    return current ? species::get(current->species) : ditto();
}

const form* player::active_form() const
{
    return _form ? &*_form : nullptr;
}

hit_result player::take_hit(const attack& hit, message_box& messages)
{
    const species_data& current = body();
    hit_result result = combat::resolve(hit, current.type_1, current.type_2);
    _invulnerable_frames = hurt_invulnerable_frames;

    if(! result.effectiveness)
    {
        messages.show("It doesn't affect DITTO...");
        return result;
    }

    if(! result.damage)
    {
        return result;
    }

    messages.show(combat::effectiveness_message(result.effectiveness));

    if(! _form)
    {
        _hp = bn::max(_hp - result.damage, 0);
        return result;
    }

    _form->hp -= result.damage;

    if(_form->hp <= 0)
    {
        _faint_form(messages);
    }

    return result;
}

void player::apply_status(status_effect effect, message_box& messages)
{
    if(effect == status_effect::none || _status != status_effect::none || _transform_frames)
    {
        return;
    }

    _status = effect;
    _poison_timer = 0;

    switch(effect)
    {

    case status_effect::paralysis:
        _status_frames = paralysis_frames;
        messages.show("DITTO is paralyzed!");
        break;

    case status_effect::poison:
        _status_frames = poison_frames;
        messages.show("DITTO was poisoned!");
        break;

    case status_effect::confusion:
        _status_frames = confusion_frames;
        messages.show("DITTO became confused!");
        break;

    default:
        _status_frames = sleep_frames;
        _charge_frames = 0;
        messages.show("DITTO fell asleep!");
        break;
    }
}

void player::heal(int amount)
{
    if(_form)
    {
        _form->hp = bn::min(_form->hp + amount, species::get(_form->species).hp * form_hp_scale);
    }
    else
    {
        _hp = bn::min(_hp + amount, ditto().hp);
    }
}

void player::recoil(message_box& messages)
{
    messages.show("DITTO is hit with recoil!");

    if(_form)
    {
        _form->hp = bn::max(_form->hp - 1, 1);
    }
    else
    {
        _hp = bn::max(_hp - 1, 0);
    }
}

bool player::_update_status(message_box& messages)
{
    if(_status == status_effect::none)
    {
        return true;
    }

    if(_status == status_effect::poison && ++_poison_timer % poison_tick_frames == 0)
    {
        _lose_hp(1);
    }

    bool asleep = _status == status_effect::sleep;

    if(--_status_frames <= 0)
    {
        if(asleep)
        {
            messages.show("DITTO woke up!");
        }
        else if(_status == status_effect::confusion)
        {
            messages.show("DITTO snapped out of confusion!");
        }

        _status = status_effect::none;
    }

    return ! asleep;
}

void player::_faint_form(message_box& messages)
{
    if(! _form)
    {
        return;
    }

    message_box::text message(species::get(_form->species).name);
    message.append(" fainted!");
    messages.show(message);
    messages.show("DITTO lost its shape!");
    _form.reset();
    _switch_flash_frames = switch_flash_frames;
}

void player::_lose_hp(int amount)
{
    if(_form)
    {
        _form->hp = bn::max(_form->hp - amount, 1);
    }
    else
    {
        _hp = bn::max(_hp - amount, 1);
    }
}

void player::start_transform(species_id target)
{
    _transform_target = target;
    _transform_frames = transform_frames;
    _dash_frames = 0;
    _digging = false;
    _area_frames = 0;
    _charge_frames = 0;
    _status = status_effect::none;
    _dodge_frames = 0;
}

void player::evolve(species_id target)
{
    if(_form)
    {
        _evolving_from = _form->species;
        start_transform(target);
    }
}

void player::set_position(const bn::fixed_point& position)
{
    _position = position;
    _dash_frames = 0;
    _digging = false;
    _dodge_frames = 0;
    _area_frames = 0;
    _update_sprite(false);
}

void player::set_visible(bool visible)
{
    _sprite.set_visible(visible);
}

void player::_use_move(bool move_a, player_projectiles& projectiles, message_box& messages)
{
    const species_data& current = body();
    move_id move = move_a ? current.move_a : current.move_b;

    if(_form && ! move_a)
    {
        int& pp = _form->pp_b;

        if(pp)
        {
            --pp;
        }
        else
        {
            message_box::text message(moves::get(move).name);
            message.append(" has no PP left!");
            messages.show(message);
            move = move_id::struggle;
        }
    }

    const move_data& data = moves::get(move);
    attack hit = combat::make_attack(move, current.type_1, current.type_2);
    const bn::fixed_point& aim = directions::vectors[_aim];
    _cooldown = data.cooldown;

    switch(data.pattern)
    {

    case move_pattern::shot:
        attacks::shoot(projectiles, hit, _position + aim * 6, aim, 1);
        break;

    case move_pattern::melee:
        attacks::slash(projectiles, hit, _position, aim);
        break;

    case move_pattern::dash:
        _dash_frames = data.life;
        _dash_velocity = aim * data.speed;
        _start_area(hit, data.life, dash_half_size);
        break;

    case move_pattern::cloud:
        attacks::cloud(projectiles, hit, _position + aim * 8, aim, 1);
        break;

    case move_pattern::explode:
        _start_area(hit, data.life, explode_half_size);
        _self_destructing = true;
        messages.show("DITTO used SELFDESTRUCT!");
        break;

    case move_pattern::dig:
        _dash_frames = data.life;
        _dash_velocity = aim * data.speed;
        _digging = true;
        _start_area(hit, data.life, dash_half_size);
        messages.show("DITTO dug underground!");
        break;

    case move_pattern::beam:
        _charge_frames = beam_charge_frames;
        _charge_attack = hit;
        messages.show("DITTO is taking in sunlight!");
        break;

    case move_pattern::wave:
        _start_area(hit, data.life, wave_half_size);
        _wave_sprite = bn::sprite_items::wave.create_sprite(_position, 0);
        _wave_sprite->set_camera(_camera);
        _wave_sprite->set_z_order(-999);
        break;

    default:
        if(move == move_id::splash)
        {
            messages.show("DITTO used SPLASH!");
            messages.show("But nothing happened!");
        }
        else
        {
            messages.show("DITTO used TRANSFORM!");
            messages.show("But it failed!");
        }
        break;
    }
}

void player::_finish_transform(message_box& messages)
{
    const species_data& target = species::get(_transform_target);
    _form = form{ _transform_target, target.hp * form_hp_scale,
                  moves::get(target.move_b).pp };
    _hp = ditto().hp;

    message_box::text message;

    if(_evolving_from)
    {
        message.append(species::get(*_evolving_from).name);
        message.append(" evolved into ");
        _evolving_from.reset();
    }
    else
    {
        message.append("DITTO transformed into ");
    }

    message.append(target.name);
    message.append("!");
    messages.show(message);
}

void player::_move(const bn::fixed_point& delta)
{
    bool can_swim = species::can_swim(body());
    bn::fixed_point next(_position.x() + delta.x(), _position.y());

    if(! room::feet_are_blocked(next, can_swim))
    {
        _position = next;
    }

    next = bn::fixed_point(_position.x(), _position.y() + delta.y());

    if(! room::feet_are_blocked(next, can_swim))
    {
        _position = next;
    }
}

void player::_update_water(message_box& messages)
{
    bool can_swim = species::can_swim(body());

    if(room::feet_are_blocked(_position, can_swim))
    {
        if(bn::optional<bn::fixed_point> shore = room::nearest_standable(_position, can_swim))
        {
            _position = *shore;
            messages.show("DITTO washed ashore!");
        }

        return;
    }

    bn::fixed_point flow = room::flow_at(_position);

    if(flow != bn::fixed_point())
    {
        _move(flow * current_speed);
    }
}

void player::_start_area(const attack& hit, int frames, int half_size)
{
    _area_attack = hit;
    _area_frames = frames;
    _area_half_size = half_size;
    ++_area_serial;
}

void player::_update_wave()
{
    if(! _wave_sprite)
    {
        return;
    }

    if(! _area_frames)
    {
        _wave_sprite.reset();
        return;
    }

    _wave_sprite->set_tiles(bn::sprite_items::wave.tiles_item(), (_area_frames / 3) % 2);
    _wave_sprite->set_position(_position);
}

void player::_update_sprite(bool moving)
{
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer());

    if(_charge_frames && (_charge_frames / 3) % 2)
    {
        _sprite.set_item(*body().sprite, species_frames::white);
        _sprite.set_visible(true);
        return;
    }

    if(_transform_frames)
    {
        bool show_target = (_transform_frames / 3) % 2 == 0 && _transform_frames < transform_frames * 2 / 3;
        const bn::sprite_item& item = show_target ? *species::get(_transform_target).sprite : bn::sprite_items::ditto;
        _sprite.set_item(item, species_frames::white);
        _sprite.set_visible(true);
        return;
    }

    bool squashed = _dodge_frames && _form;

    if(_digging)
    {
        _sprite.set_item(*body().sprite, species_frames::mound);
        _sprite.set_visible(true);
        return;
    }

    if(_dodge_frames && ! _form)
    {
        _sprite.set_item(bn::sprite_items::ditto, species_frames::ditto_flat);
    }
    else if(squashed)
    {
        _sprite.set_item(*body().sprite, species_frames::own_walk);
    }
    else if(_switch_flash_frames)
    {
        _sprite.set_item(*body().sprite, species_frames::white);
    }
    else
    {
        if(moving)
        {
            ++_walk_frames;
        }
        else
        {
            _walk_frames = 0;
        }

        bool flailing = _wave_sprite.has_value();
        int walk_frame = flailing ? 1 : (_walk_frames / 8) % 2;
        _sprite.set_item(*body().sprite, species_frames::own_walk + walk_frame);
    }

    if(squashed)
    {
        _sprite.set_vertical_scale(dodge_squash);
        _sprite.set_y(_position.y() + _sprite.shape_size().height() * (1 - dodge_squash) / 2);
    }
    else if(_sprite.affine_mat())
    {
        _sprite.remove_affine_mat();
    }

    _sprite.set_horizontal_flip(directions::vectors[_aim].x() < 0);

    bool blink = _invulnerable_frames && (_invulnerable_frames / 4) % 2;
    _sprite.set_visible(! blink);
}
