#include "hitmon_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_hitmonchan.h"
#include "bn_sprite_items_hitmonlee.h"
#include "bn_sprite_items_projectiles.h"

#include "attacks.h"
#include "directions.h"
#include "projectile_frames.h"

namespace
{
    constexpr int boss_hp = 220;
    constexpr int intro_frames = 50;
    constexpr int windup_frames = 28;
    constexpr int kick_dash_frames = 26;
    constexpr int punch_dash_frames = 16;
    constexpr int stunned_frames = 80;
    constexpr int crash_damage = 25;
    constexpr int strike_interval = 90;
    constexpr int combo_interval = 70;
    constexpr int combo_spacing = 8;
    constexpr int dash_interval = 240;
    constexpr int counter_interval = 300;
    constexpr int counter_frames = 60;
    constexpr int shockwave_count = 8;
    constexpr int dash_radius = 16;
    constexpr bn::fixed walk_speed = 0.9;
    constexpr bn::fixed kick_speed = 4.5;
    constexpr bn::fixed punch_speed = 3.8;
    constexpr bn::fixed shockwave_speed = 1.6;
    constexpr int shockwave_life = 60;

    const bn::sprite_item& item(bool kicker)
    {
        return kicker ? bn::sprite_items::hitmonlee : bn::sprite_items::hitmonchan;
    }
}

hitmon_boss::hitmon_boss(const bn::fixed_point& position, const bn::camera_ptr& camera, bool kicker) :
    boss(kicker ? species_id::hitmonlee : species_id::hitmonchan, boss_hp, position),
    _sprite(item(kicker).create_sprite(position, species_frames::walk)),
    _dash_attack(wild_attack(kicker ? move_id::hi_jump_kick : move_id::mega_punch)),
    _kick(wild_attack(move_id::rolling_kick)),
    _punches{ wild_attack(move_id::fire_punch), wild_attack(move_id::ice_punch), wild_attack(move_id::thunderpunch) },
    _shockwave(wild_attack(move_id::karate_chop)),
    _kicker(kicker),
    _state_frames(intro_frames),
    _strike_timer(strike_interval),
    _dash_timer(dash_interval / 2),
    _counter_timer(counter_interval)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void hitmon_boss::announce_defeat(message_box& messages) const
{
    message_box::text message(name());
    message.append(" fainted!");
    messages.show(message);
    messages.show(_kicker ? "The DOJO MASTER offers HITMONCHAN!" : "The DOJO MASTER offers HITMONLEE!");
}

void hitmon_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                         message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    if(! _furious && _hp * 2 < _max_hp)
    {
        _furious = true;
        messages.show(_kicker ? "HITMONLEE is fired up!" : "HITMONCHAN is fired up!");
    }

    bn::fixed_point aim = directions::toward(_position, target);

    switch(_state)
    {

    case state::intro:
        if(! --_state_frames)
        {
            _state = state::roaming;
        }
        break;

    case state::roaming:
        if(walk(aim * walk_speed))
        {
            ++_walk_frames;
        }

        if(_kicker)
        {
            if(--_strike_timer <= 0)
            {
                messages.show("HITMONLEE used ROLLING KICK!");

                for(int index = 0; index < 4; ++index)
                {
                    attacks::slash(projectiles, _kick, _position, attacks::rotate(aim, index * 90));
                }

                _strike_timer = _scaled(strike_interval) + random.get_int(30);
            }
        }
        else
        {
            if(--_strike_timer <= 0)
            {
                _combo_left = 3;
                _strike_timer = _scaled(combo_interval) + random.get_int(30);
            }

            if(_combo_left && _strike_timer % combo_spacing == 0)
            {
                --_combo_left;
                attacks::slash(projectiles, _punches[_combo_index], _position, aim);
                _combo_index = (_combo_index + 1) % 3;
            }

            if(--_counter_timer <= 0)
            {
                messages.show("HITMONCHAN is ready to COUNTER!");
                _state = state::countering;
                _state_frames = counter_frames;
                _hp_at_stance = _hp;
                break;
            }
        }

        if(--_dash_timer <= 0)
        {
            _state = state::windup;
            _state_frames = windup_frames;
            _direction = aim;
        }
        break;

    case state::windup:
        if(! --_state_frames)
        {
            messages.show(_kicker ? "HITMONLEE used HI JUMP KICK!" : "HITMONCHAN used MEGA PUNCH!");
            _state = state::dashing;
            _state_frames = _kicker ? kick_dash_frames : punch_dash_frames;
        }
        break;

    case state::dashing:
        if(! walk(_direction * (_kicker ? kick_speed : punch_speed)))
        {
            if(_kicker)
            {
                _crash(messages);
            }
            else
            {
                _state = state::roaming;
            }
        }
        else if(! --_state_frames)
        {
            _state = state::roaming;
        }

        if(_state == state::roaming)
        {
            _dash_timer = _scaled(dash_interval) + random.get_int(60);
        }
        break;

    case state::stunned:
        if(! --_state_frames)
        {
            _state = state::roaming;
            _dash_timer = _scaled(dash_interval) + random.get_int(60);
        }
        break;

    case state::countering:
        if(_hp < _hp_at_stance)
        {
            _counter(projectiles, messages);
        }
        else if(! --_state_frames)
        {
            _state = state::roaming;
            _counter_timer = _scaled(counter_interval) + random.get_int(60);
        }
        break;

    default:
        break;
    }

    _update_sprite();
}

bool hitmon_boss::touches(const bn::fixed_point& point) const
{
    if(_state != state::dashing)
    {
        return false;
    }

    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < dash_radius && bn::abs(delta.y()) < dash_radius;
}

int hitmon_boss::_scaled(int frames) const
{
    return _furious ? frames * 7 / 10 : frames;
}

void hitmon_boss::_crash(message_box& messages)
{
    messages.show("HITMONLEE kept going and crashed!");
    _hp -= crash_damage;
    _flash_frames = 8;
    _state = state::stunned;
    _state_frames = stunned_frames;
}

void hitmon_boss::_counter(enemy_projectiles& projectiles, message_box& messages)
{
    messages.show("HITMONCHAN used COUNTER!");

    for(int index = 0; index < shockwave_count; ++index)
    {
        bn::fixed_point direction(bn::degrees_lut_cos(index * 45), bn::degrees_lut_sin(index * 45));
        bn::fixed_point origin = _position + direction * 10;
        projectiles.spawn(bn::sprite_items::projectiles.create_sprite(origin, projectile_frames::enemy_shot), origin,
                          direction * shockwave_speed, _shockwave, shockwave_life, 5, true);
    }

    _state = state::roaming;
    _counter_timer = _scaled(counter_interval) + 60;
}

void hitmon_boss::_update_sprite()
{
    int frame = species_frames::walk + (_walk_frames / 12) % 2;

    if(_flash_frames || (_state == state::windup && (_state_frames / 3) % 2))
    {
        frame = species_frames::white;
    }
    else if(_state == state::countering || _state == state::dashing)
    {
        frame = species_frames::charging;
    }

    bn::fixed shake = _state == state::stunned ? bn::fixed((_state_frames / 2) % 2 ? 1 : -1) : bn::fixed(0);
    bool visible = _state != state::intro || (_state_frames / 3) % 2;
    _sprite.set_tiles(item(_kicker).tiles_item(), frame);
    _sprite.set_position(_position + bn::fixed_point(shake, 0));
    _sprite.set_z_order(-_position.y().round_integer() - 8);
    _sprite.set_visible(visible);
}
