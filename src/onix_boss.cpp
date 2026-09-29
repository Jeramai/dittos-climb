#include "onix_boss.h"

#include "bn_math.h"

#include "bn_sprite_items_onix.h"
#include "bn_sprite_items_onix_segment.h"
#include "bn_sprite_items_projectiles.h"

#include "attacks.h"
#include "directions.h"
#include "projectile_frames.h"

namespace
{
    constexpr int boss_hp = 130;
    constexpr int contact_power = 30;
    constexpr int emerging_frames = 50;
    constexpr int windup_frames = 30;
    constexpr int charge_frames = 50;
    constexpr int stunned_frames = 80;
    constexpr int charge_interval = 240;
    constexpr int throw_interval = 110;
    constexpr int slide_interval = 300;
    constexpr int slide_warning_frames = 45;
    constexpr int slide_rock_life = 8;
    constexpr int slide_rock_half_size = 9;
    constexpr int slide_spread = 48;
    constexpr int segment_spacing = 7;
    constexpr int segment_radius = 9;
    constexpr int head_radius = 12;
    constexpr bn::fixed roam_speed = 0.9;
    constexpr bn::fixed charge_speed = 2.6;
    constexpr bn::fixed roam_turn = 2.5;
    constexpr bn::fixed throw_speed_scale = 0.8;

    bn::fixed normalize(bn::fixed degrees)
    {
        while(degrees < 0)
        {
            degrees += 360;
        }

        while(degrees >= 360)
        {
            degrees -= 360;
        }

        return degrees;
    }

    bool near(const bn::fixed_point& a, const bn::fixed_point& b, int radius)
    {
        bn::fixed_point delta = a - b;
        return bn::abs(delta.x()) < radius && bn::abs(delta.y()) < radius;
    }
}

onix_boss::onix_boss(const bn::fixed_point& position, const bn::camera_ptr& camera) :
    boss(species_id::onix, boss_hp, position),
    _head(bn::sprite_items::onix.create_sprite(position, species_frames::walk)),
    _camera(camera),
    _rock_throw(wild_attack(move_id::rock_throw)),
    _rock_slide(wild_attack(move_id::rock_throw)),
    _state_frames(emerging_frames),
    _charge_timer(charge_interval / 2),
    _throw_timer(throw_interval),
    _slide_timer(slide_interval / 2)
{
    _head.set_camera(camera);

    for(bn::fixed_point& point : _history)
    {
        point = position;
    }

    for(int index = 0; index < segment_count; ++index)
    {
        bn::sprite_ptr segment = bn::sprite_items::onix_segment.create_sprite(position);
        segment.set_camera(camera);
        _segments.push_back(bn::move(segment));
    }

    _update_sprites();
}

void onix_boss::update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                       message_box& messages)
{
    if(_flash_frames)
    {
        --_flash_frames;
    }

    if(_slide_frames && ! --_slide_frames)
    {
        _drop_slide(projectiles);
    }

    switch(_state)
    {

    case state::emerging:
        if(! --_state_frames)
        {
            _state = state::roaming;
        }
        break;

    case state::roaming:
        _turn_toward(target, roam_turn);

        if(! walk(_direction() * roam_speed))
        {
            _heading = normalize(_heading + 90);
        }

        _record_history();

        if(--_throw_timer <= 0)
        {
            attacks::shoot(projectiles, _rock_throw, _position, directions::toward(_position, target),
                           throw_speed_scale);
            _throw_timer = throw_interval + random.get_int(40);
        }

        if(--_slide_timer <= 0)
        {
            messages.show("ONIX used ROCK SLIDE!");
            _start_slide(target, random);
            _slide_timer = slide_interval + random.get_int(60);
        }

        if(--_charge_timer <= 0)
        {
            _state = state::windup;
            _state_frames = windup_frames;
        }
        break;

    case state::windup:
        _turn_toward(target, 360);

        if(! --_state_frames)
        {
            messages.show("ONIX used SLAM!");
            _state = state::charging;
            _state_frames = charge_frames;
        }
        break;

    case state::charging:
        if(! walk(_direction() * charge_speed))
        {
            messages.show("ONIX crashed into the wall!");
            _state = state::stunned;
            _state_frames = stunned_frames;
            break;
        }

        _record_history();
        _record_history();

        if(! --_state_frames)
        {
            _state = state::roaming;
            _charge_timer = charge_interval + random.get_int(60);
        }
        break;

    case state::stunned:
        if(! --_state_frames)
        {
            _heading = normalize(_heading + 180);
            _state = state::roaming;
            _charge_timer = charge_interval + random.get_int(60);
        }
        break;

    default:
        break;
    }

    _update_sprites();
}

