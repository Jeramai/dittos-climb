#include "game.h"

#include "bn_bg_palettes.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_string.h"
#include "bn_window.h"

#include "bn_sprite_items_electric_projectiles.h"
#include "bn_sprite_items_ditto.h"
#include "bn_sprite_items_light.h"
#include "bn_sprite_items_mew.h"
#include "bn_sprite_items_mewtwo.h"
#include "bn_sprite_items_mart.h"
#include "bn_sprite_items_pickups.h"
#include "bn_sprite_items_poke_flute.h"
#include "bn_sprite_items_projectiles.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "gyarados_boss.h"
#include "journal.h"
#include "mewtwo_boss.h"
#include "articuno_boss.h"
#include "moltres_boss.h"
#include "pidgeot_boss.h"
#include "dragonite_boss.h"
#include "gengar_boss.h"
#include "hitmon_boss.h"
#include "team_rocket_boss.h"
#include "onix_boss.h"
#include "audio.h"
#include "profile.h"
#include "shiny.h"
#include "projectile_frames.h"
#include "snorlax_boss.h"
#include "venusaur_boss.h"
#include "zapdos_boss.h"

namespace
{
    constexpr int spawn_delay_frames = 30;
    constexpr int page_min_frames = 45;
    constexpr int boss_coins = 20;
    constexpr int mewtwo_coins = 50;
    constexpr int defeat_min_frames = 60;
    constexpr int shake_frames = 10;
    constexpr int effect_frames = 6;
    constexpr int outline_frames = 300;
    constexpr int outline_blink_frames = 90;
    constexpr int fade_frames = 8;
    constexpr int min_spawn_distance = 72;
    constexpr int boss_talk_distance = 36;
    constexpr int light_radius = 44;
    constexpr bn::fixed light_scale = 1.5;
    constexpr int plate_cycle = 200;
    constexpr int boss_plate_cycle = 140;
    constexpr int plate_warning = 40;
    constexpr int plate_shock = 30;
    constexpr int plate_power = 50;
    constexpr int gas_power = 20;
    constexpr int plate_paralysis_chance = 25;
    constexpr int explosion_radius = 34;
    constexpr int flicker_frames = 16;
    constexpr int ember_interval = 80;
    constexpr int boss_ember_interval = 45;
    constexpr int ember_warning_frames = 45;
    constexpr int ember_spread = 64;
    constexpr int ember_power = 40;
    constexpr int ember_life = 8;
    constexpr int ember_half_size = 7;
    constexpr int fall_damage = 4;

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

