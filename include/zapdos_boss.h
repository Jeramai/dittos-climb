#ifndef ZAPDOS_BOSS_H
#define ZAPDOS_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "boss.h"

class zapdos_boss final : public boss
{

public:
    zapdos_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::intro;
    }

    [[nodiscard]] bool touches(const bn::fixed_point& point) const final;

    [[nodiscard]] attack contact_attack() const final
    {
        return _drill_peck;
    }

private:
    enum class state
    {
        intro,
        roaming,
        windup,
        pecking,
    };

    bn::sprite_ptr _sprite;
    bn::camera_ptr _camera;
    bn::vector<bn::sprite_ptr, 4> _thunder_markers;
    bn::fixed_point _direction;
    attack _thundershock;
    attack _thunder;
    attack _drill_peck;
    state _state = state::intro;
    int _state_frames;
    int _orbit_degrees = 0;
    int _burst_timer;
    int _burst_left = 0;
    int _peck_timer;
    int _thunder_timer;
    int _thunder_frames = 0;
    int _walk_frames = 0;
    bool _charged = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _start_thunder(const bn::fixed_point& target, bn::random& random);

    void _drop_thunder(enemy_projectiles& projectiles);

    void _update_sprite();
};

#endif
