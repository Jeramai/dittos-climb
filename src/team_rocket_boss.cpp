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
    constexpr int pokemon_hp = 110;
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
    boss(species_id::arbok, pokemon_hp * 2, position),
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
    _coin_timer(coin_interval),
    _arbok_hp(pokemon_hp),
    _weezing_hp(pokemon_hp)
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

void team_rocket_boss::set_test_hp(int hp)
{
    _arbok_hp = bn::max(hp / 2, 1);
    _weezing_hp = bn::max(hp - _arbok_hp, 1);
    _max_hp = _arbok_hp + _weezing_hp;
    _hp = _max_hp;
}

hit_result team_rocket_boss::take_hit(const attack& hit, const bn::fixed_point& point, int half_size)
{
    if(! vulnerable())
    {
        return hit_result{ 0, types::neutral };
    }

    bool weezing = _arbok_hp <= 0 ||
                   (_weezing_hp > 0 && ! near(point, _position, body_radius + half_size) &&
                    near(point, _weezing_position, body_radius + half_size));
    const species_data& data = species::get(weezing ? species_id::weezing : species_id::arbok);
    hit_result result = combat::resolve(hit, data.type_1, data.type_2);

    if(weezing)
    {
        _weezing_hp = bn::max(_weezing_hp - result.damage, 0);
        _weezing_flash = 4;
        _last_hit_position = _weezing_position;
    }
    else
    {
        _arbok_hp = bn::max(_arbok_hp - result.damage, 0);
        _arbok_flash = 4;
        _last_hit_position = _position;
    }

    _hp = _arbok_hp + _weezing_hp;
    return result;
}

void team_rocket_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                              message_box& messages)
{
    ++_frame_counter;

    if(_arbok_flash)
    {
        --_arbok_flash;
    }

    if(_weezing_flash)
    {
        --_weezing_flash;
    }

    if(_intro_frames)
    {
        --_intro_frames;
        _update_sprites();
        return;
    }

    if(! _arbok_fainted && _arbok_hp <= 0)
    {
        _arbok_fainted = true;
        _windup_frames = 0;
        _wrap_frames = 0;
        messages.show("ARBOK fainted!");
    }

    if(! _weezing_fainted && _weezing_hp <= 0)
    {
        _weezing_fainted = true;
        messages.show("WEEZING fainted!");
    }

    if(! _desperate && (_arbok_fainted || _weezing_fainted) && _hp > 0)
    {
        _desperate = true;
        messages.show("JAMES: We're not done yet!");
    }

    if(! _arbok_fainted)
    {
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
                    attacks::shoot(projectiles, _poison_sting, _position, attacks::rotate(aim, offset),
                                   shot_speed_scale);
                }

                _sting_timer = _scaled(sting_interval) + random.get_int(30);
            }

            if(--_wrap_timer <= 0)
            {
                _windup_frames = wrap_windup_frames;
                _wrap_direction = directions::toward(_position, target);
            }
        }
    }

    if(! _weezing_fainted)
    {
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
    return (_arbok_hp > 0 && near(point, _position, body_radius + half_size)) ||
           (_weezing_hp > 0 && near(point, _weezing_position, body_radius + half_size));
}

bool team_rocket_boss::touches(const bn::fixed_point& point) const
{
    return _wrap_frames && _arbok_hp > 0 && near(point, _position, body_radius + 4);
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
    bool flash = _arbok_flash || (_windup_frames && (_windup_frames / 3) % 2);
    int arbok_frame = flash ? species_frames::white : species_frames::walk + (_frame_counter / 14) % 2;
    int weezing_frame = _weezing_flash ? species_frames::white : species_frames::walk + (_frame_counter / 20) % 2;
    bool visible = ! _intro_frames || (_intro_frames / 3) % 2 == 0;

    _arbok.set_tiles(bn::sprite_items::arbok.tiles_item(), arbok_frame);
    _arbok.set_position(_position);
    _arbok.set_z_order(-_position.y().round_integer() - 8);
    _arbok.set_visible(visible && ! _arbok_fainted);

    _weezing.set_tiles(bn::sprite_items::weezing.tiles_item(), weezing_frame);
    _weezing.set_position(_weezing_position);
    _weezing.set_z_order(-_weezing_position.y().round_integer() - 8);
    _weezing.set_visible(visible && ! _weezing_fainted);

    int degrees = (_frame_counter * 1) % 360;
    _balloon.set_position(_balloon_anchor + bn::fixed_point(bn::degrees_lut_sin(degrees) * balloon_swing,
                                                            bn::degrees_lut_sin((degrees * 2) % 360) * 6));
}
