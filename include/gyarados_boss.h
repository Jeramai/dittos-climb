#ifndef GYARADOS_BOSS_H
#define GYARADOS_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "boss.h"

class gyarados_boss final : public boss
{

public:
    gyarados_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::magikarp && _state != state::evolving;
    }

    [[nodiscard]] bool touches(const bn::fixed_point& point) const final;

    [[nodiscard]] attack contact_attack() const final
    {
        return _bite;
    }

private:
    enum class state
    {
        magikarp,
        evolving,
        roaming,
        windup,
        lunging,
        charging,
    };

    bn::sprite_ptr _sprite;
    bn::camera_ptr _camera;
    bn::vector<bn::sprite_ptr, 3> _beam_markers;
    bn::fixed_point _direction;
    attack _bite;
    attack _hydro_pump;
    attack _dragon_rage;
    state _state = state::magikarp;
    int _state_frames;
    int _bite_timer;
    int _pump_timer;
    int _rage_timer;
    int _walk_frames = 0;
    bool _thrashing = false;

    void _evolve_step(message_box& messages);

    void _update_sprite();
};

#endif
