#include "venusaur_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_projectiles.h"
#include "bn_sprite_items_venusaur.h"

#include "attacks.h"
#include "directions.h"
#include "projectile_frames.h"

namespace
{
    constexpr int boss_hp = 150;
    constexpr int intro_frames = 40;
    constexpr int razor_frames = 90;
    constexpr int powder_frames = 330;
    constexpr int beam_frames = 320;
    constexpr int charge_frames = 60;
    constexpr int keep_distance = 64;
    constexpr bn::fixed walk_speed = 0.25;
    constexpr bn::fixed leaf_speed_scale = 0.7;
    constexpr int marker_spacing = 24;
}

venusaur_boss::venusaur_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::venusaur, boss_hp, position),
    _sprite(bn::sprite_items::venusaur.create_sprite(position, species_frames::walk)),
    _camera(camera),
    _razor_leaf(wild_attack(move_id::razor_leaf)),
    _sleep_powder(wild_attack(move_id::sleep_powder)),
    _solar_beam(wild_attack(move_id::solar_beam)),
    _intro_frames(intro_frames),
    _razor_timer(razor_frames),
    _powder_timer(powder_frames / 2),
    _beam_timer(beam_frames / 2)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void venusaur_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                           message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    if(_intro_frames)
    {
        --_intro_frames;
        _update_sprite();
        return;
    }

    if(! _enraged && _hp * 2 < _max_hp)
    {
        _enraged = true;
        messages.show("VENUSAUR's flower is glowing!");
    }

    if(_charge_frames)
    {
        if(! --_charge_frames)
        {
            _beam_markers.clear();
            messages.show("VENUSAUR used SOLAR BEAM!");
            attacks::beam(projectiles, _solar_beam, _position + bn::fixed_point(0, 4), _beam_direction);
            _beam_timer = _scaled(beam_frames) + random.get_int(60);
        }

        _update_sprite();
        return;
    }

    bn::fixed_point delta = target - _position;

    if(bn::abs(delta.x()) + bn::abs(delta.y()) > keep_distance && walk(directions::toward(_position, target) * walk_speed))
    {
        ++_walk_frames;
    }

    bn::fixed_point aim = directions::toward(_position, target);

    if(--_razor_timer <= 0)
    {
        attacks::shoot(projectiles, _razor_leaf, _position, aim, leaf_speed_scale);

        _razor_timer = _scaled(razor_frames) + random.get_int(20);
    }

    if(--_powder_timer <= 0)
    {
        messages.show("VENUSAUR used SLEEP POWDER!");

        for(int index = 0; index < 4; ++index)
        {
            bn::fixed_point direction = attacks::rotate(aim, 45 + index * 90);
            attacks::cloud(projectiles, _sleep_powder, _position + direction * 12, direction, 1);
        }

        _powder_timer = _scaled(powder_frames) + random.get_int(60);
    }

    if(--_beam_timer <= 0)
    {
        messages.show("VENUSAUR is taking in sunlight!");
        _charge_frames = charge_frames;
        _beam_direction = aim;

        for(int index = 1; index <= _beam_markers.max_size(); ++index)
        {
            bn::fixed_point position = _position + aim * (index * marker_spacing);
            bn::sprite_ptr marker = bn::sprite_items::projectiles.create_sprite(position, projectile_frames::impact);
            marker.set_camera(_camera);
            marker.set_z_order(-900);
            _beam_markers.push_back(bn::move(marker));
        }
    }

    _update_sprite();
}

int venusaur_boss::_scaled(int frames) const
{
    return _enraged ? frames * 7 / 10 : frames;
}

void venusaur_boss::_update_sprite()
{
    int frame = species_frames::walk + (_walk_frames / 18) % 2;

    if(_flash_frames)
    {
        frame = species_frames::white;
    }
    else if(_charge_frames)
    {
        frame = (_charge_frames / 4) % 2 ? species_frames::charging : species_frames::walk;
    }

    for(bn::sprite_ptr& marker : _beam_markers)
    {
        marker.set_visible((_charge_frames / 3) % 2 == 0);
    }

    _sprite.set_tiles(bn::sprite_items::venusaur.tiles_item(), frame);
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer() - 8);
}
