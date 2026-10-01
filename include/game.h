#ifndef GAME_H
#define GAME_H

#include "bn_camera_ptr.h"
#include "bn_optional.h"
#include "bn_random.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"

#include "boss.h"
#include "enemy.h"
#include "floor_map.h"
#include "floor_theme.h"
#include "hud.h"
#include "message_box.h"
#include "overlay.h"
#include "player.h"
#include "projectiles.h"
#include "room_view.h"
#include "save.h"

class game
{

public:
    game(bn::random& random, const save_data* saved);

    void run();

    [[nodiscard]] bool won() const
    {
        return _won;
    }

    [[nodiscard]] bool quit() const
    {
        return _quit;
    }

private:
    struct effect
    {
        bn::sprite_ptr sprite;
        int frames;
    };

    struct damage_number
    {
        bn::vector<bn::sprite_ptr, 2> sprites;
        int frames;
    };

    struct particle
    {
        bn::sprite_ptr sprite;
        bn::fixed_point velocity;
        int frames;
    };

    struct falling_ember
    {
        bn::sprite_ptr marker;
        int frames;
    };

    struct outline
    {
        bn::sprite_ptr sprite;
        species_id species;
        int frames;
        bool shiny;
    };

    bn::random& _random;
    bn::camera_ptr _camera;
    room_view _view;
    overlay _overlay;
    message_box _messages;
    player _player;
    player_projectiles _player_projectiles;
    enemy_projectiles _enemy_projectiles;
    bn::vector<enemy, 6> _enemies;
    bn::vector<outline, 6> _outlines;
    bn::vector<effect, 10> _effects;
    bn::vector<damage_number, 6> _damage_numbers;
    bn::vector<particle, 10> _particles;
    int _particle_timer = 0;
    bn::unique_ptr<boss> _boss;
    bn::optional<bn::sprite_ptr> _flute_pickup;
    bn::optional<bn::sprite_ptr> _light;
    bn::optional<bn::sprite_ptr> _reward_pickup;
    bn::optional<bn::sprite_ptr> _mart_counter;
    bn::vector<falling_ember, 4> _embers;
    hud _hud;
    floor_map _floor;
    bn::fixed_point _camera_position;
    int _floor_number = 1;
    int _room = 0;
    int _previous_room = -1;
    int _spawn_delay = 0;
    int _waves_left = 0;
    int _shake_frames = 0;
    int _hit_stop = 0;
    int _last_recoil_serial = -1;
    int _plate_timer = 0;
    int _plate_serial = -1000;
    int _flicker_timer = 400;
    int _flicker_frames = 0;
    int _journal_mask = 0;
    int _run_frames = 0;
    int _defeated = 0;
    int _shinies = 0;
    int _coins_earned = 0;
    unsigned _run_forms[3] = {};
    int _ember_timer = 60;
    int _flute_room = -1;
    bool _locked = false;
    bool _has_flute = false;
    bool _has_silph_scope = false;
    bool _on_warp = false;
    bool _won = false;
    bool _boss_defeated = false;
    bool _quit = false;
    bool _magikarp_at_entry = false;
    bool _boss_waiting = false;

    [[nodiscard]] const floor_room& _current_room() const
    {
        return _floor[_room];
    }

    [[nodiscard]] const floor_theme& _theme() const
    {
        return floor_themes::get(_floor_number);
    }

    void _start_floor(const save_data* saved = nullptr);

    void _enter_room(int index, bn::optional<direction> entered_from);

    void _update_play();

    void _update_room_state();

    void _handle_player_attacks();

    void _handle_enemy_attacks();

    void _update_boss();

    void _spawn_boss();

    void _show_boss_card();

    void _show_credits();

    void _start_boss_fight();

    void _handle_cut();

    void _update_darkness(bool room_changed);

    void _update_struggle_check();

    void _update_warps();

    void _ending();

    void _update_plates();

    void _update_flicker();

    void _update_ember_rain();

    void _handle_explosions();

    [[nodiscard]] status_effect _roll_status(move_id move);

    void _after_player_hit(const attack& hit, const hit_result& result, enemy* target, const bn::fixed_point& where);

    void _spawn_damage_number(const bn::fixed_point& where, int damage, int effectiveness);

    void _update_damage_numbers();

    void _update_particles();

    void _update_flute();

    void _update_reward();

    void _collect_reward();

    void _show_journal_page(int page);

    void _generate_journal_page(int page, bn::ivector<bn::sprite_ptr>& text);

    void _read_journal();

    void _spawn_outline(species_id id, const bn::fixed_point& position, bool shiny = false);

    [[nodiscard]] int _outline_below_player() const;

    void _update_outlines();

    void _spawn_enemies();

    void _spawn_effect(const bn::fixed_point& position);

    void _update_effects();

    void _update_camera(bool snap);

    void _change_room(direction side);

    void _climb_stairs();

    void _fall_into_pit();

    void _fade(bool out);

    void _clear_room_objects();

    void _set_world_visible(bool visible);

    void _pause_map();

    void _save_and_quit();

    void _wait_for_a(int min_frames);

    [[nodiscard]] int _journal_page_count() const
    {
        int count = 0;

        for(int mask = _journal_mask; mask; mask &= mask - 1)
        {
            ++count;
        }

        return count;
    }

    void _register_form(species_id id, bool shiny);

    void _show_run_stats(const char* title);

    void _add_coins(int amount);

    [[nodiscard]] bool _near_mart_counter() const;

    void _open_mart();

    void _game_over();
};

#endif
