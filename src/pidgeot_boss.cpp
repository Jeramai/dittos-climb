#include "pidgeot_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_pidgeot.h"

#include "attacks.h"
#include "directions.h"

namespace
{
    constexpr int boss_hp = 200;
    constexpr int intro_frames = 40;
    constexpr int charge_frames = 35;
    constexpr int dive_frames = 40;
    constexpr int feather_interval = 70;
    constexpr int whirlwind_interval = 280;
    constexpr int gust_interval = 360;
    constexpr int gust_frames = 90;
    constexpr int dive_interval = 240;
    constexpr int whirlwind_clouds = 4;
    constexpr int orbit_radius = 84;
    constexpr int orbit_step = 1;
    constexpr int dive_radius = 16;
    constexpr bn::fixed fly_speed = 1.2;
    constexpr bn::fixed dive_speed = 4.4;
    constexpr bn::fixed gust_strength = 1.3;
    constexpr bn::fixed feather_speed_scale = 0.85;
}

pidgeot_boss::pidgeot_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::pidgeot, boss_hp, position),
    _sprite(bn::sprite_items::pidgeot.create_sprite(position, species_frames::walk)),
    _feathers(wild_attack(move_id::gust)),
    _whirlwind(wild_attack(move_id::whirlwind)),
    _wing_attack(wild_attack(move_id::wing_attack)),
    _state_frames(intro_frames),
    _feather_timer(feather_interval),
    _whirlwind_timer(whirlwind_interval / 2),
    _gust_timer(gust_interval / 2),
    _dive_timer(dive_interval)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void pidgeot_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                          message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    if(_gust_frames)
    {
        --_gust_frames;
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
            messages.show("PIDGEOT is flying faster!");
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

        if(--_feather_timer <= 0)
        {
            attacks::shoot(projectiles, _feathers, _position, attacks::rotate(aim, -15), feather_speed_scale);
            attacks::shoot(projectiles, _feathers, _position, aim, feather_speed_scale);
            attacks::shoot(projectiles, _feathers, _position, attacks::rotate(aim, 15), feather_speed_scale);
            _feather_timer = _scaled(feather_interval) + random.get_int(30);
        }

        if(--_whirlwind_timer <= 0)
        {
            messages.show("PIDGEOT used WHIRLWIND!");

            for(int index = 0; index < whirlwind_clouds; ++index)
            {
                bn::fixed_point direction = attacks::rotate(aim, 45 + index * 90);
                attacks::cloud(projectiles, _whirlwind, _position + direction * 14, direction, 1);
            }

            _whirlwind_timer = _scaled(whirlwind_interval) + random.get_int(60);
        }

        if(--_gust_timer <= 0)
        {
            messages.show("PIDGEOT whipped up a GUST!");
            _gust = aim * gust_strength;
            _gust_frames = gust_frames;
            _gust_timer = _scaled(gust_interval) + random.get_int(60);
        }

        if(--_dive_timer <= 0)
        {
            _state = state::charging;
            _state_frames = charge_frames;
        }
        break;
    }

    case state::charging:
        _direction = directions::toward(_position, target);

        if(! --_state_frames)
        {
            messages.show("PIDGEOT used WING ATTACK!");
            _state = state::diving;
            _state_frames = dive_frames;
        }
        break;

    case state::diving:
        if(! walk(_direction * dive_speed, true) || ! --_state_frames)
        {
            _state = state::roaming;
            _dive_timer = _scaled(dive_interval) + random.get_int(60);
        }
        break;

    default:
        break;
    }

    _update_sprite();
}

bool pidgeot_boss::touches(const bn::fixed_point& point) const
{
    if(_state != state::diving)
    {
        return false;
    }

    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < dive_radius && bn::abs(delta.y()) < dive_radius;
}

int pidgeot_boss::_scaled(int frames) const
{
    return _furious ? frames * 7 / 10 : frames;
}

void pidgeot_boss::_update_sprite()
{
    int speed = _gust_frames ? 4 : 8;
    int frame = species_frames::walk + (_walk_frames / speed) % 2;

    if(_flash_frames)
    {
        frame = species_frames::white;
    }
    else if(_state == state::charging)
    {
        frame = (_state_frames / 3) % 2 ? species_frames::charging : species_frames::white;
    }
    else if(_state == state::diving)
    {
        frame = species_frames::charging;
    }

    bool visible = _state != state::intro || (_state_frames / 3) % 2;
    _sprite.set_tiles(bn::sprite_items::pidgeot.tiles_item(), frame);
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer() - 8);
    _sprite.set_visible(visible);
}
