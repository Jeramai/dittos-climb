#ifndef ENEMY_H
#define ENEMY_H

#include "bn_camera_ptr.h"
#include "bn_random.h"
#include "bn_sprite_ptr.h"

#include "projectiles.h"
#include "species.h"

class enemy
{

public:
    enemy(species_id id, const bn::fixed_point& position, const bn::camera_ptr& camera, bn::random& random);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random);

    [[nodiscard]] species_id id() const
    {
        return _id;
    }

    [[nodiscard]] const species_data& data() const
    {
        return species::get(_id);
    }

    [[nodiscard]] const bn::fixed_point& position() const
    {
        return _position;
    }

    [[nodiscard]] bool active() const
    {
        return _state != state::spawning && ! _underground;
    }

    void set_in_light(bool in_light)
    {
        _in_light = in_light;
    }

    [[nodiscard]] bool contains(const bn::fixed_point& point, int half_size) const;

    hit_result take_hit(const attack& hit);

    [[nodiscard]] bool dead() const
    {
        return _hp <= 0;
    }

    [[nodiscard]] bool dash_hits(const bn::fixed_point& point);

    [[nodiscard]] const attack& dash_attack() const
    {
        return _dash_attack;
    }

    [[nodiscard]] bool hit_by_area(int serial);

    void apply_status(status_effect effect);

    [[nodiscard]] bool take_explosion()
    {
        bool result = _exploded;
        _exploded = false;
        return result;
    }

    [[nodiscard]] attack explosion_attack() const
    {
        return combat::make_attack(move_id::selfdestruct, data().type_1, data().type_2);
    }

    [[nodiscard]] bool take_reveal()
    {
        bool result = _just_revealed;
        _just_revealed = false;
        return result;
    }

private:
    enum class state
    {
        spawning,
        moving,
        windup,
        dashing,
        recovering,
    };

    bn::sprite_ptr _sprite;
    bn::fixed_point _position;
    bn::fixed_point _attack_direction;
    species_id _id;
    state _state = state::spawning;
    int _state_frames = 40;
    int _hp;
    int _cooldowns[2];
    int _pending_move = 0;
    int _flash_frames = 0;
    int _walk_frames = 0;
    int _last_area_serial = -1;
    bool _dash_connected = false;
    bool _facing_left = false;
    bool _hidden = false;
    bool _underground = false;
    bool _exploded = false;
    bool _in_light = true;
    int _burrow_frames = 0;
    int _wobble_frames = 0;
    bool _was_hidden = false;
    bool _just_revealed = false;
    status_effect _status = status_effect::none;
    int _status_frames = 0;
    int _frame_counter = 0;
    attack _dash_attack;

    [[nodiscard]] move_id _move(int index) const;

    [[nodiscard]] bool _try_start_attack(const bn::fixed_point& target);

    void _execute(enemy_projectiles& projectiles, bn::random& random);

    [[nodiscard]] bool _update_status();

    void _update_hidden(const bn::fixed_point& target);

    [[nodiscard]] bool _update_burrow(const bn::fixed_point& target);

    [[nodiscard]] bn::fixed_point _movement(const bn::fixed_point& target, bn::random& random);

    void _walk(const bn::fixed_point& step);

    void _update_sprite(bool moving);
};

#endif
