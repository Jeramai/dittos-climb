#include "articuno_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_articuno.h"
#include "bn_sprite_items_projectiles.h"

#include "attacks.h"
#include "directions.h"
#include "projectile_frames.h"

namespace
{
    constexpr int boss_hp = 200;
    constexpr int intro_frames = 40;
    constexpr int beam_charge_frames = 50;
    constexpr int charge_frames = 40;
    constexpr int sweep_frames = 45;
    constexpr int shard_interval = 80;
    constexpr int shard_spacing = 6;
    constexpr int shard_shots = 3;
    constexpr int blizzard_interval = 260;
    constexpr int beam_interval = 320;
    constexpr int sky_interval = 380;
    constexpr int blizzard_clouds = 8;
    constexpr int orbit_radius = 76;
    constexpr int orbit_step = 1;
    constexpr int sweep_radius = 16;
    constexpr int marker_spacing = 24;
    constexpr bn::fixed fly_speed = 1.1;
    constexpr bn::fixed sweep_speed = 4.2;
    constexpr bn::fixed shard_speed_scale = 0.85;
}

articuno_boss::articuno_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::articuno, boss_hp, position),
    _sprite(bn::sprite_items::articuno.create_sprite(position, species_frames::walk)),
    _camera(camera),
    _ice_shard(wild_attack(move_id::ice_shard)),
    _blizzard(wild_attack(move_id::blizzard)),
    _ice_beam(wild_attack(move_id::ice_beam)),
    _sky_attack(wild_attack(move_id::sky_attack)),
    _state_frames(intro_frames),
    _shard_timer(shard_interval),
    _blizzard_timer(blizzard_interval / 2),
    _beam_timer(beam_interval / 2),
    _sky_timer(sky_interval)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void articuno_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                           message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    ++_walk_frames;

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
            messages.show("The air around ARTICUNO freezes!");
        }

        _orbit_degrees = (_orbit_degrees + orbit_step) % 360;
        bn::fixed_point goal = target + bn::fixed_point(bn::degrees_lut_cos(_orbit_degrees) * orbit_radius,
                                                        -bn::degrees_lut_sin(_orbit_degrees) * orbit_radius / 2);
        bn::fixed_point delta = goal - _position;

        if(bn::abs(delta.x()) + bn::abs(delta.y()) > 4)
        {
            static_cast<void>(walk(directions::toward(_position, goal) * fly_speed, true));
        }

        bn::fixed_point aim = directions::toward(_position, target);

        if(--_shard_timer <= 0)
        {
            _burst_left = shard_shots;
            _shard_timer = _scaled(shard_interval) + random.get_int(30);
        }

        if(_burst_left && _shard_timer % shard_spacing == 0)
        {
            --_burst_left;
            attacks::shoot(projectiles, _ice_shard, _position, aim, shard_speed_scale);
        }

        if(--_blizzard_timer <= 0)
        {
            messages.show("ARTICUNO used BLIZZARD!");

            for(int index = 0; index < blizzard_clouds; ++index)
            {
                bn::fixed_point direction = attacks::rotate(aim, index * 360 / blizzard_clouds);
                attacks::cloud(projectiles, _blizzard, _position + direction * 14, direction, 1);
            }

            _blizzard_timer = _scaled(blizzard_interval) + random.get_int(60);
        }

        if(--_beam_timer <= 0)
        {
            messages.show("ARTICUNO is gathering cold air...");
            _state = state::beam_charge;
            _state_frames = beam_charge_frames;
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
        else if(--_sky_timer <= 0)
        {
            _state = state::charging;
            _state_frames = charge_frames;
        }
        break;
    }

    case state::beam_charge:
        if(! --_state_frames)
        {
            _beam_markers.clear();
            messages.show("ARTICUNO used ICE BEAM!");
            attacks::beam(projectiles, _ice_beam, _position + bn::fixed_point(0, 4), _direction);
            _state = state::roaming;
            _beam_timer = _scaled(beam_interval) + random.get_int(60);
        }
        break;

    case state::charging:
        _direction = directions::toward(_position, target);

        if(! --_state_frames)
        {
            messages.show("ARTICUNO dove at DITTO!");
            _state = state::sweeping;
            _state_frames = sweep_frames;
        }
        break;

    case state::sweeping:
        if(! walk(_direction * sweep_speed, true) || ! --_state_frames)
        {
            _state = state::roaming;
            _sky_timer = _scaled(sky_interval) + random.get_int(60);
        }
        break;

    default:
        break;
    }

    _update_sprite();
}

bool articuno_boss::touches(const bn::fixed_point& point) const
{
    if(_state != state::sweeping)
    {
        return false;
    }

    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < sweep_radius && bn::abs(delta.y()) < sweep_radius;
}

int articuno_boss::_scaled(int frames) const
{
    return _furious ? frames * 7 / 10 : frames;
}

void articuno_boss::_update_sprite()
{
    int frame = species_frames::walk + (_walk_frames / 8) % 2;

    if(_flash_frames)
    {
        frame = species_frames::white;
    }
    else if(_state == state::charging || _state == state::beam_charge)
    {
        frame = (_state_frames / 3) % 2 ? species_frames::charging : species_frames::white;
    }
    else if(_state == state::sweeping)
    {
        frame = species_frames::charging;
    }

    for(bn::sprite_ptr& marker : _beam_markers)
    {
        marker.set_visible((_state_frames / 3) % 2 == 0);
    }

    bool visible = _state != state::intro || (_state_frames / 3) % 2 == 0;
    _sprite.set_tiles(bn::sprite_items::articuno.tiles_item(), frame);
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer() - 8);
    _sprite.set_visible(visible);
}
