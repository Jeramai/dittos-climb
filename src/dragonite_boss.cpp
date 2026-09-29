#include "dragonite_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_dragonite.h"
#include "bn_sprite_items_projectiles.h"

#include "attacks.h"
#include "directions.h"
#include "projectile_frames.h"

namespace
{
    constexpr int boss_hp = 250;
    constexpr int intro_frames = 50;
    constexpr int windup_frames = 26;
    constexpr int lunge_frames = 16;
    constexpr int lunge_count = 3;
    constexpr int tired_frames = 70;
    constexpr int recharge_frames = 100;
    constexpr int charge_frames = 50;
    constexpr int twister_interval = 90;
    constexpr int rage_interval = 200;
    constexpr int outrage_interval = 280;
    constexpr int beam_interval = 360;
    constexpr int keep_distance = 60;
    constexpr int lunge_radius = 16;
    constexpr int marker_spacing = 24;
    constexpr bn::fixed fly_speed = 1.1;
    constexpr bn::fixed lunge_speed = 4;
    constexpr bn::fixed shot_speed_scale = 0.8;
}

dragonite_boss::dragonite_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::dragonite, boss_hp, position),
    _sprite(bn::sprite_items::dragonite.create_sprite(position, species_frames::walk)),
    _camera(camera),
    _twister(wild_attack(move_id::twister)),
    _dragon_rage(wild_attack(move_id::dragon_rage)),
    _outrage(wild_attack(move_id::outrage)),
    _hyper_beam(wild_attack(move_id::hyper_beam)),
    _state_frames(intro_frames),
    _twister_timer(twister_interval),
    _rage_timer(rage_interval / 2),
    _outrage_timer(outrage_interval / 2),
    _beam_timer(beam_interval)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void dragonite_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                            message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    ++_walk_frames;
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
    {
        if(! _furious && _hp * 2 < _max_hp)
        {
            _furious = true;
            messages.show("DRAGONITE is enraged!");
        }

        bn::fixed_point delta = target - _position;

        if(bn::abs(delta.x()) + bn::abs(delta.y()) > keep_distance)
        {
            static_cast<void>(walk(aim * fly_speed, true));
        }

        if(--_twister_timer <= 0)
        {
            for(int offset : { -15, 0, 15 })
            {
                attacks::shoot(projectiles, _twister, _position, attacks::rotate(aim, offset), shot_speed_scale);
            }

            _twister_timer = _scaled(twister_interval) + random.get_int(30);
        }

        if(--_rage_timer <= 0)
        {
            messages.show("DRAGONITE used DRAGON RAGE!");

            for(int offset : { -40, -20, 0, 20, 40 })
            {
                attacks::shoot(projectiles, _dragon_rage, _position, attacks::rotate(aim, offset), shot_speed_scale);
            }

            _rage_timer = _scaled(rage_interval) + random.get_int(60);
        }

        if(--_beam_timer <= 0)
        {
            messages.show("DRAGONITE is charging HYPER BEAM!");
            _state = state::charging;
            _state_frames = charge_frames;
            _direction = aim;

            for(int index = 1; index <= _beam_markers.max_size(); ++index)
            {
                bn::fixed_point position = _position + aim * (index * marker_spacing);
                bn::sprite_ptr marker = bn::sprite_items::projectiles.create_sprite(position, projectile_frames::impact);
                marker.set_camera(_camera);
                marker.set_z_order(-900);
                _beam_markers.push_back(bn::move(marker));
            }
        }
        else if(--_outrage_timer <= 0)
        {
            messages.show("DRAGONITE used OUTRAGE!");
            _lunges_left = lunge_count;
            _state = state::windup;
            _state_frames = windup_frames;
        }
        break;
    }

    case state::windup:
        _direction = aim;

        if(! --_state_frames)
        {
            _state = state::rampaging;
            _state_frames = lunge_frames;
        }
        break;

    case state::rampaging:
        if(! walk(_direction * lunge_speed, true) || ! --_state_frames)
        {
            if(--_lunges_left > 0)
            {
                _state = state::windup;
                _state_frames = windup_frames / 2;
            }
            else
            {
                messages.show("DRAGONITE is tired out!");
                _state = state::tired;
                _state_frames = tired_frames;
                _outrage_timer = _scaled(outrage_interval) + random.get_int(60);
            }
        }
        break;

    case state::charging:
        if(! --_state_frames)
        {
            _beam_markers.clear();
            messages.show("DRAGONITE used HYPER BEAM!");
            attacks::beam(projectiles, _hyper_beam, _position + bn::fixed_point(0, 4), _direction);
            messages.show("DRAGONITE must recharge!");
            _state = state::tired;
            _state_frames = recharge_frames;
            _beam_timer = _scaled(beam_interval) + random.get_int(60);
        }
        break;

    case state::tired:
        if(! --_state_frames)
        {
            _state = state::roaming;
        }
        break;

    default:
        break;
    }

    _update_sprite();
}

bool dragonite_boss::touches(const bn::fixed_point& point) const
{
    if(_state != state::rampaging)
    {
        return false;
    }

    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < lunge_radius && bn::abs(delta.y()) < lunge_radius;
}

int dragonite_boss::_scaled(int frames) const
{
    return _furious ? frames * 7 / 10 : frames;
}

void dragonite_boss::_update_sprite()
{
    int frame = species_frames::walk + (_walk_frames / 10) % 2;

    if(_flash_frames || (_state == state::windup && (_state_frames / 3) % 2))
    {
        frame = species_frames::white;
    }
    else if(_state == state::charging || _state == state::rampaging)
    {
        frame = species_frames::charging;
    }

    bn::fixed shake = _state == state::tired ? bn::fixed((_state_frames / 4) % 2 ? 1 : 0) : bn::fixed(0);
    bool visible = _state != state::intro || (_state_frames / 3) % 2;

    for(bn::sprite_ptr& marker : _beam_markers)
    {
        marker.set_visible((_state_frames / 3) % 2 == 0);
    }

    _sprite.set_tiles(bn::sprite_items::dragonite.tiles_item(), frame);
    _sprite.set_position(_position + bn::fixed_point(0, shake));
    _sprite.set_z_order(-_position.y().round_integer() - 8);
    _sprite.set_visible(visible);
}
