#include "game.h"

#include "bn_bg_palettes.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_window.h"

#include "bn_sprite_items_light.h"
#include "bn_sprite_items_pickups.h"
#include "bn_sprite_items_poke_flute.h"
#include "bn_sprite_items_projectiles.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "gyarados_boss.h"
#include "journal.h"
#include "onix_boss.h"
#include "projectile_frames.h"
#include "snorlax_boss.h"
#include "venusaur_boss.h"
#include "zapdos_boss.h"

namespace
{
    constexpr int spawn_delay_frames = 30;
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
    constexpr int plate_paralysis_chance = 25;
    constexpr int explosion_radius = 34;
    constexpr int flicker_frames = 16;

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

    #ifdef DITTO_TEST_FLOOR
        _floor_number = DITTO_TEST_FLOOR;
    #endif

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
    _floor.generate(_floor_number, _theme().overgrown_percent, _random);
    _has_flute = false;
    _boss_defeated = false;
    _flute_room = -1;

    if(_theme().boss == boss_kind::snorlax)
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
        }
    }
    #ifdef DITTO_TEST_START_KIND
        for(int index = 0; index < _floor.size(); ++index)
        {
            if(_floor[index].kind == room_kind(DITTO_TEST_START_KIND))
            {
                _has_flute = room_kind(DITTO_TEST_START_KIND) == room_kind::stairs;
                _flute_room = _has_flute || _theme().boss != boss_kind::snorlax ? -1 : index;

                #ifdef DITTO_TEST_REWARD
                    _floor[index].reward = room_reward(DITTO_TEST_REWARD);
                    _flute_room = -1;
                #endif

                #ifdef DITTO_TEST_ITEM
                    _floor[index].item = item_id(DITTO_TEST_ITEM);
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
    _player.set_position(entered_from ? _view.entry_position(*entered_from) : _view.interior_center());
    value.visited = true;

    if(value.kind == room_kind::combat && _locked)
    {
        _spawn_delay = spawn_delay_frames;
        _messages.show(_theme().lock_message);
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

        if(value.take_reveal())
        {
            show_name_message(_messages, "A wild ", value.data().name, " jumped out!");
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
        _messages.show(_theme().unlock_message);

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
    _outlines.push_back(outline{ bn::move(sprite), id, outline_frames });
}

void game::_spawn_boss()
{
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
    else
    {
        _boss.reset(new zapdos_boss(position, _camera));
        _messages.show("ZAPDOS appeared in a flash of lightning!");
    }
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

    if(_boss->vulnerable())
    {
        _hud.show_boss(_boss->name(), _boss->hp(), _boss->max_hp());
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

    if(_boss->dead())
    {
        show_name_message(_messages, "", _boss->name(), " fainted!");
        _spawn_effect(_boss->position());
        _spawn_outline(_boss->species(), _boss->position());
        _boss.reset();
        _boss_defeated = true;
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

    for(enemy& value : _enemies)
    {
        value.set_in_light(within(value.position(), _player.position(), light_radius, light_radius));
    }
}

void game::_update_plates()
{
    if(! _theme().plates)
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

    attack shock{ move_id::thundershock, pokemon_type::electric, plate_power };
    bn::fixed_point feet = _player.position() + bn::fixed_point(0, 4);

    if(_player.vulnerable() && room::at(feet.x(), feet.y()) == room::cells::plate)
    {
        _messages.show("The floor is electrified!");
        hit_result result = _player.take_hit(shock, _messages);
        _shake_frames = shake_frames;

        if(result.effectiveness && _random.get_int(100) < plate_paralysis_chance)
        {
            _player.apply_status(status_effect::paralysis, _messages);
        }
    }

    for(enemy& value : _enemies)
    {
        bn::fixed_point enemy_feet = value.position() + bn::fixed_point(0, 4);

        if(value.active() && room::at(enemy_feet.x(), enemy_feet.y()) == room::cells::plate &&
           value.hit_by_area(_plate_serial))
        {
            hit_result result = value.take_hit(shock);
            static_cast<void>(result);
            _spawn_effect(value.position());
        }
    }
}

void game::_update_flicker()
{
    if(! _theme().plates)
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

void game::_handle_explosions()
{
    for(enemy& value : _enemies)
    {
        if(! value.take_explosion())
        {
            continue;
        }

        show_name_message(_messages, "", value.data().name, " used SELFDESTRUCT!");
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

void game::_handle_cut()
{
    const species_data& body = _player.body();
    bool grass_form = body.type_1 == pokemon_type::grass || body.type_2 == pokemon_type::grass;
    bn::fixed_point feet = _player.position() + bn::fixed_point(0, 4);

    if(! grass_form)
    {
        for(bn::fixed_point offset : { bn::fixed_point(-10, 0), bn::fixed_point(10, 0), bn::fixed_point(0, -10),
                                       bn::fixed_point(0, 10) })
        {
            bn::fixed_point probe = feet + offset;

            if(room::at(probe.x(), probe.y()) == room::cells::bush)
            {
                _messages.show("A GRASS POKEMON could CUT this bush.");
                return;
            }
        }

        return;
    }

    if(! _view.cut_bushes(feet, 8))
    {
        return;
    }

    _messages.show("DITTO used CUT!");
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

void game::_update_reward()
{
    floor_room& value = _floor[_room];
    bool pickup = value.reward == room_reward::journal || value.reward == room_reward::item;

    if(_reward_pickup || ! pickup || value.reward_taken || ! value.cleared)
    {
        return;
    }

    bool page = value.reward == room_reward::journal;
    bn::fixed_point position = _view.interior_center() + bn::fixed_point(0, _flute_room == _room ? 20 : 0);
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
        ++_journal_pages;
        _show_journal_page(_floor_number - 1);
        return;
    }

    if(_player.give_item(value.item, _messages))
    {
        _reward_pickup.reset();
        value.reward_taken = true;
    }
}

void game::_show_journal_page(int page)
{
    _set_world_visible(false);
    _overlay.show_black();

    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::string<32> title("LAB JOURNAL  PAGE ");
    title.append(bn::to_string<4>(page + 1));

    bn::vector<bn::sprite_ptr, 40> text;
    small.generate(0, -56, title, text);

    for(int line = 0; line < journal::lines_per_page; ++line)
    {
        big.generate(0, -20 + line * 18, journal::line(page, line), text);
    }

    small.generate(0, 70, "A: CLOSE", text);
    bn::core::update();

    while(! bn::keypad::a_pressed())
    {
        bn::core::update();
    }

    text.clear();
    _overlay.hide();
    _set_world_visible(true);
    bn::core::update();
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

    int count = bn::min(2 + _floor_number / 2 + _random.get_int(2), _enemies.max_size());

    for(int index = 0; index < count; ++index)
    {
        for(int attempt = 0; attempt < 20; ++attempt)
        {
            species_id id = floor_themes::pick_species(_theme(), _random.get_int(100));
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

    if(current_room.reward == room_reward::rare && ! current_room.reward_taken && ! _enemies.empty())
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
    _reward_pickup.reset();
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

    bn::string<32> held("HELD: ");
    held.append(_player.held_item() ? items::get(*_player.held_item()).name : "NOTHING");

    bn::string<32> pages("JOURNAL ");
    pages.append(bn::to_string<4>(_journal_pages));
    pages.append("/");
    pages.append(bn::to_string<4>(journal::page_count));

    bn::vector<bn::sprite_ptr, 32> text;
    big.generate(0, -66, floor_label(_floor_number), text);
    small.generate(0, -48, held, text);
    small.generate(0, -38, pages, text);
    small.generate(0, 74, "START: RESUME", text);
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
