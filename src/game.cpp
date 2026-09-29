#include "game.h"

#include "bn_bg_palettes.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"

#include "bn_sprite_items_poke_flute.h"
#include "bn_sprite_items_projectiles.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "projectile_frames.h"

namespace
{
    constexpr int spawn_delay_frames = 30;
    constexpr int shake_frames = 10;
    constexpr int effect_frames = 6;
    constexpr int outline_frames = 300;
    constexpr int outline_blink_frames = 90;
    constexpr int fade_frames = 8;
    constexpr int min_spawn_distance = 72;
    constexpr int boss_floor = 1;
    constexpr int boss_talk_distance = 36;

    constexpr const char* floor_names[] = {
        "CINNABAR LAB", "VIRIDIAN FOREST", "ROCK TUNNEL", "UNDERGROUND LAKE", "POWER PLANT", "VOLCANO",
        "SEAFOAM CAVE", "THE CHASM", "ROCKET HIDEOUT", "FIGHTING DOJO", "POKEMON TOWER", "DRAGON'S DEN",
        "CERULEAN CAVE",
    };

    constexpr int floor_name_count = sizeof(floor_names) / sizeof(floor_names[0]);

    bool within(const bn::fixed_point& a, const bn::fixed_point& b, int half_width, int half_height)
    {
        bn::fixed_point delta = a - b;
        return bn::abs(delta.x()) < half_width && bn::abs(delta.y()) < half_height;
    }

    void show_name_message(message_box& messages, const char* prefix, const char* name, const char* suffix)
    {
        message_box::text message(prefix);
        message.append(name);
        message.append(suffix);
        messages.show(message);
    }

    bn::string<32> floor_label(int floor_number)
    {
        bn::string<32> label = bn::to_string<4>(floor_number);
        label.append("F  ");
        label.append(floor_number <= floor_name_count ? floor_names[floor_number - 1] : "???");
        return label;
    }

    void set_fade(bn::fixed intensity)
    {
        bn::color black(0, 0, 0);
        bn::bg_palettes::set_fade(black, intensity);
        bn::sprite_palettes::set_fade(black, intensity);
    }
}

game::game(bn::random& random) :
    _random(random),
    _camera(bn::camera_ptr::create(0, 0)),
    _player(_camera, bn::fixed_point()),
    _player_projectiles(_camera),
    _enemy_projectiles(_camera)
{
    set_fade(0);
    _view.set_camera(_camera);
    _start_floor();

    #ifdef DITTO_TEST_FORM
        _player.start_transform(species_id(DITTO_TEST_FORM));
    #endif
}

void game::run()
{
    while(! _player.fainted())
    {
        if(bn::keypad::start_pressed())
        {
            _pause_map();
        }

        _update_play();
        _random.update();
        bn::core::update();
    }

    _game_over();
}

void game::_start_floor()
{
    _floor.generate(_floor_number, _random);
    _has_flute = false;
    _boss_defeated = false;
    _flute_room = -1;

    if(_floor_number == boss_floor)
    {
        bn::vector<int, floor_map::max_rooms> combat_rooms;

        for(int index = 0; index < _floor.size(); ++index)
        {
            if(_floor[index].kind == room_kind::combat)
            {
                combat_rooms.push_back(index);
            }
        }

        if(! combat_rooms.empty())
        {
            _flute_room = combat_rooms[_random.get_int(combat_rooms.size())];
        }
        else
        {
            _has_flute = true;
        }
    }
    #ifdef DITTO_TEST_START_KIND
        for(int index = 0; index < _floor.size(); ++index)
        {
            if(_floor[index].kind == room_kind(DITTO_TEST_START_KIND))
            {
                _has_flute = room_kind(DITTO_TEST_START_KIND) == room_kind::stairs;
                _flute_room = _has_flute ? -1 : index;
                _enter_room(index, bn::nullopt);
                _messages.show(floor_label(_floor_number));
                return;
            }
        }
    #endif

    _enter_room(0, bn::nullopt);
    _messages.show(floor_label(_floor_number));
}

void game::_enter_room(int index, bn::optional<direction> entered_from)
{
    _clear_room_objects();
    _room = index;

    floor_room& value = _floor[index];
    bool doors[4];

    for(int side = 0; side < 4; ++side)
    {
        doors[side] = _floor.neighbor(index, direction(side)) >= 0;
    }

    _locked = value.kind == room_kind::combat && ! value.cleared;
    _view.build(value, doors, _locked);
    _player.set_position(entered_from ? _view.entry_position(*entered_from) : _view.interior_center());
    value.visited = true;

    if(_locked)
    {
        _spawn_delay = spawn_delay_frames;
        _messages.show("The lab doors locked!");
    }

    if(value.kind == room_kind::stairs)
    {
        if(_floor_number == boss_floor && ! _boss_defeated)
        {
            _boss.emplace(_view.interior_center() - bn::fixed_point(0, 12), _camera);
            _messages.show("A SNORLAX sleeps on the stairs!");
        }
        else
        {
            _messages.show("There are stairs going up!");
        }
    }

    _update_flute();

    _update_camera(true);
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
    _update_boss();
    _update_outlines();

    _update_effects();
    _update_camera(false);
    _messages.update();
    _hud.update(_player);
    _update_room_state();
}

