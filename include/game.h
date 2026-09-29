#ifndef GAME_H
#define GAME_H

#include "bn_camera_ptr.h"
#include "bn_random.h"
#include "bn_regular_bg_ptr.h"
#include "bn_vector.h"

#include "enemy.h"
#include "hud.h"
#include "message_box.h"
#include "player.h"
#include "projectiles.h"

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
    bn::regular_bg_ptr _room_bg;
    message_box _messages;
    player _player;
    player_projectiles _player_projectiles;
    enemy_projectiles _enemy_projectiles;
    bn::vector<enemy, 6> _enemies;
    bn::vector<outline, 6> _outlines;
    bn::vector<effect, 10> _effects;
    hud _hud;
    bn::fixed_point _camera_position;
    int _wave = 0;
    int _waves_cleared = 0;
    int _wave_delay = 90;
    int _shake_frames = 0;
    int _last_recoil_serial = -1;

    void _update_play();

    void _handle_player_attacks();

    void _handle_enemy_attacks();

    [[nodiscard]] int _outline_below_player() const;

    void _update_outlines();

    void _spawn_wave();

    void _spawn_effect(const bn::fixed_point& position);

    void _update_effects();

    void _update_camera();

    void _pause();

    void _game_over();
};

#endif
