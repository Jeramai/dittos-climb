#ifndef VENUSAUR_BOSS_H
#define VENUSAUR_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "boss.h"

class venusaur_boss final : public boss
{

public:
    venusaur_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _intro_frames == 0;
    }

private:
    bn::sprite_ptr _sprite;
    bn::camera_ptr _camera;
    bn::vector<bn::sprite_ptr, 3> _beam_markers;
    bn::fixed_point _beam_direction;
    attack _razor_leaf;
    attack _sleep_powder;
    attack _solar_beam;
    int _intro_frames;
    int _razor_timer;
    int _powder_timer;
    int _beam_timer;
    int _charge_frames = 0;
    int _walk_frames = 0;
    bool _enraged = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _update_sprite();
};

#endif