attack onix_boss::contact_attack() const
{
    return attack{ move_id::slam, pokemon_type::normal, contact_power };
}

bool onix_boss::touches(const bn::fixed_point& point) const
{
    if(_state == state::emerging)
    {
        return false;
    }

    if(near(point, _position, head_radius))
    {
        return true;
    }

    for(int index = 0; index < segment_count; ++index)
    {
        if(near(point, _segment_position(index), segment_radius))
        {
            return true;
        }
    }

    return false;
}

bool onix_boss::blocks(const bn::fixed_point& point, int half_size) const
{
    for(int index = 0; index < segment_count; ++index)
    {
        if(near(point, _segment_position(index), segment_radius + half_size))
        {
            return true;
        }
    }

    return false;
}

bn::fixed_point onix_boss::_direction() const
{
    return bn::fixed_point(bn::degrees_lut_cos(_heading), -bn::degrees_lut_sin(_heading));
}

void onix_boss::_turn_toward(const bn::fixed_point& target, bn::fixed max_degrees)
{
    bn::fixed desired = attacks::screen_degrees(directions::toward(_position, target));
    bn::fixed difference = normalize(desired - _heading);

    if(difference > 180)
    {
        difference -= 360;
    }

    _heading = normalize(_heading + bn::clamp(difference, -max_degrees, max_degrees));
}

void onix_boss::_record_history()
{
    _history_index = (_history_index + 1) % history_size;
    _history[_history_index] = _position;
}

const bn::fixed_point& onix_boss::_segment_position(int index) const
{
    int offset = (index + 1) * segment_spacing;
    return _history[(_history_index - offset + history_size * 2) % history_size];
}

void onix_boss::_start_slide(const bn::fixed_point& target, bn::random& random)
{
    _slide_markers.clear();

    for(int index = 0; index < _slide_markers.max_size(); ++index)
    {
        bn::fixed_point offset(random.get_int(slide_spread * 2) - slide_spread,
                               random.get_int(slide_spread * 2) - slide_spread);
        bn::fixed_point position = index == 0 ? target : target + offset;
        bn::sprite_ptr marker = bn::sprite_items::projectiles.create_sprite(position, projectile_frames::impact);
        marker.set_camera(_camera);
        marker.set_z_order(-900);
        _slide_markers.push_back(bn::move(marker));
    }

    _slide_frames = slide_warning_frames;
}

void onix_boss::_drop_slide(enemy_projectiles& projectiles)
{
    for(bn::sprite_ptr& marker : _slide_markers)
    {
        bn::fixed_point position = marker.position();
        projectiles.spawn(bn::sprite_items::projectiles.create_sprite(position, projectile_frames::rock), position,
                          bn::fixed_point(), _rock_slide, slide_rock_life, slide_rock_half_size, false);
    }

    _slide_markers.clear();
}

void onix_boss::_update_sprites()
{
    bool flash = _flash_frames || (_state == state::windup && (_state_frames / 3) % 2);
    int head_frame = flash ? species_frames::white :
                     _state == state::charging ? species_frames::charging : species_frames::walk;
    bool visible = _state != state::emerging || (_state_frames / 3) % 2;

    _head.set_tiles(bn::sprite_items::onix.tiles_item(), head_frame);
    _head.set_position(_position);
    _head.set_z_order(-_position.y().round_integer() - 8);
    _head.set_visible(visible);

    for(int index = 0; index < segment_count; ++index)
    {
        const bn::fixed_point& position = _segment_position(index);
        bn::sprite_ptr& segment = _segments[index];
        segment.set_tiles(bn::sprite_items::onix_segment.tiles_item(), flash ? 1 : 0);
        segment.set_position(position);
        segment.set_z_order(-position.y().round_integer());
        segment.set_visible(visible);
    }

    for(bn::sprite_ptr& marker : _slide_markers)
    {
        marker.set_visible((_slide_frames / 3) % 2 == 0);
    }
}
