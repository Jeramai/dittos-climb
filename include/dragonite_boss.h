#ifndef DRAGONITE_BOSS_H
#define DRAGONITE_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "boss.h"

class dragonite_boss final : public boss
{

public:
    dragonite_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::intro;
    }

    [[nodiscard]] bool touches(const bn::fixed_point& point) const final;

    [[nodiscard]] attack contact_attack() const final
    {
        return _outrage;
    }

private:
    enum class state
    {
        intro,
        roaming,
        windup,
        rampaging,
        charging,
        tired,
    };

    bn::sprite_ptr _sprite;
    bn::camera_ptr _camera;
    bn::vector<bn::sprite_ptr, 3> _beam_markers;
    bn::fixed_point _direction;
    attack _twister;
    attack _dragon_rage;
    attack _outrage;
    attack _hyper_beam;
    state _state = state::intro;
    int _state_frames;
    int _twister_timer;
    int _rage_timer;
    int _outrage_timer;
    int _beam_timer;
    int _lunges_left = 0;
    int _walk_frames = 0;
    bool _furious = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _update_sprite();
};

#endif
