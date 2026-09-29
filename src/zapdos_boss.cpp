#include "zapdos_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_electric_projectiles.h"
#include "bn_sprite_items_projectiles.h"
#include "bn_sprite_items_zapdos.h"

#include "attacks.h"
#include "directions.h"
#include "projectile_frames.h"

namespace
{
    constexpr int boss_hp = 180;
    constexpr int intro_frames = 40;
    constexpr int windup_frames = 24;
    constexpr int peck_frames = 18;
    constexpr int burst_interval = 90;
    constexpr int burst_spacing = 6;
    constexpr int burst_shots = 3;
    constexpr int peck_interval = 200;
    constexpr int thunder_interval = 260;
    constexpr int thunder_warning_frames = 45;
    constexpr int thunder_spread = 56;
    constexpr int thunder_life = 8;
    constexpr int thunder_half_size = 9;
    constexpr int orbit_radius = 72;
    constexpr int orbit_step = 2;
    constexpr int peck_radius = 16;
    constexpr bn::fixed fly_speed = 1.2;
    constexpr bn::fixed peck_speed = 3.6;
    constexpr bn::fixed shock_speed_scale = 0.9;
}

zapdos_boss::zapdos_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::zapdos, boss_hp, position),
    _sprite(bn::sprite_items::zapdos.create_sprite(position, species_frames::walk)),
    _camera(camera),
    _thundershock(wild_attack(move_id::thundershock)),
    _thunder(wild_attack(move_id::thunder)),
    _drill_peck(wild_attack(move_id::drill_peck)),
    _state_frames(intro_frames),
    _burst_timer(burst_interval),
    _peck_timer(peck_interval),
    _thunder_timer(thunder_interval / 2)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void zapdos_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                         message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    if(_thunder_frames && ! --_thunder_frames)
    {
        _drop_thunder(projectiles);
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
        if(! _charged && _hp * 2 < _max_hp)
        {
            _charged = true;
            messages.show("ZAPDOS is overflowing with power!");
        }

        _orbit_degrees = (_orbit_degrees + orbit_step) % 360;
        bn::fixed_point orbit(bn::degrees_lut_cos(_orbit_degrees) * orbit_radius,
                              bn::degrees_lut_sin(_orbit_degrees) * orbit_radius / 2);
        bn::fixed_point goal = target + orbit;
        bn::fixed_point delta = goal - _position;

        if(bn::abs(delta.x()) + bn::abs(delta.y()) > 4)
        {
            static_cast<void>(walk(directions::toward(_position, goal) * fly_speed, true));
        }

        if(--_burst_timer <= 0)
        {
            _burst_left = burst_shots;
            _burst_timer = _scaled(burst_interval) + random.get_int(30);
        }

        if(_burst_left && _burst_timer % burst_spacing == 0)
        {
            --_burst_left;
            attacks::shoot(projectiles, _thundershock, _position, directions::toward(_position, target),
                           shock_speed_scale);
        }

        if(--_thunder_timer <= 0)
        {
            messages.show("ZAPDOS used THUNDER!");
            _start_thunder(target, random);
            _thunder_timer = _scaled(thunder_interval) + random.get_int(60);
        }

        if(--_peck_timer <= 0)
        {
            _state = state::windup;
            _state_frames = windup_frames;
            _direction = directions::toward(_position, target);
        }
        break;
    }

    case state::windup:
        if(! --_state_frames)
        {
            messages.show("ZAPDOS used DRILL PECK!");
            _state = state::pecking;
            _state_frames = peck_frames;
        }
        break;

    case state::pecking:
        if(! walk(_direction * peck_speed, true) || ! --_state_frames)
        {
            _state = state::roaming;
            _peck_timer = _scaled(peck_interval) + random.get_int(60);
        }
        break;

    default:
        break;
    }

    _update_sprite();
}

bool zapdos_boss::touches(const bn::fixed_point& point) const
{
    if(_state != state::pecking)
    {
        return false;
    }

    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < peck_radius && bn::abs(delta.y()) < peck_radius;
}

int zapdos_boss::_scaled(int frames) const
{
    return _charged ? frames * 7 / 10 : frames;
}

void zapdos_boss::_start_thunder(const bn::fixed_point& target, bn::random& random)
{
    _thunder_markers.clear();

    for(int index = 0; index < _thunder_markers.max_size(); ++index)
    {
        bn::fixed_point offset(random.get_int(thunder_spread * 2) - thunder_spread,
                               random.get_int(thunder_spread * 2) - thunder_spread);
        bn::fixed_point position = index == 0 ? target : target + offset;
        bn::sprite_ptr marker = bn::sprite_items::projectiles.create_sprite(position, projectile_frames::impact);
        marker.set_camera(_camera);
        marker.set_z_order(-900);
        _thunder_markers.push_back(bn::move(marker));
    }

    _thunder_frames = thunder_warning_frames;
}

void zapdos_boss::_drop_thunder(enemy_projectiles& projectiles)
{
    for(bn::sprite_ptr& marker : _thunder_markers)
    {
        bn::fixed_point position = marker.position();
        projectiles.spawn(bn::sprite_items::electric_projectiles.create_sprite(position, electric_frames::bolt),
                          position, bn::fixed_point(), _thunder, thunder_life, thunder_half_size, false);
    }

    _thunder_markers.clear();
}

void zapdos_boss::_update_sprite()
{
    int frame = species_frames::walk + (_walk_frames / 8) % 2;

    if(_flash_frames || (_state == state::windup && (_state_frames / 3) % 2))
    {
        frame = species_frames::white;
    }
    else if(_charged && (_walk_frames / 6) % 4 == 0)
    {
        frame = species_frames::charging;
    }

    for(bn::sprite_ptr& marker : _thunder_markers)
    {
        marker.set_visible((_thunder_frames / 3) % 2 == 0);
    }

    bool visible = _state != state::intro || (_state_frames / 3) % 2;
    _sprite.set_tiles(bn::sprite_items::zapdos.tiles_item(), frame);
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer() - 8);
    _sprite.set_visible(visible);
}
