#include "gyarados_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_gyarados.h"
#include "bn_sprite_items_magikarp.h"
#include "bn_sprite_items_projectiles.h"

#include "attacks.h"
#include "directions.h"
#include "projectile_frames.h"

namespace
{
    constexpr int boss_hp = 170;
    constexpr int magikarp_frames = 240;
    constexpr int notice_distance = 56;
    constexpr int evolving_frames = 80;
    constexpr int windup_frames = 24;
    constexpr int lunge_frames = 22;
    constexpr int charge_frames = 45;
    constexpr int bite_interval = 150;
    constexpr int thrash_bite_interval = 80;
    constexpr int pump_interval = 280;
    constexpr int rage_interval = 120;
    constexpr int bite_radius = 16;
    constexpr int marker_spacing = 24;
    constexpr bn::fixed walk_speed = 0.5;
    constexpr bn::fixed lunge_speed = 3.2;
    constexpr bn::fixed rage_speed_scale = 0.8;
}

gyarados_boss::gyarados_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::gyarados, boss_hp, position),
    _sprite(bn::sprite_items::magikarp.create_sprite(position, species_frames::walk)),
    _camera(camera),
    _bite(wild_attack(move_id::bite)),
    _hydro_pump(wild_attack(move_id::hydro_pump)),
    _dragon_rage(wild_attack(move_id::dragon_rage)),
    _state_frames(magikarp_frames),
    _bite_timer(bite_interval),
    _pump_timer(pump_interval / 2),
    _rage_timer(rage_interval)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void gyarados_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                           message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    bn::fixed_point delta = target - _position;
    bn::fixed distance = bn::abs(delta.x()) + bn::abs(delta.y());

    switch(_state)
    {

    case state::magikarp:
        ++_walk_frames;

        if(--_state_frames <= 0 || distance < notice_distance)
        {
            messages.show("What? MAGIKARP is evolving!");
            _state = state::evolving;
            _state_frames = evolving_frames;
        }
        break;

    case state::evolving:
        _evolve_step(messages);
        break;

    case state::roaming:
        if(! _thrashing && _hp * 10 < _max_hp * 4)
        {
            _thrashing = true;
            messages.show("GYARADOS is thrashing about!");
        }

        if(walk(directions::toward(_position, target) * walk_speed, true))
        {
            ++_walk_frames;
        }

        if(--_rage_timer <= 0)
        {
            attacks::shoot(projectiles, _dragon_rage, _position, directions::toward(_position, target),
                           rage_speed_scale);
            _rage_timer = rage_interval + random.get_int(40);
        }

        if(--_pump_timer <= 0)
        {
            messages.show("GYARADOS is taking aim...");
            _state = state::charging;
            _state_frames = charge_frames;
            _direction = directions::toward(_position, target);

            for(int index = 1; index <= _beam_markers.max_size(); ++index)
            {
                bn::fixed_point position = _position + _direction * (index * marker_spacing);
                bn::sprite_ptr marker = bn::sprite_items::projectiles.create_sprite(position, projectile_frames::impact);
                marker.set_camera(_camera);
                marker.set_z_order(-900);
                _beam_markers.push_back(bn::move(marker));
            }
        }
        else if(--_bite_timer <= 0)
        {
            _state = state::windup;
            _state_frames = windup_frames;
            _direction = directions::toward(_position, target);
        }
        break;

    case state::windup:
        if(! --_state_frames)
        {
            messages.show("GYARADOS used BITE!");
            _state = state::lunging;
            _state_frames = lunge_frames;
        }
        break;

    case state::lunging:
        if(! walk(_direction * lunge_speed, true) || ! --_state_frames)
        {
            _state = state::roaming;
            _bite_timer = (_thrashing ? thrash_bite_interval : bite_interval) + random.get_int(40);
        }
        break;

    case state::charging:
        if(! --_state_frames)
        {
            _beam_markers.clear();
            messages.show("GYARADOS used HYDRO PUMP!");
            attacks::beam(projectiles, _hydro_pump, _position + bn::fixed_point(0, 4), _direction);
            _state = state::roaming;
            _pump_timer = pump_interval + random.get_int(60);
        }
        break;

    default:
        break;
    }

    _update_sprite();
}

bool gyarados_boss::touches(const bn::fixed_point& point) const
{
    if(_state != state::lunging)
    {
        return false;
    }

    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < bite_radius && bn::abs(delta.y()) < bite_radius;
}

void gyarados_boss::_evolve_step(message_box& messages)
{
    --_state_frames;
    bool show_gyarados = (_state_frames / 4) % 2 == 0 && _state_frames < evolving_frames * 2 / 3;
    const bn::sprite_item& item = show_gyarados ? bn::sprite_items::gyarados : bn::sprite_items::magikarp;
    _sprite.set_item(item, species_frames::white);

    if(! _state_frames)
    {
        messages.show("MAGIKARP evolved into GYARADOS!");
        _sprite.set_item(bn::sprite_items::gyarados, species_frames::walk);
        _state = state::roaming;
    }
}

void gyarados_boss::_update_sprite()
{
    if(_state == state::evolving)
    {
        _sprite.set_position(_position);
        return;
    }

    if(_state == state::magikarp)
    {
        bn::fixed hop = bn::abs(bn::degrees_lut_sin((_walk_frames * 8) % 360)) * 6;
        _sprite.set_tiles(bn::sprite_items::magikarp.tiles_item(), species_frames::walk + (_walk_frames / 12) % 2);
        _sprite.set_position(_position - bn::fixed_point(0, hop));
        return;
    }

    int frame = species_frames::walk + (_walk_frames / 16) % 2;

    if(_flash_frames || (_state == state::windup && (_state_frames / 3) % 2))
    {
        frame = species_frames::white;
    }
    else if(_state == state::charging)
    {
        frame = (_state_frames / 4) % 2 ? species_frames::charging : species_frames::walk;
    }

    for(bn::sprite_ptr& marker : _beam_markers)
    {
        marker.set_visible((_state_frames / 3) % 2 == 0);
    }

    _sprite.set_tiles(bn::sprite_items::gyarados.tiles_item(), frame);
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer() - 8);
}
