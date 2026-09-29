#ifndef SNORLAX_BOSS_H
#define SNORLAX_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "boss.h"

class snorlax_boss final : public boss
{

public:
    snorlax_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool asleep() const final
    {
        return _state == state::asleep;
    }

    void wake(message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::asleep && _state != state::waking && _state != state::jumping;
    }

    [[nodiscard]] bn::optional<boss_area_hit> area_hit() const final;

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
    bn::fixed_point _jump_start;
    bn::fixed_point _jump_target;
    attack _slam_attack;
    attack _shockwave_attack;
    state _state = state::asleep;
    int _state_frames = 0;
    int _slam_timer = 120;
    int _walk_frames = 0;
    bool _rested = false;
    bool _landed_this_frame = false;

    void _land(enemy_projectiles& projectiles);

    void _update_sprite(bn::fixed height);
};

#endif
