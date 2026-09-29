#include "game.h"

#include "bn_bg_palettes.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_optional.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"

#include "bn_regular_bg_items_room.h"
#include "bn_sprite_items_projectiles.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "projectile_frames.h"

namespace
{
    constexpr int wave_delay_frames = 120;
    constexpr int shake_frames = 10;
    constexpr int effect_frames = 6;
    constexpr int outline_frames = 300;
    constexpr int outline_blink_frames = 90;

    bool within(const bn::fixed_point& a, const bn::fixed_point& b, int half_width, int half_height)
    {
        bn::fixed_point delta = a - b;
        return bn::abs(delta.x()) < half_width && bn::abs(delta.y()) < half_height;
    }

    void append_name_message(message_box& messages, const char* prefix, const char* name, const char* suffix)
    {
        message_box::text message(prefix);
        message.append(name);
        message.append(suffix);
        messages.show(message);
    }
}

game::game(bn::random& random) :
    _random(random),
    _camera(bn::camera_ptr::create(0, 0)),
    _room_bg(bn::regular_bg_items::room.create_bg(0, 0)),
    _player(_camera, bn::fixed_point(0, 64)),
    _player_projectiles(_camera),
    _enemy_projectiles(_camera),
    _camera_position(room::clamp_camera(_player.position()))
{
    bn::bg_palettes::set_fade_intensity(0);
    _room_bg.set_camera(_camera);
    _camera.set_position(_camera_position);
    _messages.show("DITTO woke up in the lab basement.");
}

void game::run()
{
    while(! _player.fainted())
    {
        if(bn::keypad::start_pressed())
        {
            _pause();
        }

        _update_play();
        _random.update();
        bn::core::update();
    }

    _game_over();
}

void game::_update_play()
{
    int outline_index = _outline_below_player();
    const species_id* outline_species = outline_index >= 0 ? &_outlines[outline_index].species : nullptr;

    if(outline_species && ! _player.transforming() && ! _player.active_form())
    {
        _messages.show("Press B to TRANSFORM!");
    }

    if(_player.update(_player_projectiles, _messages, outline_species))
    {
        _outlines.erase(_outlines.begin() + outline_index);
    }

    for(enemy& value : _enemies)
    {
        value.update(_player.position(), _enemy_projectiles, _random);
    }

    _player_projectiles.update([this](const bn::fixed_point& position) { _spawn_effect(position); });
    _enemy_projectiles.update([this](const bn::fixed_point& position) { _spawn_effect(position); });

    _handle_player_attacks();
    _handle_enemy_attacks();
    _update_outlines();

    if(_enemies.empty() && --_wave_delay <= 0)
    {
        ++_wave;
        _spawn_wave();
        _wave_delay = wave_delay_frames;
    }

    _update_effects();
    _update_camera();
    _messages.update();
    _hud.update(_player);
}

void game::_handle_player_attacks()
{
    bool recoil = false;

    _player_projectiles.remove_if([this, &recoil](const projectile& shot)
    {
        for(enemy& value : _enemies)
        {
            if(value.active() && value.contains(shot.position, shot.half_size))
            {
                hit_result result = value.take_hit(shot.hit);
                _messages.show(combat::effectiveness_message(result.effectiveness));
                _spawn_effect(shot.position);
                recoil = recoil || shot.hit.move == move_id::struggle;
                return true;
            }
        }

        return false;
    });

    if(_player.area_active())
    {
        int serial = _player.area_serial();

        for(enemy& value : _enemies)
        {
            if(value.active() && value.contains(_player.position(), _player.area_half_size()) &&
               value.hit_by_area(serial))
            {
                hit_result result = value.take_hit(_player.area_attack());
                _messages.show(combat::effectiveness_message(result.effectiveness));
                _spawn_effect(value.position());

                if(_player.area_attack().move == move_id::struggle && serial != _last_recoil_serial)
                {
                    _last_recoil_serial = serial;
                    recoil = true;
                }
            }
        }
    }

    if(recoil)
    {
        _player.recoil(_messages);
    }

    bool had_enemies = ! _enemies.empty();

    bn::erase_if(_enemies, [this](const enemy& value)
    {
        if(! value.dead())
        {
            return false;
        }

        append_name_message(_messages, "Wild ", value.data().name, " fainted!");
        _spawn_effect(value.position());

        if(! _outlines.full())
        {
            bn::sprite_ptr sprite = value.data().sprite->create_sprite(value.position(), species_frames::white);
            sprite.set_camera(_camera);
            sprite.set_z_order(500);
            _outlines.push_back(outline{ bn::move(sprite), value.id(), outline_frames });
        }

        return true;
    });

    if(had_enemies && _enemies.empty())
    {
        ++_waves_cleared;
    }
}

