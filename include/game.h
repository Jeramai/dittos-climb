#ifndef GAME_H
#define GAME_H

#include "bn_camera_ptr.h"
#include "bn_optional.h"
#include "bn_random.h"
#include "bn_vector.h"

#include "enemy.h"
#include "floor_map.h"
#include "hud.h"
#include "message_box.h"
#include "overlay.h"
#include "player.h"
#include "projectiles.h"
#include "room_view.h"

class game
{

public:
    explicit game(bn::random& random);

    void run();

private:
    struct effect
    {
        bn::sprite_ptr sprite;
        int frames;
    };

    struct outline
    {
        bn::sprite_ptr sprite;
        species_id species;
        int frames;
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
    bn::optional<bn::sprite_ptr> _chansey;
    hud _hud;
    floor_map _floor;
    bn::fixed_point _camera_position;
    int _floor_number = 1;
    int _room = 0;
    int _spawn_delay = 0;
    int _shake_frames = 0;
    int _last_recoil_serial = -1;
    unsigned _pokedex = 0;
    bool _locked = false;
    bool _center_healed = false;
    bool _pc_used = false;

    [[nodiscard]] const floor_room& _current_room() const
    {
        return _floor[_room];
    }

    void _start_floor();

    void _enter_room(int index, bn::optional<direction> entered_from);

    void _update_play();

    void _update_room_state();

    void _handle_player_attacks();

    void _handle_enemy_attacks();

    void _handle_center();

    [[nodiscard]] int _outline_below_player() const;

    void _update_outlines();

    void _spawn_enemies();

    void _spawn_effect(const bn::fixed_point& position);

    void _update_effects();

    void _update_camera(bool snap);

    void _change_room(direction side);

    void _climb_stairs();

    void _fade(bool out);

    void _clear_room_objects();

    void _set_world_visible(bool visible);

    void _pause_map();

    void _bills_pc();

    void _game_over();
};

#endif
