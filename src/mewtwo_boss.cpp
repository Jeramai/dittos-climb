#include "mewtwo_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_mewtwo.h"

#include "attacks.h"
#include "directions.h"
#include "room.h"

namespace
{
    constexpr int boss_hp = 300;
    constexpr int intro_frames = 90;
    constexpr int barrier_frames = 90;
    constexpr int fan_interval = 80;
    constexpr int wave_interval = 220;
    constexpr int barrier_interval = 380;
    constexpr int teleport_interval = 190;
    constexpr int swift_interval = 130;
    constexpr int wave_shots = 12;
    constexpr int recover_amount = 90;
    constexpr int keep_distance = 70;
    constexpr int teleport_distance = 80;
    constexpr bn::fixed float_speed = 1;
    constexpr bn::fixed shot_speed_scale = 0.8;
}

mewtwo_boss::mewtwo_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::mewtwo, boss_hp, position),
    _sprite(bn::sprite_items::mewtwo.create_sprite(position, species_frames::walk)),
    _confusion(wild_attack(move_id::confusion)),
    _psywave(wild_attack(move_id::psywave)),
    _swift(wild_attack(move_id::swift)),
    _intro_frames(intro_frames),
    _fan_timer(fan_interval),
    _wave_timer(wave_interval / 2),
    _barrier_timer(barrier_interval),
    _teleport_timer(teleport_interval),
    _swift_timer(swift_interval)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void mewtwo_boss::announce_defeat(message_box& messages) const
{
    messages.show("MEWTWO: ...How? You are only a copy...");
}

void mewtwo_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                         message_box& messages)
{
    ++_frame_counter;

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

    if(! _recovered && _hp > 0 && _hp * 10 < _max_hp * 3)
    {
        _recovered = true;
        _hp += recover_amount;
        messages.show("MEWTWO used RECOVER!");
    }

    if(! _rising && _hp * 2 < _max_hp)
    {
        _rising = true;
        messages.show("MEWTWO's power is rising!");
    }

    if(_barrier_frames)
    {
        --_barrier_frames;
    }

    bn::fixed_point aim = directions::toward(_position, target);
    bn::fixed_point delta = target - _position;

    if(bn::abs(delta.x()) + bn::abs(delta.y()) > keep_distance)
    {
        static_cast<void>(walk(aim * float_speed, true));
    }

    if(--_fan_timer <= 0)
    {
        attacks::shoot(projectiles, _confusion, _position, aim, shot_speed_scale);
        _fan_timer = _scaled(fan_interval) + random.get_int(30);
    }

    if(--_wave_timer <= 0)
    {
        messages.show("MEWTWO used PSYCHIC!");

        for(int index = 0; index < wave_shots; ++index)
        {
            attacks::shoot(projectiles, _psywave, _position, attacks::rotate(aim, index * 360 / wave_shots),
                           shot_speed_scale);
        }

        _wave_timer = _scaled(wave_interval) + random.get_int(60);
    }

    if(_rising && --_swift_timer <= 0)
    {
        attacks::shoot(projectiles, _swift, _position, aim, shot_speed_scale);
        _swift_timer = _scaled(swift_interval) + random.get_int(40);
    }

    if(--_barrier_timer <= 0)
    {
        messages.show("MEWTWO used BARRIER!");
        _barrier_frames = barrier_frames;
        _barrier_timer = _scaled(barrier_interval) + random.get_int(60);
    }

    if(--_teleport_timer <= 0)
    {
        int degrees = random.get_int(8) * 45;
        bn::fixed_point goal = target + bn::fixed_point(bn::degrees_lut_cos(degrees) * teleport_distance,
                                                        bn::degrees_lut_sin(degrees) * teleport_distance / 2);

        if(! room::area_is_blocked(goal.x() - 10, goal.y() + 6, goal.x() + 10, goal.y() + 14, true, true))
        {
            _position = goal;
            _flash_frames = 6;
        }

        _teleport_timer = _scaled(teleport_interval) + random.get_int(60);
    }

    _update_sprite();
}

int mewtwo_boss::_scaled(int frames) const
{
    return _rising ? frames * 7 / 10 : frames;
}

void mewtwo_boss::_update_sprite()
{
    int frame = species_frames::walk + (_frame_counter / 16) % 2;

    if(_flash_frames)
    {
        frame = species_frames::white;
    }
    else if(_barrier_frames)
    {
        frame = (_barrier_frames / 3) % 2 ? species_frames::charging : species_frames::white;
    }
    else if(_rising && (_frame_counter / 8) % 4 == 0)
    {
        frame = species_frames::charging;
    }

    bool visible = ! _intro_frames || (_intro_frames / 3) % 2;
    _sprite.set_tiles(bn::sprite_items::mewtwo.tiles_item(), frame);
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer() - 8);
    _sprite.set_visible(visible);
}
