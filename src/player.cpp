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
    _area_attack(combat::make_attack(move_id::struggle, pokemon_type::normal, pokemon_type::none))
{
    _sprite.set_camera(camera);
}

bool player::update(player_projectiles& projectiles, message_box& messages, const species_id* outline_below)
{
    bool used_outline = false;

    if(_area_frames)
    {
        --_area_frames;
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

    int input_x = int(bn::keypad::right_held()) - int(bn::keypad::left_held());
    int input_y = int(bn::keypad::down_held()) - int(bn::keypad::up_held());
    int move_direction = directions::from_input(input_x, input_y);
    bool moving = move_direction >= 0;

    if(moving && ! bn::keypad::r_held() && ! _dash_frames)
    {
        _aim = move_direction;
    }

    if(_cooldown)
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
                _move(directions::vectors[move_direction] * body().speed);
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
    if(_invulnerable_frames || _transform_frames)
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

void player::take_hit(const attack& hit, message_box& messages)
{
    const species_data& current = body();
    hit_result result = combat::resolve(hit, current.type_1, current.type_2);
    _invulnerable_frames = hurt_invulnerable_frames;

    if(! result.damage)
    {
        messages.show("It doesn't affect DITTO...");
        return;
    }

    messages.show(combat::effectiveness_message(result.effectiveness));

    if(! _form)
    {
        _hp = bn::max(_hp - result.damage, 0);
        return;
    }

    _form->hp -= result.damage;

    if(_form->hp <= 0)
    {
        message_box::text message(current.name);
        message.append(" fainted!");
        messages.show(message);
        messages.show("DITTO lost its shape!");
        _form.reset();
        _switch_flash_frames = switch_flash_frames;
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

void player::start_transform(species_id target)
{
    _transform_target = target;
    _transform_frames = transform_frames;
    _dash_frames = 0;
    _area_frames = 0;
    _dodge_frames = 0;
}

void player::set_position(const bn::fixed_point& position)
{
    _position = position;
    _dash_frames = 0;
    _dodge_frames = 0;
    _area_frames = 0;
    _update_sprite(false);
}

void player::restore()
{
    _hp = ditto().hp;

    if(_form)
    {
        const species_data& shape = species::get(_form->species);
        _form->hp = shape.hp * form_hp_scale;
        _form->pp_b = moves::get(shape.move_b).pp;
    }
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

    case move_pattern::wave:
        _start_area(hit, data.life, wave_half_size);
        _wave_sprite = bn::sprite_items::wave.create_sprite(_position, 0);
        _wave_sprite->set_camera(_camera);
        _wave_sprite->set_z_order(-999);
        break;

    default:
        messages.show("DITTO used TRANSFORM!");
        messages.show("But it failed!");
        break;
    }
}

void player::_finish_transform(message_box& messages)
{
    const species_data& target = species::get(_transform_target);
    _form = form{ _transform_target, target.hp * form_hp_scale,
                  moves::get(target.move_b).pp };
    _hp = ditto().hp;

    message_box::text message("DITTO transformed into ");
    message.append(target.name);
    message.append("!");
    messages.show(message);
}

void player::_move(const bn::fixed_point& delta)
{
    bn::fixed_point next(_position.x() + delta.x(), _position.y());

    if(! room::feet_are_blocked(next))
    {
        _position = next;
    }

    next = bn::fixed_point(_position.x(), _position.y() + delta.y());

    if(! room::feet_are_blocked(next))
    {
        _position = next;
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

    if(_transform_frames)
    {
        bool show_target = (_transform_frames / 3) % 2 == 0 && _transform_frames < transform_frames * 2 / 3;
        const bn::sprite_item& item = show_target ? *species::get(_transform_target).sprite : bn::sprite_items::ditto;
        _sprite.set_item(item, species_frames::white);
        _sprite.set_visible(true);
        return;
    }

    bool squashed = _dodge_frames && _form;

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
