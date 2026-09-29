#ifndef MOLTRES_BOSS_H
#define MOLTRES_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "boss.h"

class moltres_boss final : public boss
{

public:
    moltres_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::intro;
    }

    [[nodiscard]] bool touches(const bn::fixed_point& point) const final;

    [[nodiscard]] attack contact_attack() const final
    {
        return _sky_attack;
    }

private:
    enum class state
    {
        intro,
        roaming,
        charging,
        sweeping,
    };

    bn::sprite_ptr _sprite;
    bn::fixed_point _direction;
    attack _flamethrower;
    attack _fire_spin;
    attack _sky_attack;
    state _state = state::intro;
    int _state_frames;
    int _orbit_degrees = 90;
    int _flame_timer;
    int _spin_timer;
    int _sky_timer;
    int _walk_frames = 0;
    bool _hotter = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _update_sprite();
};

#endif