    template<typename Frame>
    void wait_for_a(int min_frames, const Frame& frame)
    {
        bool released = false;

        for(int wait = 0; ! (released && wait >= min_frames && bn::keypad::a_pressed()); ++wait)
        {
            released = released || ! bn::keypad::a_held();
            frame();
            bn::core::update();
        }
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

game::game(bn::random& random, const save_data* saved) :
    _random(random),
    _camera(bn::camera_ptr::create(0, 0)),
    _player(_camera, bn::fixed_point()),
    _player_projectiles(_camera),
    _enemy_projectiles(_camera)
{
    set_fade(0);
    _view.set_camera(_camera);

    #ifdef DITTO_TEST_FLOOR
        _floor_number = DITTO_TEST_FLOOR;
    #endif

    #ifdef DITTO_TEST_SCOPE
        _has_silph_scope = true;
    #endif

    #ifdef DITTO_TEST_PAGES
        _journal_mask = (1 << DITTO_TEST_PAGES) - 1;
    #endif

    #ifdef DITTO_TEST_ENDING
        _journal_mask = (1 << DITTO_TEST_ENDING) - 1;
        _won = true;
    #endif

    if(saved)
    {
        _floor_number = saved->floor_number;
        _journal_mask = saved->journal_mask;
        _run_frames = saved->run_frames;
        _defeated = saved->defeated;
        _shinies = saved->shinies;
        _coins_earned = saved->coins_earned;

        for(int word = 0; word < 3; ++word)
        {
            _run_forms[word] = saved->run_forms[word];
        }
        _has_silph_scope = saved->has_silph_scope;
        _player.restore(saved->player);
    }
    else
    {
        _player.set_shiny_ditto(shiny::roll(_random));
        profile::record_run_start();
    }

    _start_floor(saved);

    if(! saved && _player.shiny_ditto())
    {
        _messages.show("Huh? DITTO is shiny!");
        audio::play(bn::sound_items::sfx_key_item);
    }

    #ifdef DITTO_TEST_FORM
        if(! saved)
        {
            _player.start_transform(species_id(DITTO_TEST_FORM), shiny::roll(_random));
        }
    #endif
}

void game::run()
{
    while(! _player.fainted() && ! _won && ! _quit)
    {
        if(bn::keypad::start_pressed())
        {
            _pause_map();

            if(_quit)
            {
                break;
            }
        }

        _update_play();
        ++_run_frames;
        _random.update();
        bn::core::update();
    }

    if(_won)
    {
        _ending();
    }
    else if(! _quit)
    {
        _game_over();
    }
}

void game::_start_floor(const save_data* saved)
{
    profile::record_floor(_floor_number);
    _previous_room = -1;
    audio::play_floor_music(_floor_number);
    _floor.generate(_floor_number, _theme().overgrown_percent, ! _theme().no_items, _theme().large_rooms, _random);
    _player.set_items_allowed(! _theme().no_items);

    if(_theme().no_items && _player.held_item())
    {
        _messages.show("The DOJO bans held items!");
    }

    if(_theme().ghosts_need_scope)
    {
        _messages.show(_has_silph_scope ? "The SILPH SCOPE revealed the ghosts!" : "GHOST: Get out... Get out...");
    }
    _has_flute = false;
    _boss_defeated = false;
    _flute_room = -1;
    bool key_needed = (_theme().key == key_item::poke_flute) ||
                      (_theme().key == key_item::silph_scope && ! _has_silph_scope);

    if(key_needed)
    {
        bn::vector<int, floor_map::max_rooms> combat_rooms;

        for(int index = 0; index < _floor.size(); ++index)
        {
            if(_floor[index].kind == room_kind::combat && _floor[index].reward == room_reward::none)
            {
                combat_rooms.push_back(index);
            }
        }

        if(combat_rooms.empty())
        {
            for(int index = 0; index < _floor.size(); ++index)
            {
                if(_floor[index].kind == room_kind::combat)
                {
                    combat_rooms.push_back(index);
                }
            }
        }

        if(! combat_rooms.empty())
        {
            _flute_room = combat_rooms[_random.get_int(combat_rooms.size())];
        }
        else
        {
            _has_flute = true;
            _has_silph_scope = true;
        }
    }

    if(saved)
    {
        _floor.restore(saved->rooms, saved->room_count);
        _has_flute = saved->has_flute;
        _boss_defeated = saved->boss_defeated;
        _flute_room = saved->flute_room;
        _enter_room(saved->room, bn::nullopt);
        _messages.show(floor_label(_floor_number));
        return;
    }
    #ifdef DITTO_TEST_START_KIND
        for(int index = 0; index < _floor.size(); ++index)
        {
            if(_floor[index].kind == room_kind(DITTO_TEST_START_KIND))
            {
                _has_flute = room_kind(DITTO_TEST_START_KIND) == room_kind::stairs;
                _flute_room = _has_flute || _theme().key == key_item::none ? -1 : index;

                #ifdef DITTO_TEST_REWARD
                    _floor[index].reward = room_reward(DITTO_TEST_REWARD);
                    _flute_room = -1;
                #endif

                #ifdef DITTO_TEST_ITEM
                    _floor[index].item = item_id(DITTO_TEST_ITEM);
                #endif

                #ifdef DITTO_TEST_LAYOUT
                    _floor[index].layout = DITTO_TEST_LAYOUT;
                #endif

                #ifdef DITTO_TEST_OVERGROWN
                    for(int side = 0; side < 4; ++side)
                    {
                        int other = _floor.neighbor(index, direction(side));

                        if(other >= 0)
                        {
                            _floor[index].overgrown[side] = true;
                            _floor[other].overgrown[int(directions_of_floor::opposite(direction(side)))] = true;
                        }
                    }
                #endif

                _enter_room(index, bn::nullopt);

                #if defined(DITTO_TEST_FALL) || defined(DITTO_TEST_WARP)
                    #ifdef DITTO_TEST_WARP
                        constexpr char test_cell = room::cells::warp;
                    #else
                        constexpr char test_cell = room::cells::pit;
                    #endif

                    for(int row = 0; row < room::rows; ++row)
                    {
                        for(int column = 0; column < room::columns; ++column)
                        {
                            if(room::get(column, row) == test_cell)
                            {
                                _player.set_position(room::cell_center(column, row) - bn::fixed_point(0, 4));
                            }
                        }
                    }
                #endif

                _messages.show(floor_label(_floor_number));
                return;
            }
        }
    #endif

    #ifdef DITTO_TEST_MART
        for(int index = 0; index < _floor.size(); ++index)
        {
            if(_floor[index].reward == room_reward::mart)
            {
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

    bool boss_room = value.kind == room_kind::stairs && _theme().boss != boss_kind::none && ! _boss_defeated;
    _locked = (value.kind == room_kind::combat && ! value.cleared) ||
              (boss_room && _theme().boss != boss_kind::snorlax);
    _view.build(value, doors, _locked, _theme(), _floor_number * 977 + index * 131 + 7);
    _player.set_position(entered_from ? _view.entry_position(*entered_from) :
                                        _view.open_spot_near(_view.interior_center()));
    value.visited = true;

    if(value.kind == room_kind::combat && _locked)
    {
        _spawn_delay = spawn_delay_frames;
        _waves_left = _theme().waves - 1;
        _messages.show(_theme().lock_message);
        audio::play(bn::sound_items::sfx_door_lock);
    }

    if(boss_room)
    {
        _spawn_boss();
    }
    else if(value.kind == room_kind::stairs)
    {
        _messages.show("There are stairs going up!");
    }

    _update_flute();
    _update_reward();
    _update_darkness(true);

    _update_camera(true);
}

void game::_update_play()
{
    _view.update();

    if(bn::keypad::select_pressed() && _player.bag_item() && ! _player.transforming())
    {
        _player.use_bag(_messages);
    }

    if(_near_mart_counter())
    {
        _messages.show("Press A to shop!");

        if(bn::keypad::a_pressed())
        {
            _open_mart();
            return;
        }
    }
    int outline_index = _outline_below_player();
    const species_id* outline_species = outline_index >= 0 ? &_outlines[outline_index].species : nullptr;

    if(outline_species && ! _player.transforming() && ! _player.active_form())
    {
        _messages.show("Press B to TRANSFORM!");
    }

    bool outline_shiny = outline_index >= 0 && _outlines[outline_index].shiny;

    if(_player.update(_player_projectiles, _messages, outline_species, outline_shiny))
    {
        _outlines.erase(_outlines.begin() + outline_index);
    }

    if(const form* current = _player.active_form())
    {
        _register_form(current->species, current->shiny);
    }

    for(enemy& value : _enemies)
    {
        value.update(_player.position(), _enemy_projectiles, _random);

        if(value.take_reveal())
        {
            show_name_message(_messages, "A wild ", value.data().name, " jumped out!");
        }

        if(value.take_shiny_sighting())
        {
            show_name_message(_messages, "A shiny ", value.data().name, " appeared!");
            ++_shinies;
            profile::register_seen(value.id(), true);
            audio::play(bn::sound_items::sfx_key_item);
            _spawn_effect(value.position());
        }
    }

    _handle_cut();

    _player_projectiles.update([this](const bn::fixed_point& position) { _spawn_effect(position); });
    _enemy_projectiles.update([this](const bn::fixed_point& position) { _spawn_effect(position); });

    _handle_explosions();
    _handle_player_attacks();
    _handle_enemy_attacks();
    _update_boss();
    _update_outlines();

    _update_darkness(false);
    _update_plates();
    _update_flicker();
    _update_ember_rain();
    _update_effects();
    _update_camera(false);
    _messages.update();
    _update_struggle_check();
    _update_warps();
    _hud.update(_player);
    _update_room_state();
}

void game::_update_room_state()
{
    if(_spawn_delay && ! --_spawn_delay)
    {
        _spawn_enemies();
    }

    if(_locked && ! _spawn_delay && _enemies.empty() && ! _boss && _waves_left)
    {
        --_waves_left;
        _spawn_delay = spawn_delay_frames;

        bn::string<32> wave("Wave ");
        wave.append(bn::to_string<4>(_theme().waves - _waves_left));
        wave.append("! A new challenger!");
        _messages.show(wave);
        return;
    }

    if(_locked && ! _spawn_delay && _enemies.empty() && ! _boss)
    {
        _locked = false;
        _floor[_room].cleared = true;
        _view.set_locked(false);
        _messages.show(_theme().unlock_message);
        audio::play(bn::sound_items::sfx_door_open);

        if(_floor[_room].reward == room_reward::rare)
        {
            _floor[_room].reward_taken = true;
        }

        _update_flute();
        _update_reward();

        const form* current = _player.active_form();

        if(current && current->species == species_id::magikarp)
        {
            _messages.show("What? MAGIKARP is evolving!");
            _player.evolve(species_id::gyarados);
        }
    }

    if(_reward_pickup && within(_reward_pickup->position(), _player.position(), 12, 12))
    {
        _collect_reward();
    }

    if(_flute_pickup && within(_flute_pickup->position(), _player.position(), 12, 12))
    {
        _flute_pickup.reset();
        audio::play(bn::sound_items::sfx_key_item);

        if(_theme().key == key_item::silph_scope)
        {
            _has_silph_scope = true;
            _messages.show("DITTO found the SILPH SCOPE!");
            _messages.show("It reveals what hides in the dark.");
        }
        else
        {
            _has_flute = true;
            _messages.show("DITTO found the POKE FLUTE!");
        }
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

    if(_player.over_pit())
    {
        _fall_into_pit();
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
                _after_player_hit(shot.hit, result, &value);
                _spawn_effect(shot.position);
                return true;
            }
        }

        if(_boss && _boss->contains(shot.position, shot.half_size))
        {
            if(_boss->vulnerable())
            {
                hit_result result = _boss->take_hit(shot.hit);
                _after_player_hit(shot.hit, result, nullptr);
            }
            else if(_boss->asleep())
            {
                show_name_message(_messages, "", _boss->name(), " is fast asleep...");
            }

            _spawn_effect(shot.position);
            return true;
        }

        if(_boss && _boss->blocks(shot.position, shot.half_size))
        {
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
                _after_player_hit(_player.area_attack(), result, &value);
                _spawn_effect(value.position());

                if(_player.area_attack().move == move_id::struggle && serial != _last_recoil_serial)
                {
                    _last_recoil_serial = serial;
                    recoil = true;
                }
            }
        }
    }

    if(_player.area_active() && _boss && _boss->vulnerable() &&
       _boss->contains(_player.position(), _player.area_half_size()) && _boss->hit_by_area(_player.area_serial()))
    {
        hit_result result = _boss->take_hit(_player.area_attack());
        _after_player_hit(_player.area_attack(), result, nullptr);
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
        ++_defeated;
        _add_coins(1 + _random.get_int(3));
        profile::register_seen(value.id(), value.shiny());
        audio::play_quiet(bn::sound_items::sfx_faint);
        _spawn_effect(value.position());
        _spawn_outline(value.id(), value.position(), value.shiny());
        return true;
    });
}

void game::_spawn_outline(species_id id, const bn::fixed_point& position, bool shiny)
{
    if(_outlines.full())
    {
        return;
    }

    bn::fixed_point spot = position;

    if(room::feet_are_blocked(spot))
    {
        if(bn::optional<bn::fixed_point> shore = room::nearest_standable(spot, false))
        {
            spot = *shore;
        }
    }

    bn::sprite_ptr sprite = species::get(id).sprite->create_sprite(spot, species_frames::white);
    sprite.set_camera(_camera);
    sprite.set_z_order(500);
    _outlines.push_back(outline{ bn::move(sprite), id, outline_frames, shiny });
}

void game::_spawn_boss()
{
    audio::play(bn::sound_items::sfx_boss);
    audio::play_music(_theme().boss == boss_kind::mewtwo ? bn::music_items::final_boss : bn::music_items::boss);
    bn::fixed_point position = _view.interior_center() - bn::fixed_point(0, 12);

    if(_theme().boss == boss_kind::snorlax)
    {
        _boss.reset(new snorlax_boss(position, _camera));
        _messages.show("A SNORLAX sleeps on the stairs!");
    }
    else if(_theme().boss == boss_kind::venusaur)
    {
        _boss.reset(new venusaur_boss(position, _camera));
        _messages.show("A wild VENUSAUR blocks the stairs!");
    }
    else if(_theme().boss == boss_kind::onix)
    {
        _boss.reset(new onix_boss(position, _camera));
        _messages.show("The ground is shaking...");
        _messages.show("A wild ONIX burst out!");
    }
    else if(_theme().boss == boss_kind::gyarados)
    {
        _boss.reset(new gyarados_boss(position, _camera));
        _messages.show("A MAGIKARP is splashing around...");
    }
    else if(_theme().boss == boss_kind::zapdos)
    {
        _boss.reset(new zapdos_boss(position, _camera));
        _messages.show("ZAPDOS appeared in a flash!");
    }
    else if(_theme().boss == boss_kind::moltres)
    {
        _boss.reset(new moltres_boss(position, _camera));
        _messages.show("MOLTRES rose from the magma!");
    }
    else if(_theme().boss == boss_kind::articuno)
    {
        _boss.reset(new articuno_boss(position, _camera));
        _messages.show("A freezing wind... ARTICUNO appeared!");
    }
    else if(_theme().boss == boss_kind::pidgeot)
    {
        _boss.reset(new pidgeot_boss(position, _camera));
        _messages.show("PIDGEOT swooped down from above!");
    }
    else if(_theme().boss == boss_kind::dragonite)
    {
        _boss.reset(new dragonite_boss(position, _camera));
        _messages.show("A wild DRAGONITE descends!");
    }
    else if(_theme().boss == boss_kind::mewtwo)
    {
        _boss.reset(new mewtwo_boss(position, _camera));
        _messages.show("MEWTWO: So you are the other clone.");
        _messages.show("MEWTWO: There can be only one of us!");
    }
    else if(_theme().boss == boss_kind::gengar)
    {
        _boss.reset(new gengar_boss(position, _camera));
        _messages.show("A GENGAR rose from the shadows!");
    }
    else if(_theme().boss == boss_kind::hitmon)
    {
        bool kicker = _random.get_int(2);
        _boss.reset(new hitmon_boss(position, _camera, kicker));
        _messages.show(kicker ? "The DOJO MASTER sent out HITMONLEE!" : "The DOJO MASTER sent out HITMONCHAN!");
    }
    else
    {
        _boss.reset(new team_rocket_boss(position, _camera));
        _messages.show("JESSIE: Prepare for trouble!");
        _messages.show("JAMES: Make it double!");
        _messages.show("MEOWTH: Meowth, that's right!");
    }

    #ifdef DITTO_TEST_BOSS_HP
        _boss->set_test_hp(DITTO_TEST_BOSS_HP);
    #endif
}

void game::_update_boss()
{
    if(! _boss)
    {
        return;
    }

    if(! _boss->dead())
    {
        _boss->update(_player.position(), _enemy_projectiles, _random, _messages);
    }

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

    if(_boss->vulnerable())
    {
        _hud.show_boss(_boss->name(), _boss->hp(), _boss->max_hp());
    }

    bn::fixed_point wind = _boss->wind();

    if(wind != bn::fixed_point())
    {
        _player.push(wind);
    }

    if(bn::optional<boss_area_hit> area = _boss->area_hit())
    {
        _shake_frames = shake_frames * 2;

        if(_player.vulnerable() && within(area->center, _player.position(), area->radius, area->radius))
        {
            hit_result result = _player.take_hit(area->hit, _messages);

            if(result.effectiveness)
            {
                _player.apply_status(_roll_status(area->hit.move), _messages);
            }
        }
    }

    if(_boss->dead() && _theme().boss == boss_kind::mewtwo)
    {
        audio::play(bn::sound_items::sfx_faint);
        audio::stop_music();
        _messages.clear();
        _boss->announce_defeat(_messages);
        _enemy_projectiles.clear();
        _wait_for_a(defeat_min_frames);
        ++_defeated;
        _add_coins(mewtwo_coins);
        profile::register_seen(_boss->species(), false);
        profile::register_seen(_boss->outline_species(), false);
        _won = true;
        return;
    }

    if(_boss->dead())
    {
        audio::play(bn::sound_items::sfx_faint);
        audio::play_floor_music(_floor_number);
        _boss->announce_defeat(_messages);
        _spawn_effect(_boss->position());
        _spawn_outline(_boss->outline_species(), _boss->position());
        profile::register_seen(_boss->species(), false);
        profile::register_seen(_boss->outline_species(), false);

        if(bn::optional<species_id> extra = _boss->extra_outline())
        {
            profile::register_seen(*extra, false);
            _spawn_outline(*extra, _boss->position() + bn::fixed_point(28, 0));
        }
        _boss.reset();
        _boss_defeated = true;
        ++_defeated;
        _add_coins(boss_coins);
        _messages.show("DITTO found some coins!");
        _hud.hide_boss();
        _enemy_projectiles.clear();
    }
}

void game::_update_darkness(bool room_changed)
{
    if(! _theme().dark)
    {
        if(room_changed)
        {
            _light.reset();
            bn::window::outside().set_show_all();
        }

        return;
    }

    if(room_changed)
    {
        if(! _light)
        {
            bn::sprite_ptr light = bn::sprite_items::light.create_sprite(_player.position());
            light.set_camera(_camera);
            light.set_scale(light_scale);
            light.set_window_enabled(true);
            _light = bn::move(light);
        }

        bn::window::outside().set_show_all();
        bn::window::outside().set_show_bg(_view.bg(), false);
    }

    _light->set_position(_player.position());

    bool ghosts_unseen = _theme().ghosts_need_scope && ! _has_silph_scope;

    for(enemy& value : _enemies)
    {
        const species_data& data = value.data();
        bool ghost = data.type_1 == pokemon_type::ghost || data.type_2 == pokemon_type::ghost;
        value.set_in_light(within(value.position(), _player.position(), light_radius, light_radius));
        value.set_unseen(ghost && ghosts_unseen);
    }

    if(_boss)
    {
        _boss->set_revealed(! ghosts_unseen);
    }
}

void game::_update_plates()
{
    hazard_kind hazard = _theme().hazard;

    if(hazard == hazard_kind::none)
    {
        return;
    }

    int cycle = _boss ? boss_plate_cycle : plate_cycle;
    _plate_timer = (_plate_timer + 1) % cycle;
    int phase = _plate_timer < cycle - plate_warning - plate_shock ? 0 : _plate_timer < cycle - plate_shock ? 1 : 2;
    _view.set_plate_phase(phase);

    if(phase != 2)
    {
        return;
    }

    if(_plate_timer == cycle - plate_shock)
    {
        --_plate_serial;
    }

    bool lava = hazard == hazard_kind::lava;
    bool gas = hazard == hazard_kind::gas;
    attack shock = lava ? attack{ move_id::ember, pokemon_type::fire, plate_power } :
                   gas ? attack{ move_id::poison_gas, pokemon_type::poison, gas_power } :
                         attack{ move_id::thundershock, pokemon_type::electric, plate_power };
    pokemon_type immune_type = lava ? pokemon_type::fire : pokemon_type::poison;
    auto fireproof = [lava, gas, immune_type](const species_data& data)
    {
        return (lava || gas) && (data.type_1 == immune_type || data.type_2 == immune_type);
    };

    bn::fixed_point feet = _player.position() + bn::fixed_point(0, 4);

    if(_player.vulnerable() && ! fireproof(_player.body()) && room::at(feet.x(), feet.y()) == room::cells::plate)
    {
        _messages.show(lava ? "The lava is burning DITTO!" : gas ? "DITTO breathed in poison gas!" :
                       "The floor is electrified!");
        audio::play(bn::sound_items::sfx_shock);
        hit_result result = _player.take_hit(shock, _messages);
        _shake_frames = shake_frames;

        if(gas && result.effectiveness)
        {
            _player.apply_status(status_effect::poison, _messages);
        }
        else if(! lava && result.effectiveness && _random.get_int(100) < plate_paralysis_chance)
        {
            _player.apply_status(status_effect::paralysis, _messages);
        }
    }

    for(enemy& value : _enemies)
    {
        bn::fixed_point enemy_feet = value.position() + bn::fixed_point(0, 4);

        if(value.active() && ! fireproof(value.data()) &&
           room::at(enemy_feet.x(), enemy_feet.y()) == room::cells::plate && value.hit_by_area(_plate_serial))
        {
            hit_result result = value.take_hit(shock);
            static_cast<void>(result);
            _spawn_effect(value.position());
        }
    }
}

void game::_update_flicker()
{
    if(_theme().hazard != hazard_kind::electric)
    {
        return;
    }

    if(_flicker_frames)
    {
        --_flicker_frames;
        bn::bg_palettes::set_fade(bn::color(0, 0, 0), _flicker_frames && (_flicker_frames / 2) % 2 ? 0.6 : 0);
        return;
    }

    if(--_flicker_timer <= 0)
    {
        _flicker_frames = flicker_frames;
        _flicker_timer = 300 + _random.get_int(300);
    }
}

void game::_update_ember_rain()
{
    if(! _theme().ember_rain || _current_room().kind == room_kind::start || _player.transforming())
    {
        _embers.clear();
        return;
    }

    bn::erase_if(_embers, [this](falling_ember& value)
    {
        value.marker.set_visible((value.frames / 3) % 2 == 0);

        if(--value.frames > 0)
        {
            return false;
        }

        attack hit{ move_id::ember, pokemon_type::fire, ember_power };
        bn::fixed_point position = value.marker.position();
        _enemy_projectiles.spawn(bn::sprite_items::electric_projectiles.create_sprite(position, electric_frames::ember),
                                 position, bn::fixed_point(), hit, ember_life, ember_half_size, false);
        return true;
    });

    if(--_ember_timer > 0 || _embers.full())
    {
        return;
    }

    bn::fixed_point offset(_random.get_int(ember_spread * 2) - ember_spread,
                           _random.get_int(ember_spread * 2) - ember_spread);
    bn::fixed_point position = _player.position() + offset;

    if(! room::feet_are_blocked(position, true))
    {
        bn::sprite_ptr marker = bn::sprite_items::projectiles.create_sprite(position, projectile_frames::impact);
        marker.set_camera(_camera);
        marker.set_z_order(-900);
        _embers.push_back(falling_ember{ bn::move(marker), ember_warning_frames });
    }

    int interval = _boss ? boss_ember_interval : ember_interval;
    _ember_timer = interval + _random.get_int(interval / 2);
}

void game::_handle_explosions()
{
    for(enemy& value : _enemies)
    {
        if(! value.take_explosion())
        {
            continue;
        }

        show_name_message(_messages, "", value.data().name, " used SELFDESTRUCT!");
        audio::play(bn::sound_items::sfx_explosion);
        _shake_frames = shake_frames * 2;

        for(bn::fixed_point offset : { bn::fixed_point(-10, -8), bn::fixed_point(10, -8), bn::fixed_point(0, 10) })
        {
            _spawn_effect(value.position() + offset);
        }

        if(_player.vulnerable() && within(value.position(), _player.position(), explosion_radius, explosion_radius))
        {
            static_cast<void>(_player.take_hit(value.explosion_attack(), _messages));
        }
    }
}

void game::_update_warps()
{
    bn::fixed_point feet = _player.position() + bn::fixed_point(0, 4);

    if(room::at(feet.x(), feet.y()) != room::cells::warp || _player.transforming())
    {
        _on_warp = false;
        return;
    }

    if(_on_warp)
    {
        return;
    }

    _on_warp = true;

    if(bn::optional<bn::fixed_point> partner = _view.warp_partner(feet))
    {
        _player.set_position(*partner - bn::fixed_point(0, 4));
        _messages.show("DITTO was warped!");
        audio::play(bn::sound_items::sfx_warp);
        _spawn_effect(*partner);
    }
}

void game::_update_struggle_check()
{
    const form* current = _player.active_form();

    if(! current)
    {
        _player.set_forced_struggle(false, _messages);
        return;
    }

    const species_data& body = species::get(current->species);
    move_id own_moves[] = { body.move_a, body.move_b };
    bool has_foe = false;
    bool can_affect = false;

    auto check = [&](const species_data& foe)
    {
        has_foe = true;

        for(move_id move : own_moves)
        {
            const move_data& data = moves::get(move);

            if(data.power && types::effectiveness(data.type, foe.type_1, foe.type_2))
            {
                can_affect = true;
            }
        }
    };

    for(const enemy& value : _enemies)
    {
        check(value.data());
    }

    if(_boss)
    {
        check(species::get(_boss->species()));
    }

    _player.set_forced_struggle(has_foe && ! can_affect, _messages);
}

void game::_handle_cut()
{
    gate_kind gate = _theme().gate;

    if(gate == gate_kind::none)
    {
        return;
    }

    bool ice = gate == gate_kind::ice;
    bool rock = gate == gate_kind::cracked;
    bool spirit = gate == gate_kind::spirit;
    pokemon_type needed = ice ? pokemon_type::fire : rock ? pokemon_type::fighting :
                          spirit ? pokemon_type::ghost : pokemon_type::grass;
    const species_data& body = _player.body();
    bool can_clear = body.type_1 == needed || body.type_2 == needed;
    bn::fixed_point feet = _player.position() + bn::fixed_point(0, 4);

    if(! can_clear)
    {
        for(bn::fixed_point offset : { bn::fixed_point(-10, 0), bn::fixed_point(10, 0), bn::fixed_point(0, -10),
                                       bn::fixed_point(0, 10) })
        {
            bn::fixed_point probe = feet + offset;

            if(room::at(probe.x(), probe.y()) == room::cells::bush)
            {
                _messages.show(ice ? "A FIRE POKEMON could melt this ice." :
                               rock ? "A FIGHTING POKEMON can smash this." :
                               spirit ? "Only a GHOST POKEMON can pass this." :
                                        "A GRASS POKEMON could CUT this bush.");
                return;
            }
        }

        return;
    }

    if(! _view.cut_bushes(feet, 8))
    {
        return;
    }

    audio::play(bn::sound_items::sfx_hit);
    _messages.show(ice ? "DITTO melted the ice!" : rock ? "DITTO used ROCK SMASH!" :
                   spirit ? "DITTO phased through the barrier!" : "DITTO used CUT!");
    _spawn_effect(feet);

    for(int side = 0; side < 4; ++side)
    {
        if(_floor[_room].overgrown[side] && ! _view.bushes_remaining(direction(side)))
        {
            _floor.clear_overgrown(_room, direction(side));
        }
    }
}

status_effect game::_roll_status(move_id move)
{
    const move_data& data = moves::get(move);

    if(data.status == status_effect::none || _random.get_int(100) >= data.status_chance)
    {
        return status_effect::none;
    }

    return data.status;
}

void game::_after_player_hit(const attack& hit, const hit_result& result, enemy* target)
{
    _messages.show(combat::effectiveness_message(result.effectiveness));

    if(result.effectiveness > types::neutral)
    {
        audio::play(bn::sound_items::sfx_super);
    }
    else if(result.effectiveness && result.effectiveness < types::neutral)
    {
        audio::play(bn::sound_items::sfx_weak);
    }
    else if(result.effectiveness)
    {
        audio::play_quiet(bn::sound_items::sfx_hit);
    }

    if(! result.effectiveness)
    {
        return;
    }

    if(target)
    {
        target->apply_status(_roll_status(hit.move));
    }

    if(result.damage && moves::get(hit.move).drain)
    {
        _player.heal(bn::max(result.damage / 2, 1));
    }
}

void game::_update_flute()
{
    bool scope = _theme().key == key_item::silph_scope;
    bool collected = scope ? _has_silph_scope : _has_flute;

    if(collected || _flute_pickup || _room != _flute_room || ! _floor[_room].cleared)
    {
        return;
    }

    bn::fixed_point position = _view.open_spot_near(_view.interior_center());
    bn::sprite_ptr sprite = scope ? bn::sprite_items::pickups.create_sprite(position, 2) :
                                    bn::sprite_items::poke_flute.create_sprite(position);
    sprite.set_camera(_camera);
    sprite.set_z_order(600);
    _flute_pickup = bn::move(sprite);
    _messages.show("Something shiny is on the floor!");
}

void game::_update_reward()
{
    floor_room& value = _floor[_room];

    if(value.reward == room_reward::mart && ! _mart_counter)
    {
        bn::sprite_ptr counter = bn::sprite_items::mart.create_sprite(_view.interior_center() - bn::fixed_point(0, 28));
        counter.set_camera(_camera);
        counter.set_z_order(-counter.position().y().round_integer() - 8);
        _mart_counter = bn::move(counter);
        _messages.show("Welcome to the POKE MART!");
        return;
    }
    bool pickup = value.reward == room_reward::journal || value.reward == room_reward::item;

    if(_reward_pickup || ! pickup || value.reward_taken || ! value.cleared)
    {
        return;
    }

    bool page = value.reward == room_reward::journal;
    bn::fixed_point position = _view.open_spot_near(_view.interior_center() +
                                                    bn::fixed_point(0, _flute_room == _room ? 20 : 0));
    bn::sprite_ptr sprite = bn::sprite_items::pickups.create_sprite(position, page ? 1 : 0);
    sprite.set_camera(_camera);
    sprite.set_z_order(600);
    _reward_pickup = bn::move(sprite);
    _messages.show(page ? "A torn page is lying here!" : "There is an item on the floor!");
}

void game::_collect_reward()
{
    floor_room& value = _floor[_room];

    if(value.reward == room_reward::journal)
    {
        _reward_pickup.reset();
        value.reward_taken = true;
        _journal_mask |= 1 << (_floor_number - 1);
        audio::play(bn::sound_items::sfx_key_item);
        _show_journal_page(_floor_number - 1);
        return;
    }

    if(_player.give_item(value.item, _messages))
    {
        audio::play(bn::sound_items::sfx_pickup);
        _reward_pickup.reset();
        value.reward_taken = true;
    }
}

void game::_generate_journal_page(int page, bn::ivector<bn::sprite_ptr>& text)
{
    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::string<32> title("LAB JOURNAL  PAGE ");
    title.append(bn::to_string<4>(page + 1));
    small.generate(0, -56, title, text);

    for(int line = 0; line < journal::lines_per_page; ++line)
    {
        big.generate(0, -20 + line * 18, journal::line(page, line), text);
    }
}

void game::_show_journal_page(int page)
{
    _set_world_visible(false);
    _overlay.show_black();

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::vector<bn::sprite_ptr, 40> text;
    _generate_journal_page(page, text);
    small.generate(0, 70, "A: CLOSE", text);
    _wait_for_a(page_min_frames);

    text.clear();
    _overlay.hide();
    _set_world_visible(true);
    bn::core::update();
}

void game::_read_journal()
{
    bn::vector<int, journal::page_count> collected;

    for(int page = 0; page < journal::page_count; ++page)
    {
        if(_journal_mask & (1 << page))
        {
            collected.push_back(page);
        }
    }

    _overlay.show_black();

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::vector<bn::sprite_ptr, 48> text;
    int index = 0;
    auto draw = [&]()
    {
        text.clear();
        _generate_journal_page(collected[index], text);

        bn::string<32> position(index > 0 ? "<  " : "   ");
        position.append(bn::to_string<4>(index + 1));
        position.append("/");
        position.append(bn::to_string<4>(collected.size()));
        position.append(index < collected.size() - 1 ? "  >" : "   ");
        small.generate(0, 58, position, text);
        small.generate(0, 70, "B: BACK", text);
    };

    draw();
    bn::core::update();

    while(! bn::keypad::b_pressed())
    {
        if(bn::keypad::left_pressed() && index > 0)
        {
            --index;
            audio::play(bn::sound_items::sfx_menu);
            draw();
        }
        else if(bn::keypad::right_pressed() && index < collected.size() - 1)
        {
            ++index;
            audio::play(bn::sound_items::sfx_menu);
            draw();
        }

        bn::core::update();
    }

    text.clear();
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

    if(! hit && _boss && _boss->touches(_player.position()))
    {
        hit = _boss->contact_attack();
    }

    if(hit)
    {
        hit_result result = _player.take_hit(*hit, _messages);
        _shake_frames = shake_frames;

        if(result.damage)
        {
            audio::play(bn::sound_items::sfx_hurt);
        }

        if(result.effectiveness)
        {
            _player.apply_status(_roll_status(hit->move), _messages);
        }
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

    int count = _theme().waves > 1 ? 3 : bn::min(2 + _floor_number / 2 + _random.get_int(2), _enemies.max_size());

    for(int index = 0; index < count; ++index)
    {
        for(int attempt = 0; attempt < 20; ++attempt)
        {
            species_id id = floor_themes::pick_species(_theme(), _random.get_int(100));

            #ifdef DITTO_TEST_SPECIES
                id = species_id(DITTO_TEST_SPECIES);
            #endif

            const species_data& data = species::get(id);
            bn::fixed_point position = _view.random_floor_position(_random);
            bool aquatic = data.behavior == species_behavior::aquatic;

            if(aquatic || (_theme().water && species::can_swim(data) && _random.get_int(100) < 40))
            {
                if(bn::optional<bn::fixed_point> water = _view.random_water_position(_random))
                {
                    position = *water;
                }
                else if(aquatic)
                {
                    id = species_id::poliwag;
                }
            }

            if(_theme().tall_grass && _random.get_int(100) < 60)
            {
                if(bn::optional<bn::fixed_point> grass = _view.random_grass_position(_random))
                {
                    position = *grass;
                }
            }

            bn::fixed_point delta = position - _player.position();

            if(bn::abs(delta.x()) + bn::abs(delta.y()) < min_spawn_distance)
            {
                continue;
            }

            _enemies.emplace_back(id, position, _camera, _random);
            break;
        }
    }

    floor_room& current_room = _floor[_room];

    bool first_wave = _waves_left == _theme().waves - 1;

    if(current_room.reward == room_reward::rare && ! current_room.reward_taken && first_wave && ! _enemies.empty())
    {
        bn::fixed_point position = _enemies.back().position();
        _enemies.pop_back();
        _enemies.emplace_back(_theme().rare, position, _camera, _random);
        show_name_message(_messages, "A rare ", species::get(_theme().rare).name, " is here!");
    }

    if(_theme().tall_grass)
    {
        _messages.show("Something rustles in the grass...");
    }
    else if(_enemies.size() == 1)
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
    _previous_room = _room;
    _enter_room(next, directions_of_floor::opposite(side));
    _fade(false);
}

void game::_climb_stairs()
{
    _messages.clear();
    _messages.show("DITTO went up the stairs!");
    audio::play(bn::sound_items::sfx_stairs);

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

void game::_fall_into_pit()
{
    _messages.show("DITTO fell down the chasm!");
    audio::play(bn::sound_items::sfx_fall);
    _fade(true);
    _player.take_fall_damage(fall_damage);
    _enter_room(_previous_room >= 0 ? _previous_room : _room, bn::nullopt);
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
    _reward_pickup.reset();
    _mart_counter.reset();
    _embers.clear();
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
    audio::play(bn::sound_items::sfx_menu);
    _set_world_visible(false);
    _overlay.show_map(_floor, _room);

    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::string<32> held("HELD: ");
    held.append(_player.held_item() ? items::get(*_player.held_item()).name : "NOTHING");

    bn::string<32> bag("BAG: ");
    bag.append(_player.bag_item() ? items::get(*_player.bag_item()).name : "EMPTY");
    bag.append(_player.bag_item() ? "  (SELECT)" : "");

    bn::string<32> pages("JOURNAL ");
    pages.append(bn::to_string<4>(_journal_page_count()));
    pages.append("/");
    pages.append(bn::to_string<4>(journal::page_count));
    pages.append("  COINS ");
    pages.append(bn::to_string<8>(profile::get().coins));

    bn::vector<bn::sprite_ptr, 56> text;
    auto show_header = [&]()
    {
        text.clear();
        big.generate(0, -68, floor_label(_floor_number), text);
        small.generate(0, -52, held, text);
        small.generate(0, -43, bag, text);
        small.generate(0, -34, pages, text);

        if(_journal_mask)
        {
            small.generate(0, -25, "A: READ JOURNAL", text);
        }
    };

    show_header();
    bn::vector<bn::sprite_ptr, 32> prompt;
    bool confirming = false;
    auto show_prompt = [&]()
    {
        prompt.clear();
        small.generate(0, 64, confirming ? "A: SAVE AND QUIT" : "START: RESUME", prompt);
        small.generate(0, 74, confirming ? "B: BACK" : "SELECT: SAVE AND QUIT", prompt);
    };

    show_prompt();
    bn::core::update();

    while(true)
    {
        if(confirming)
        {
            if(bn::keypad::a_pressed())
            {
                _save_and_quit();
                return;
            }

            if(bn::keypad::b_pressed())
            {
                confirming = false;
                show_prompt();
            }
        }
        else if(bn::keypad::start_pressed())
        {
            break;
        }
        else if(bn::keypad::a_pressed() && _journal_mask)
        {
            audio::play(bn::sound_items::sfx_menu);
            text.clear();
            prompt.clear();
            _read_journal();
            _overlay.show_map(_floor, _room);
            show_header();
            show_prompt();
        }
        else if(bn::keypad::select_pressed())
        {
            audio::play(bn::sound_items::sfx_menu);
            confirming = true;
            show_prompt();
        }

        bn::core::update();
    }

    prompt.clear();
    text.clear();
    _overlay.hide();
    _set_world_visible(true);
    bn::core::update();
}

void game::_register_form(species_id id, bool shiny)
{
    unsigned bit = 1u << (int(id) % 32);
    unsigned& word = _run_forms[int(id) / 32];

    if(! (word & bit) || shiny)
    {
        word |= bit;
        profile::register_form(id, shiny);
    }
}

void game::_show_run_stats(const char* title)
{
    _overlay.show_black();

    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    int seconds = _run_frames / 60;
    bn::string<24> time("TIME ");

    if(seconds >= 3600)
    {
        time.append(bn::to_string<4>(seconds / 3600));
        time.append(":");
    }

    int minutes = (seconds / 60) % 60;
    time.append(minutes < 10 && seconds >= 3600 ? "0" : "");
    time.append(bn::to_string<4>(minutes));
    time.append(seconds % 60 < 10 ? ":0" : ":");
    time.append(bn::to_string<4>(seconds % 60));

    int forms = 0;

    for(unsigned word : _run_forms)
    {
        for(; word; word &= word - 1)
        {
            ++forms;
        }
    }

    auto line = [](const char* label, int value)
    {
        bn::string<24> result(label);
        result.append(bn::to_string<8>(value));
        return result;
    };

    bn::string<24> dex("POKEDEX ");
    dex.append(bn::to_string<4>(profile::seen_count()));
    dex.append("/");
    dex.append(bn::to_string<4>(int(species_id::mew)));

    bn::vector<bn::sprite_ptr, 48> text;
    big.generate(0, -60, title, text);
    small.generate(0, -32, time, text);
    small.generate(0, -20, line("FLOOR REACHED ", _floor_number), text);
    small.generate(0, -8, line("POKEMON DEFEATED ", _defeated), text);
    small.generate(0, 4, line("FORMS USED ", forms), text);
    small.generate(0, 16, line("SHINIES SEEN ", _shinies), text);
    small.generate(0, 28, line("JOURNAL PAGES ", _journal_page_count()), text);
    small.generate(0, 40, line("COINS EARNED ", _coins_earned), text);
    small.generate(0, 54, dex, text);
    small.generate(0, 70, "PRESS START", text);
    bn::core::update();

    while(! bn::keypad::start_pressed())
    {
        bn::core::update();
    }

    text.clear();
    _overlay.hide();
    bn::core::update();
}

void game::_add_coins(int amount)
{
    _coins_earned += amount;
    profile::add_coins(amount);
}

bool game::_near_mart_counter() const
{
    if(! _mart_counter)
    {
        return false;
    }

    bn::fixed_point delta = _player.position() - _mart_counter->position();
    return bn::abs(delta.x()) < 20 && delta.y() > 8 && delta.y() < 32;
}

void game::_open_mart()
{
    audio::play(bn::sound_items::sfx_menu);
    _set_world_visible(false);
    _overlay.show_black();

    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::sprite_text_generator left(common::fixed_8x8_sprite_font);
    left.set_left_alignment();
    left.set_bg_priority(0);

    bn::sprite_text_generator right(common::fixed_8x8_sprite_font);
    right.set_right_alignment();
    right.set_bg_priority(0);

    floor_room& room = _floor[_room];
    bn::vector<bn::sprite_ptr, 64> text;
    int cursor = 0;
    const char* notice = "";

    auto draw = [&]()
    {
        text.clear();
        big.generate(0, -66, "POKE MART", text);

        bn::string<24> coins("COINS ");
        coins.append(bn::to_string<8>(profile::get().coins));
        small.generate(0, -46, coins, text);

        for(int slot = 0; slot < 3; ++slot)
        {
            const item_data& item = items::get(room.stock[slot]);
            int y = -24 + slot * 16;
            bn::string<24> name(slot == cursor ? "> " : "  ");
            name.append(item.name);
            left.generate(-100, y, name, text);

            if(room.sold[slot])
            {
                right.generate(100, y, "SOLD OUT", text);
            }
            else
            {
                right.generate(100, y, bn::to_string<8>(item.price), text);
            }
        }

        small.generate(0, 30, items::get(room.stock[cursor]).description, text);
        small.generate(0, 46, notice, text);
        small.generate(0, 70, "A: BUY  B: LEAVE", text);
    };

    draw();
    bn::core::update();

    while(! bn::keypad::b_pressed())
    {
        if(bn::keypad::up_pressed() || bn::keypad::down_pressed())
        {
            cursor = (cursor + (bn::keypad::up_pressed() ? 2 : 1)) % 3;
            notice = "";
            audio::play_quiet(bn::sound_items::sfx_menu);
            draw();
        }
        else if(bn::keypad::a_pressed())
        {
            const item_data& item = items::get(room.stock[cursor]);

            if(room.sold[cursor])
            {
                notice = "That is sold out.";
            }
            else if(profile::get().coins < item.price)
            {
                notice = "You don't have enough coins.";
            }
            else if(! items::held(item.kind) && _player.bag_item())
            {
                notice = "The BAG is full!";
            }
            else if(! _player.give_item(room.stock[cursor], _messages))
            {
                notice = "DITTO can't use that now.";
            }
            else
            {
                static_cast<void>(profile::spend_coins(item.price));
                room.sold[cursor] = true;
                notice = "Thank you!";
                audio::play(bn::sound_items::sfx_pickup);
            }

            draw();
        }

        bn::core::update();
    }

    audio::play(bn::sound_items::sfx_menu);
    text.clear();
    _overlay.hide();
    _set_world_visible(true);
    bn::core::update();
}

void game::_wait_for_a(int min_frames)
{
    wait_for_a(min_frames, []()
    {
    });
}

void game::_save_and_quit()
{
    save_data data;
    data.floor_number = _floor_number;
    data.room = _room;
    data.journal_mask = _journal_mask;
    data.run_frames = _run_frames;
    data.defeated = _defeated;
    data.shinies = _shinies;
    data.coins_earned = _coins_earned;

    for(int word = 0; word < 3; ++word)
    {
        data.run_forms[word] = _run_forms[word];
    }
    data.flute_room = _flute_room;
    data.has_flute = _has_flute;
    data.has_silph_scope = _has_silph_scope;
    data.boss_defeated = _boss_defeated;
    data.player = _player.state();
    data.room_count = _floor.size();

    for(int index = 0; index < _floor.size(); ++index)
    {
        data.rooms[index] = _floor[index];
    }

    save::write(data);
    audio::play(bn::sound_items::sfx_pickup);
    _quit = true;
    _fade(true);
    _clear_room_objects();
    _messages.clear();
    _hud.set_visible(false);
    _player.set_visible(false);
    _overlay.hide();
    bn::core::update();
    set_fade(0);
}

void game::_ending()
{
    profile::record_win();
    struct page
    {
        const char* lines[3];
    };

    constexpr page story[] = {
        { { "MEWTWO fell to its knees.", "", "" } },
        { { "A soft pink light filled", "the cave. MEW floated", "down to DITTO." } },
        { { "MEW: You came all this way,", "little one.", "" } },
        { { "MEW: You were made from me,", "just like MEWTWO.", "" } },
        { { "MEW: You have no true shape.", "That is not a failure.", "That is your gift." } },
        { { "DITTO smiled its silly smile...", "", "" } },
    };

    constexpr page secret[] = {
        { { "MEW: You found every page", "of the lab journal.", "You know the whole truth." } },
        { { "MEW: Then take my shape too.", "It was always yours.", "" } },
    };

    for(int frame = 0; frame < 120; ++frame)
    {
        _messages.update();
        _update_effects();
        bn::core::update();
    }

    _fade(true);
    audio::play_music(bn::music_items::ending);
    _clear_room_objects();
    _messages.clear();
    _hud.set_visible(false);
    _player.set_visible(false);
    _light.reset();
    bn::window::outside().set_show_all();
    _overlay.show_black();
    set_fade(0);

    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::sprite_ptr mew = bn::sprite_items::mew.create_sprite(-24, 30, species_frames::walk);
    bn::sprite_ptr ditto = bn::sprite_items::ditto.create_sprite(24, 34, species_frames::own_walk);
    bool shiny_ditto = _player.shiny_ditto();
    auto set_ditto_item = [&](const bn::sprite_item& item, int frame)
    {
        ditto.set_item(item, frame);

        if(shiny_ditto && &item == &bn::sprite_items::ditto)
        {
            ditto.set_palette(shiny::ditto_palette());
        }
    };
    set_ditto_item(bn::sprite_items::ditto, species_frames::own_walk);
    mew.set_bg_priority(0);
    mew.set_horizontal_flip(true);

    bool shiny_mew = shiny::roll(_random);
    profile::register_seen(species_id::mew, shiny_mew);

    if(shiny_mew)
    {
        mew.set_palette(*shiny::palette(species_id::mew));
    }
    ditto.set_bg_priority(0);
    int frame_counter = 0;

    auto show_page = [&](const page& value)
    {
        bn::vector<bn::sprite_ptr, 40> text;

        for(int line = 0; line < 3; ++line)
        {
            big.generate(0, -60 + line * 18, value.lines[line], text);
        }

        small.generate(0, 72, "A: NEXT", text);
        bn::core::update();

        wait_for_a(page_min_frames, [&]()
        {
            ++frame_counter;
            mew.set_y(30 + bn::degrees_lut_sin((frame_counter * 4) % 360) * 3);
            mew.set_tiles(bn::sprite_items::mew.tiles_item(), species_frames::walk + (frame_counter / 20) % 2);
        });
    };

    auto transform_ditto = [&](const bn::sprite_item& target, const char* message)
    {
        _register_form(&target == &bn::sprite_items::mew ? species_id::mew : species_id::mewtwo, false);

        for(int frame = 0; frame < 60; ++frame)
        {
            bool show_target = (frame / 4) % 2 && frame > 20;
            set_ditto_item(show_target ? target : bn::sprite_items::ditto, species_frames::white);
            bn::core::update();
        }

        set_ditto_item(target, species_frames::own_walk);
        show_page(page{ { message, "", "" } });
    };

    for(const page& value : story)
    {
        show_page(value);
    }

    transform_ditto(bn::sprite_items::mewtwo, "...and TRANSFORMED into MEWTWO!");

    if(_journal_page_count() >= journal::page_count)
    {
        set_ditto_item(bn::sprite_items::ditto, species_frames::own_walk);

        for(const page& value : secret)
        {
            show_page(value);
        }

        transform_ditto(bn::sprite_items::mew, "DITTO transformed into MEW!");
    }

    bn::string<32> pages("JOURNAL ");
    pages.append(bn::to_string<4>(_journal_page_count()));
    pages.append("/");
    pages.append(bn::to_string<4>(journal::page_count));

    bn::vector<bn::sprite_ptr, 32> text;
    big.generate(0, -50, "THE END", text);
    small.generate(0, -30, "THANKS FOR PLAYING!", text);
    small.generate(0, 60, pages, text);
    small.generate(0, 72, "PRESS START", text);

    while(! bn::keypad::start_pressed())
    {
        ++frame_counter;
        mew.set_y(30 + bn::degrees_lut_sin((frame_counter * 4) % 360) * 3);
        bn::core::update();
    }

    text.clear();
    mew.set_visible(false);
    ditto.set_visible(false);
    _show_run_stats("RUN COMPLETE!");
}

void game::_game_over()
{
    audio::stop_music();
    audio::play(bn::sound_items::sfx_faint);
    _clear_room_objects();
    _messages.clear();
    _hud.set_visible(false);
    _player.set_visible(false);
    bn::bg_palettes::set_fade(bn::color(0, 0, 0), 0.6);

    _show_run_stats("DITTO blacked out!");
}
