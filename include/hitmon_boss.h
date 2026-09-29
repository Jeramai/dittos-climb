#ifndef HITMON_BOSS_H
#define HITMON_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "boss.h"

class hitmon_boss final : public boss
{

public:
    hitmon_boss(const bn::fixed_point& position, const bn::camera_ptr& camera, bool kicker);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::intro;
    }

    [[nodiscard]] bool touches(const bn::fixed_point& point) const final;

    [[nodiscard]] attack contact_attack() const final
    {
        return _dash_attack;
    }

    [[nodiscard]] species_id outline_species() const final
    {
        return _kicker ? species_id::hitmonchan : species_id::hitmonlee;
    }

    void announce_defeat(message_box& messages) const final;

private:
    enum class state
    {
        intro,
        roaming,
        windup,
        dashing,
        stunned,
        countering,
    };

    bn::sprite_ptr _sprite;
    bn::fixed_point _direction;
    attack _dash_attack;
    attack _kick;
    attack _punches[3];
    attack _shockwave;
    bool _kicker;
    state _state = state::intro;
    int _state_frames;
    int _strike_timer;
    int _dash_timer;
    int _counter_timer;
    int _combo_left = 0;
    int _combo_index = 0;
    int _walk_frames = 0;
    int _hp_at_stance = 0;
    bool _furious = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _crash(message_box& messages);

    void _counter(enemy_projectiles& projectiles, message_box& messages);

    void _update_sprite();
};

#endif
