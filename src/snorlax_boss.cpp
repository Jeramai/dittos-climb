#include "snorlax_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_projectiles.h"
#include "bn_sprite_items_snorlax.h"

#include "directions.h"
#include "projectile_frames.h"

namespace
{
    constexpr int boss_hp = 120;
    constexpr int slam_radius = 24;
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
    constexpr int shockwave_power = 40;
}

snorlax_boss::snorlax_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::snorlax, boss_hp, position),
    _sprite(bn::sprite_items::snorlax.create_sprite(position, species_frames::asleep)),
    _camera(camera),
    _slam_attack(wild_attack(move_id::body_slam)),
    _shockwave_attack(attack{ move_id::body_slam, pokemon_type::normal, shockwave_power })
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

bn::optional<boss_area_hit> snorlax_boss::area_hit() const
{
    if(! _landed_this_frame)
    {
        return bn::nullopt;
    }

    return boss_area_hit{ _position + bn::fixed_point(0, 8), slam_radius, _slam_attack };
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
        if(! _rested && _hp * 100 < _max_hp * 35)
        {
            _rested = true;
            messages.show("SNORLAX used REST!");
            messages.show("SNORLAX fell asleep!");
            _state = state::resting;
            _state_frames = resting_frames;
            break;
        }

        if(walk(directions::toward(_position, target) * walk_speed))
        {
            ++_walk_frames;
        }

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
            _hp = bn::min(_hp + rest_heal, _max_hp);
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