void game::_update_room_state()
{
    if(_spawn_delay && ! --_spawn_delay)
    {
        _spawn_enemies();
    }

    if(_locked && ! _spawn_delay && _enemies.empty() && ! _boss)
    {
        _locked = false;
        _floor[_room].cleared = true;
        _view.set_locked(false);
        _messages.show("The doors opened!");
        _update_flute();
    }

    if(_flute_pickup && within(_flute_pickup->position(), _player.position(), 12, 12))
    {
        _flute_pickup.reset();
        _has_flute = true;
        _messages.show("DITTO found the POKE FLUTE!");
    }

    if(_player.transforming())
    {
        return;
    }

    if(bn::optional<direction> side = _view.exit_side(_player.position()))
    {
        _change_room(*side);
        return;
    }

    bn::fixed_point feet = _player.position() + bn::fixed_point(0, 2);

    if(! _boss && room::at(feet.x(), feet.y()) == room::cells::stairs)
    {
        _climb_stairs();
    }
}

void game::_handle_player_attacks()
{
    bool recoil = false;

    _player_projectiles.remove_if([this](const projectile& shot)
    {
        for(enemy& value : _enemies)
        {
            if(value.active() && value.contains(shot.position, shot.half_size))
            {
                hit_result result = value.take_hit(shot.hit);
                _messages.show(combat::effectiveness_message(result.effectiveness));
                _spawn_effect(shot.position);
                return true;
            }
        }

        if(_boss && _boss->contains(shot.position, shot.half_size))
        {
            if(_boss->awake())
            {
                hit_result result = _boss->take_hit(shot.hit);
                _messages.show(combat::effectiveness_message(result.effectiveness));
            }
            else
            {
                _messages.show("SNORLAX is fast asleep...");
            }

            _spawn_effect(shot.position);
            return true;
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

    if(_player.area_active() && _boss && _boss->awake() &&
       _boss->contains(_player.position(), _player.area_half_size()) && _boss->hit_by_area(_player.area_serial()))
    {
        hit_result result = _boss->take_hit(_player.area_attack());
        _messages.show(combat::effectiveness_message(result.effectiveness));
        _spawn_effect(_boss->position());
        int serial = _player.area_serial();

        if(result.damage && _player.area_attack().move == move_id::struggle && serial != _last_recoil_serial)
        {
            _last_recoil_serial = serial;
            recoil = true;
        }
    }

    if(recoil)
    {
        _player.recoil(_messages);
    }

    bn::erase_if(_enemies, [this](const enemy& value)
    {
        if(! value.dead())
        {
            return false;
        }

        show_name_message(_messages, "Wild ", value.data().name, " fainted!");
        _spawn_effect(value.position());
        _spawn_outline(value.id(), value.position());
        return true;
    });
}

void game::_spawn_outline(species_id id, const bn::fixed_point& position)
{
    if(_outlines.full())
    {
        return;
    }

    bn::sprite_ptr sprite = species::get(id).sprite->create_sprite(position, species_frames::white);
    sprite.set_camera(_camera);
    sprite.set_z_order(500);
    _outlines.push_back(outline{ bn::move(sprite), id, outline_frames });
}

void game::_update_boss()
{
    if(! _boss)
    {
        return;
    }

    _boss->update(_player.position(), _enemy_projectiles, _random, _messages);

    if(_boss->asleep())
    {
        if(within(_boss->position(), _player.position(), boss_talk_distance, boss_talk_distance))
        {
            if(! _has_flute)
            {
                _messages.show("SNORLAX is blocking the stairs!");
            }
            else if(bn::keypad::a_pressed())
            {
                _messages.show("DITTO played the POKE FLUTE!");
                _boss->wake(_messages);
                _locked = true;
                _view.set_locked(true);
            }
            else
            {
                _messages.show("Press A to play the POKE FLUTE!");
            }
        }

        return;
    }

    _hud.show_boss("SNORLAX", _boss->hp(), snorlax_boss::max_hp);

    if(_boss->landed_this_frame())
    {
        _shake_frames = shake_frames * 2;

        if(_player.vulnerable() && within(_boss->position() + bn::fixed_point(0, 8), _player.position(),
                                          snorlax_boss::slam_radius, snorlax_boss::slam_radius))
        {
            _player.take_hit(_boss->slam_attack(), _messages);
        }
    }

    if(_boss->dead())
    {
        _messages.show("SNORLAX fainted!");
        _spawn_effect(_boss->position());
        _spawn_outline(species_id::snorlax, _boss->position());
        _boss.reset();
        _boss_defeated = true;
        _hud.hide_boss();
        _enemy_projectiles.clear();
    }
}

void game::_update_flute()
{
    if(_has_flute || _flute_pickup || _room != _flute_room || ! _floor[_room].cleared)
    {
        return;
    }

    bn::sprite_ptr sprite = bn::sprite_items::poke_flute.create_sprite(_view.interior_center());
    sprite.set_camera(_camera);
    sprite.set_z_order(600);
    _flute_pickup = bn::move(sprite);
    _messages.show("Something shiny is on the floor!");
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

void game::_spawn_enemies()
{
    #ifdef DITTO_TEST_NO_ENEMIES
        return;
    #endif

    int count = bn::min(2 + _floor_number / 2 + _random.get_int(2), _enemies.max_size());

    for(int index = 0; index < count; ++index)
    {
        for(int attempt = 0; attempt < 20; ++attempt)
        {
            bn::fixed_point position = _view.random_floor_position(_random);
            bn::fixed_point delta = position - _player.position();

            if(bn::abs(delta.x()) + bn::abs(delta.y()) < min_spawn_distance)
            {
                continue;
            }

            int roll = _random.get_int(100);
            species_id id = roll < 20 ? species_id::porygon : roll < 55 ? species_id::meowth : species_id::rattata;
            _enemies.emplace_back(id, position, _camera, _random);
            break;
        }
    }

    if(_enemies.size() == 1)
    {
        show_name_message(_messages, "A wild ", _enemies[0].data().name, " appeared!");
    }
    else if(! _enemies.empty())
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

void game::_update_camera(bool snap)
{
    bn::fixed_point target = room::clamp_camera(_player.position());

    if(snap)
    {
        _camera_position = target;
    }
    else
    {
        _camera_position += (target - _camera_position) / 4;
    }

    bn::fixed_point shake;

    if(_shake_frames)
    {
        --_shake_frames;
        shake = bn::fixed_point(_random.get_int(5) - 2, _random.get_int(5) - 2);
    }

    _camera.set_position(_camera_position + shake);
}

void game::_change_room(direction side)
{
    int next = _floor.neighbor(_room, side);

    if(next < 0)
    {
        return;
    }

    _fade(true);
    _enter_room(next, directions_of_floor::opposite(side));
    _fade(false);
}

void game::_climb_stairs()
{
    _messages.clear();
    _messages.show("DITTO went up the stairs!");

    for(int frame = 0; frame < 45; ++frame)
    {
        _messages.update();
        bn::core::update();
    }

    _fade(true);
    ++_floor_number;
    _messages.clear();
    _start_floor();
    _fade(false);
}

void game::_fade(bool out)
{
    for(int frame = 1; frame <= fade_frames; ++frame)
    {
        bn::fixed progress = bn::fixed(frame) / fade_frames;
        set_fade(out ? progress : 1 - progress);
        bn::core::update();
    }
}

void game::_clear_room_objects()
{
    _player_projectiles.clear();
    _enemy_projectiles.clear();
    _effects.clear();
    _enemies.clear();
    _outlines.clear();
    _boss.reset();
    _flute_pickup.reset();
    _hud.hide_boss();
    _spawn_delay = 0;
}

void game::_set_world_visible(bool visible)
{
    _hud.set_visible(visible);
    _messages.set_visible(visible);
}

void game::_pause_map()
{
    _set_world_visible(false);
    _overlay.show_map(_floor, _room);

    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::vector<bn::sprite_ptr, 16> text;
    big.generate(0, -62, floor_label(_floor_number), text);
    small.generate(0, 70, "START: RESUME", text);
    bn::core::update();

    while(! bn::keypad::start_pressed())
    {
        bn::core::update();
    }

    text.clear();
    _overlay.hide();
    _set_world_visible(true);
    bn::core::update();
}

void game::_game_over()
{
    _clear_room_objects();
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

    bn::string<24> reached("FLOOR REACHED: ");
    reached.append(bn::to_string<4>(_floor_number));

    bn::vector<bn::sprite_ptr, 20> text;
    big.generate(0, -24, "DITTO blacked out!", text);
    small.generate(0, 4, reached, text);
    small.generate(0, 24, "PRESS START", text);

    while(! bn::keypad::start_pressed())
    {
        bn::core::update();
    }
}
