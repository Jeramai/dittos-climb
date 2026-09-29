#ifndef PIDGEOT_BOSS_H
#define PIDGEOT_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "boss.h"

class pidgeot_boss final : public boss
{

public:
    pidgeot_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::intro;
    }

    [[nodiscard]] bool touches(const bn::fixed_point& point) const final;

    [[nodiscard]] bn::fixed_point wind() const final
    {
        return _gust_frames ? _gust : bn::fixed_point();
    }

    [[nodiscard]] attack contact_attack() const final
    {
        return _wing_attack;
    }

private:
    enum class state
    {
        intro,
        roaming,
        charging,
        diving,
    };

    bn::sprite_ptr _sprite;
    bn::fixed_point _direction;
    bn::fixed_point _gust;
    attack _feathers;
    attack _whirlwind;
    attack _wing_attack;
    state _state = state::intro;
    int _state_frames;
    int _orbit_degrees = 90;
    int _feather_timer;
    int _whirlwind_timer;
    int _gust_timer;
    int _gust_frames = 0;
    int _dive_timer;
    int _walk_frames = 0;
    bool _furious = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _update_sprite();
};

#endif
