#ifndef ARTICUNO_BOSS_H
#define ARTICUNO_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "boss.h"

class articuno_boss final : public boss
{

public:
    articuno_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

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
        beam_charge,
        charging,
        sweeping,
    };

    bn::sprite_ptr _sprite;
    bn::camera_ptr _camera;
    bn::vector<bn::sprite_ptr, 3> _beam_markers;
    bn::fixed_point _direction;
    attack _ice_shard;
    attack _blizzard;
    attack _ice_beam;
    attack _sky_attack;
    state _state = state::intro;
    int _state_frames;
    int _orbit_degrees = 90;
    int _shard_timer;
    int _burst_left = 0;
    int _blizzard_timer;
    int _beam_timer;
    int _sky_timer;
    int _walk_frames = 0;
    bool _furious = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _update_sprite();
};

#endif
