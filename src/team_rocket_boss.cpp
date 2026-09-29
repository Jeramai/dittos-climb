#include "team_rocket_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_arbok.h"
#include "bn_sprite_items_balloon.h"
#include "bn_sprite_items_weezing.h"

#include "attacks.h"
#include "directions.h"
#include "room.h"

namespace
{
    constexpr int boss_hp = 220;
    constexpr int intro_frames = 90;
    constexpr int sting_interval = 100;
    constexpr int wrap_interval = 220;
    constexpr int wrap_windup_frames = 24;
    constexpr int wrap_frames = 16;
    constexpr int sludge_interval = 90;
    constexpr int smog_interval = 170;
    constexpr int coin_interval = 110;
    constexpr int weezing_orbit = 64;
    constexpr int balloon_height = 70;
    constexpr int balloon_swing = 90;
    constexpr int body_radius = 12;
    constexpr bn::fixed arbok_speed = 0.6;
    constexpr bn::fixed wrap_speed = 3.5;
    constexpr bn::fixed weezing_speed = 0.8;
    constexpr bn::fixed shot_speed_scale = 0.8;

    bool near(const bn::fixed_point& a, const bn::fixed_point& b, int radius)
    {
        bn::fixed_point delta = a - b;
        return bn::abs(delta.x()) < radius && bn::abs(delta.y()) < radius;
    }
}

team_rocket_boss::team_rocket_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::arbok, boss_hp, position),
    _arbok(bn::sprite_items::arbok.create_sprite(position, species_frames::walk)),
    _weezing(bn::sprite_items::weezing.create_sprite(position + bn::fixed_point(40, 0), species_frames::walk)),
    _balloon(bn::sprite_items::balloon.create_sprite(position - bn::fixed_point(0, balloon_height))),
    _weezing_position(position + bn::fixed_point(40, 0)),
    _balloon_anchor(position - bn::fixed_point(0, balloon_height)),
    _poison_sting(wild_attack(move_id::poison_sting)),
    _wrap(wild_attack(move_id::wrap)),
    _sludge(wild_attack(move_id::sludge)),
    _smog(wild_attack(move_id::smog)),
    _pay_day(wild_attack(move_id::pay_day)),
    _intro_frames(intro_frames),
    _sting_timer(sting_interval),
    _wrap_timer(wrap_interval),
    _sludge_timer(sludge_interval / 2),
    _smog_timer(smog_interval),
    _coin_timer(coin_interval)
{
    _arbok.set_camera(camera);
    _weezing.set_camera(camera);
    _balloon.set_camera(camera);
    _balloon.set_z_order(-2000);
    _update_sprites();
}

void team_rocket_boss::announce_defeat(message_box& messages) const
{
    messages.show("TEAM ROCKET is blasting off again!");
}

void team_rocket_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
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
        _update_sprites();
        return;
    }

    if(! _desperate && _hp * 10 < _max_hp * 4)
    {
        _desperate = true;
        messages.show("JAMES: We're not done yet!");
    }

    if(_windup_frames)
    {
        if(! --_windup_frames)
        {
            messages.show("ARBOK used WRAP!");
            _wrap_frames = wrap_frames;
        }
    }
    else if(_wrap_frames)
    {
        if(! walk(_wrap_direction * wrap_speed) || ! --_wrap_frames)
        {
            _wrap_frames = 0;
            _wrap_timer = _scaled(wrap_interval) + random.get_int(60);
        }
    }
    else
    {
        static_cast<void>(walk(directions::toward(_position, target) * arbok_speed));

        if(--_sting_timer <= 0)
        {
            bn::fixed_point aim = directions::toward(_position, target);

            for(int offset : { -15, 0, 15 })
            {
                attacks::shoot(projectiles, _poison_sting, _position, attacks::rotate(aim, offset), shot_speed_scale);
            }

            _sting_timer = _scaled(sting_interval) + random.get_int(30);
        }

        if(--_wrap_timer <= 0)
        {
            _windup_frames = wrap_windup_frames;
            _wrap_direction = directions::toward(_position, target);
        }
    }

    _move_weezing(target);
    bn::fixed_point weezing_aim = directions::toward(_weezing_position, target);

    if(--_sludge_timer <= 0)
    {
        attacks::shoot(projectiles, _sludge, _weezing_position, weezing_aim, shot_speed_scale);
        _sludge_timer = _scaled(sludge_interval) + random.get_int(30);
    }

    if(--_smog_timer <= 0)
    {
        messages.show("WEEZING used SMOG!");
        attacks::cloud(projectiles, _smog, _weezing_position, weezing_aim, 1);
        _smog_timer = _scaled(smog_interval) + random.get_int(40);
    }

    if(--_coin_timer <= 0)
    {
        bn::fixed_point balloon = _balloon.position();
        attacks::shoot(projectiles, _pay_day, balloon, directions::toward(balloon, target), shot_speed_scale);
        _coin_timer = _scaled(coin_interval) + random.get_int(40);
    }

    _update_sprites();
}

bool team_rocket_boss::contains(const bn::fixed_point& point, int half_size) const
{
    return near(point, _position, body_radius + half_size) || near(point, _weezing_position, body_radius + half_size);
}

bool team_rocket_boss::touches(const bn::fixed_point& point) const
{
    return _wrap_frames && near(point, _position, body_radius + 4);
}

int team_rocket_boss::_scaled(int frames) const
{
    return _desperate ? frames * 7 / 10 : frames;
}

void team_rocket_boss::_move_weezing(const bn::fixed_point& target)
{
    int degrees = (_frame_counter * 2) % 360;
    bn::fixed_point goal = target + bn::fixed_point(bn::degrees_lut_cos(degrees) * weezing_orbit,
                                                    bn::degrees_lut_sin(degrees) * weezing_orbit / 2);
    bn::fixed_point step = directions::toward(_weezing_position, goal) * weezing_speed;
    bn::fixed_point next = _weezing_position + step;

    if(! room::area_is_blocked(next.x() - 10, next.y() + 6, next.x() + 10, next.y() + 14, true, true))
    {
        _weezing_position = next;
    }
}

void team_rocket_boss::_update_sprites()
{
    bool flash = _flash_frames || (_windup_frames && (_windup_frames / 3) % 2);
    int arbok_frame = flash ? species_frames::white : species_frames::walk + (_frame_counter / 14) % 2;
    int weezing_frame = _flash_frames ? species_frames::white : species_frames::walk + (_frame_counter / 20) % 2;
    bool visible = ! _intro_frames || (_intro_frames / 3) % 2;

    _arbok.set_tiles(bn::sprite_items::arbok.tiles_item(), arbok_frame);
    _arbok.set_position(_position);
    _arbok.set_z_order(-_position.y().round_integer() - 8);
    _arbok.set_visible(visible);

    _weezing.set_tiles(bn::sprite_items::weezing.tiles_item(), weezing_frame);
    _weezing.set_position(_weezing_position);
    _weezing.set_z_order(-_weezing_position.y().round_integer() - 8);
    _weezing.set_visible(visible);

    int degrees = (_frame_counter * 1) % 360;
    _balloon.set_position(_balloon_anchor + bn::fixed_point(bn::degrees_lut_sin(degrees) * balloon_swing,
                                                            bn::degrees_lut_sin((degrees * 2) % 360) * 6));
}
