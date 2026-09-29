#include "gengar_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_gengar.h"

#include "attacks.h"
#include "directions.h"
#include "room.h"

namespace
{
    constexpr int boss_hp = 230;
    constexpr int intro_frames = 50;
    constexpr int fade_frames = 20;
    constexpr int hidden_frames = 30;
    constexpr int ball_interval = 110;
    constexpr int fade_interval = 240;
    constexpr int hypnosis_interval = 300;
    constexpr int dream_delay = 40;
    constexpr int keep_distance = 50;
    constexpr int reappear_distance = 22;
    constexpr bn::fixed walk_speed = 0.8;
    constexpr bn::fixed shot_speed_scale = 0.8;
}

gengar_boss::gengar_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::gengar, boss_hp, position),
    _sprite(bn::sprite_items::gengar.create_sprite(position, species_frames::walk)),
    _lick(wild_attack(move_id::lick)),
    _shadow_ball(wild_attack(move_id::shadow_ball)),
    _hypnosis(wild_attack(move_id::hypnosis)),
    _dream_eater(wild_attack(move_id::dream_eater)),
    _state_frames(intro_frames),
    _ball_timer(ball_interval),
    _fade_timer(fade_interval / 2),
    _hypnosis_timer(hypnosis_interval)
{
    _sprite.set_camera(camera);
    _update_sprite();
}

void gengar_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                         message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    ++_walk_frames;
    bn::fixed_point aim = directions::toward(_position, target);

    if(_dream_frames && ! --_dream_frames)
    {
        messages.show("GENGAR used DREAM EATER!");
        attacks::shoot(projectiles, _dream_eater, _position, aim, shot_speed_scale);
    }

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
        if(! _laughing && _hp * 10 < _max_hp * 4)
        {
            _laughing = true;
            messages.show("GENGAR is laughing...");
        }

        bn::fixed_point delta = target - _position;

        if(bn::abs(delta.x()) + bn::abs(delta.y()) > keep_distance)
        {
            static_cast<void>(walk(aim * walk_speed, true));
        }

        if(--_ball_timer <= 0)
        {
            for(int offset : { -12, 0, 12 })
            {
                attacks::shoot(projectiles, _shadow_ball, _position, attacks::rotate(aim, offset), shot_speed_scale);
            }

            _ball_timer = _scaled(ball_interval) + random.get_int(30);
        }

        if(--_hypnosis_timer <= 0)
        {
            messages.show("GENGAR used HYPNOSIS!");
            attacks::shoot(projectiles, _hypnosis, _position, aim, shot_speed_scale);
            _dream_frames = dream_delay;
            _hypnosis_timer = _scaled(hypnosis_interval) + random.get_int(60);
        }

        if(--_fade_timer <= 0)
        {
            messages.show("GENGAR vanished!");
            _state = state::fading_out;
            _state_frames = fade_frames;
        }
        break;
    }

    case state::fading_out:
        if(! --_state_frames)
        {
            _state = state::faded;
            _state_frames = hidden_frames;
        }
        break;

    case state::faded:
        if(! --_state_frames)
        {
            int degrees = random.get_int(8) * 45;
            bn::fixed_point offset(bn::degrees_lut_cos(degrees) * reappear_distance,
                                   bn::degrees_lut_sin(degrees) * reappear_distance);
            bn::fixed_point spot = target + offset;

            if(! room::area_is_blocked(spot.x() - 10, spot.y() + 6, spot.x() + 10, spot.y() + 14, true, true))
            {
                _position = spot;
            }

            _state = state::fading_in;
            _state_frames = fade_frames;
        }
        break;

    case state::fading_in:
        if(! --_state_frames)
        {
            messages.show("GENGAR used LICK!");
            attacks::slash(projectiles, _lick, _position, directions::toward(_position, target));
            _state = state::roaming;
            _fade_timer = _scaled(fade_interval) + random.get_int(60);
        }
        break;

    default:
        break;
    }

    _update_sprite();
}

int gengar_boss::_scaled(int frames) const
{
    return _laughing ? frames * 7 / 10 : frames;
}

void gengar_boss::_update_sprite()
{
    int frame = species_frames::walk + (_walk_frames / 14) % 2;

    if(_flash_frames)
    {
        frame = species_frames::white;
    }
    else if(_state == state::fading_in || _laughing)
    {
        frame = (_walk_frames / 6) % 3 ? frame : species_frames::charging;
    }

    bool visible = true;

    if(_state == state::intro || _state == state::fading_out || _state == state::fading_in)
    {
        visible = (_state_frames / 2) % 2;
    }
    else if(_state == state::faded)
    {
        visible = false;
    }

    if(! _revealed && ! _flash_frames && _state != state::fading_in)
    {
        visible = visible && (_walk_frames / 4) % 4 == 0;
    }

    _sprite.set_tiles(bn::sprite_items::gengar.tiles_item(), frame);
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer() - 8);
    _sprite.set_visible(visible);
}