void game::_handle_enemy_attacks()
{
    if(! _player.vulnerable())
    {
        return;
    }

    bn::fixed_point hurt_center = _player.position() + bn::fixed_point(0, 2);
    bn::optional<attack> hit;

    _enemy_projectiles.remove_if([&hit, &hurt_center](const projectile& shot)
    {
        if(! hit && within(shot.position, hurt_center, shot.half_size, shot.half_size + 1))
        {
            hit = shot.hit;
            return true;
        }

        return false;
    });

    for(enemy& value : _enemies)
    {
        if(! hit && value.dash_hits(_player.position()))
        {
            hit = value.dash_attack();
        }
    }

    if(hit)
    {
        _player.take_hit(*hit, _messages);
        _shake_frames = shake_frames;
    }
}

int game::_outline_below_player() const
{
    for(int index = 0; index < _outlines.size(); ++index)
    {
        if(within(_outlines[index].sprite.position(), _player.position(), 10, 10))
        {
            return index;
        }
    }

    return -1;
}

void game::_update_outlines()
{
    bn::erase_if(_outlines, [](outline& value)
    {
        if(--value.frames <= 0)
        {
            return true;
        }

        value.sprite.set_visible(value.frames > outline_blink_frames || (value.frames / 4) % 2);
        return false;
    });
}

void game::_spawn_wave()
{
    int count = bn::min(1 + _wave, _enemies.max_size());
    int meowth_chance = _wave == 1 ? 0 : bn::min(20 + _wave * 8, 60);
    species_id first = species_id::ditto;

    for(int index = 0; index < count; ++index)
    {
        for(int attempt = 0; attempt < 30; ++attempt)
        {
            bn::fixed_point position(_random.get_int(480) - 240, _random.get_int(208) - 104);
            bn::fixed_point delta = position - _player.position();

            if(room::feet_are_blocked(position) || bn::abs(delta.x()) + bn::abs(delta.y()) < 100)
            {
                continue;
            }

            species_id id = _random.get_int(100) < meowth_chance ? species_id::meowth : species_id::rattata;
            _enemies.emplace_back(id, position, _camera, _random);

            if(first == species_id::ditto)
            {
                first = id;
            }

            break;
        }
    }

    if(_enemies.size() == 1)
    {
        append_name_message(_messages, "A wild ", species::get(first).name, " appeared!");
    }
    else
    {
        _messages.show("Wild POKEMON appeared!");
    }
}

void game::_spawn_effect(const bn::fixed_point& position)
{
    if(_effects.full())
    {
        return;
    }

    bn::sprite_ptr sprite = bn::sprite_items::projectiles.create_sprite(position, projectile_frames::impact);
    sprite.set_camera(_camera);
    sprite.set_z_order(-1001);
    _effects.push_back(effect{ bn::move(sprite), effect_frames });
}

void game::_update_effects()
{
    bn::erase_if(_effects, [](effect& value)
    {
        return --value.frames <= 0;
    });
}

void game::_update_camera()
{
    bn::fixed_point target = room::clamp_camera(_player.position());
    _camera_position += (target - _camera_position) / 4;

    bn::fixed_point shake;

    if(_shake_frames)
    {
        --_shake_frames;
        shake = bn::fixed_point(_random.get_int(5) - 2, _random.get_int(5) - 2);
    }

    _camera.set_position(_camera_position + shake);
}

void game::_pause()
{
    bn::sprite_text_generator generator(common::variable_8x16_sprite_font);
    generator.set_center_alignment();
    generator.set_bg_priority(0);

    bn::vector<bn::sprite_ptr, 4> text;
    generator.generate(0, 0, "PAUSED", text);
    bn::core::update();

    while(! bn::keypad::start_pressed())
    {
        bn::core::update();
    }
}

void game::_game_over()
{
    _player_projectiles.clear();
    _enemy_projectiles.clear();
    _effects.clear();
    _enemies.clear();
    _outlines.clear();
    _messages.clear();
    _hud.set_visible(false);
    _player.set_visible(false);
    bn::bg_palettes::set_fade(bn::color(0, 0, 0), 0.6);

    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::string<24> cleared("WAVES CLEARED: ");
    cleared.append(bn::to_string<4>(_waves_cleared));

    bn::vector<bn::sprite_ptr, 20> text;
    big.generate(0, -24, "DITTO blacked out!", text);
    small.generate(0, 4, cleared, text);
    small.generate(0, 24, "PRESS START", text);

    while(! bn::keypad::start_pressed())
    {
        bn::core::update();
    }
}
