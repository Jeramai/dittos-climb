#include "moltres_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_moltres.h"

#include "attacks.h"
#include "directions.h"

namespace
{
    constexpr int boss_hp = 190;
    constexpr int intro_frames = 40;
    constexpr int charge_frames = 45;
    constexpr int sweep_frames = 50;
    constexpr int flame_interval = 100;
    constexpr int spin_interval = 240;
    constexpr int sky_interval = 300;
    constexpr int spin_clouds = 6;
    constexpr int orbit_radius = 80;
    constexpr int orbit_step = 1;
    constexpr int sweep_radius = 16;
    constexpr bn::fixed fly_speed = 1.1;
    constexpr bn::fixed sweep_speed = 4.5;
    constexpr bn::fixed flame_speed_scale = 0.8;
}

moltres_boss::moltres_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::moltres, boss_hp, position),
    _sprite(bn::sprite_items::moltres.create_sprite(position, species_frames::walk)),
    _flamethrower(wild_attack(move_id::flamethrower)),
    _fire_spin(wild_attack(move_id::fire_spin)),
    _sky_attack(wild_attack(move_id::sky_attack)),
    _state_frames(intro_frames),
    _flame_timer(flame_interval),
    _spin_timer(spin_interval / 2),
    _sky_timer(sky_interval)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void moltres_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
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
        if(! _hotter && _hp * 2 < _max_hp)
        {
            _hotter = true;
            messages.show("MOLTRES's flames burn hotter!");
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

        if(--_flame_timer <= 0)
        {
            attacks::shoot(projectiles, _flamethrower, _position, aim, flame_speed_scale);
            _flame_timer = _scaled(flame_interval) + random.get_int(30);
        }

        if(--_spin_timer <= 0)
        {
            messages.show("MOLTRES used FIRE SPIN!");

            for(int index = 0; index < spin_clouds; ++index)
            {
                bn::fixed_point direction = attacks::rotate(aim, index * 360 / spin_clouds);
                attacks::cloud(projectiles, _fire_spin, _position + direction * 14, direction, 1);
            }

            _spin_timer = _scaled(spin_interval) + random.get_int(60);
        }

        if(--_sky_timer <= 0)
        {
            messages.show("MOLTRES is glowing!");
            _state = state::charging;
            _state_frames = charge_frames;
        }
        break;
    }

    case state::charging:
        _direction = directions::toward(_position, target);

        if(! --_state_frames)
        {
            messages.show("MOLTRES used SKY ATTACK!");
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

bool moltres_boss::touches(const bn::fixed_point& point) const
{
    if(_state != state::sweeping)
    {
        return false;
    }

    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < sweep_radius && bn::abs(delta.y()) < sweep_radius;
}

int moltres_boss::_scaled(int frames) const
{
    return _hotter ? frames * 7 / 10 : frames;
}

void moltres_boss::_update_sprite()
{
    int frame = species_frames::walk + (_walk_frames / 8) % 2;

    if(_flash_frames)
    {
        frame = species_frames::white;
    }
    else if(_state == state::charging)
    {
        frame = (_state_frames / 3) % 2 ? species_frames::charging : species_frames::white;
    }
    else if(_state == state::sweeping)
    {
        frame = species_frames::charging;
    }

    bool visible = _state != state::intro || (_state_frames / 3) % 2 == 0;
    _sprite.set_tiles(bn::sprite_items::moltres.tiles_item(), frame);
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer() - 8);
    _sprite.set_visible(visible);
}
