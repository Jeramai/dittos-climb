#ifndef SNORLAX_BOSS_H
#define SNORLAX_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_optional.h"
#include "bn_random.h"
#include "bn_sprite_ptr.h"

#include "message_box.h"
#include "projectiles.h"

class snorlax_boss
{

public:
    static constexpr int max_hp = 120;
    static constexpr int slam_radius = 24;

    snorlax_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void wake(message_box& messages);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages);

    [[nodiscard]] bool asleep() const
    {
        return _state == state::asleep;
    }

    [[nodiscard]] bool awake() const
    {
        return _state != state::asleep && _state != state::waking;
    }

    [[nodiscard]] const bn::fixed_point& position() const
    {
        return _position;
    }

    [[nodiscard]] int hp() const
    {
        return _hp;
    }

    [[nodiscard]] bool dead() const
    {
        return _hp <= 0;
    }

    [[nodiscard]] bool contains(const bn::fixed_point& point, int half_size) const;

    hit_result take_hit(const attack& hit);

    [[nodiscard]] bool hit_by_area(int serial);

    [[nodiscard]] bool landed_this_frame() const
    {
        return _landed_this_frame;
    }

    [[nodiscard]] const attack& slam_attack() const
    {
        return _slam_attack;
    }

private:
    enum class state
    {
        asleep,
        waking,
        walking,
        windup,
        jumping,
        landed,
        resting,
    };

    bn::sprite_ptr _sprite;
    bn::optional<bn::sprite_ptr> _marker;
    bn::camera_ptr _camera;
    bn::fixed_point _position;
    bn::fixed_point _jump_start;
    bn::fixed_point _jump_target;
    attack _slam_attack;
    attack _shockwave_attack;
    state _state = state::asleep;
    int _state_frames = 0;
    int _slam_timer = 120;
    int _hp = max_hp;
    int _flash_frames = 0;
    int _walk_frames = 0;
    int _last_area_serial = -1;
    bool _rested = false;
    bool _landed_this_frame = false;

    void _walk(const bn::fixed_point& target);

    void _land(enemy_projectiles& projectiles);

    void _update_sprite(bn::fixed height);
};

#endif
