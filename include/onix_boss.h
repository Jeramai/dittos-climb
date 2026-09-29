#ifndef ONIX_BOSS_H
#define ONIX_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "boss.h"

class onix_boss final : public boss
{

public:
    onix_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::emerging;
    }

    [[nodiscard]] bool touches(const bn::fixed_point& point) const final;

    [[nodiscard]] bool blocks(const bn::fixed_point& point, int half_size) const final;

    [[nodiscard]] attack contact_attack() const final;

private:
    enum class state
    {
        emerging,
        roaming,
        windup,
        charging,
        stunned,
    };

    static constexpr int segment_count = 6;
    static constexpr int history_size = 64;

    bn::sprite_ptr _head;
    bn::vector<bn::sprite_ptr, segment_count> _segments;
    bn::vector<bn::sprite_ptr, 5> _slide_markers;
    bn::camera_ptr _camera;
    bn::fixed_point _history[history_size];
    int _history_index = 0;
    bn::fixed _heading = 270;
    attack _rock_throw;
    attack _rock_slide;
    state _state = state::emerging;
    int _state_frames;
    int _charge_timer;
    int _throw_timer;
    int _slide_timer;
    int _slide_frames = 0;

    [[nodiscard]] bn::fixed_point _direction() const;

    void _turn_toward(const bn::fixed_point& target, bn::fixed max_degrees);

    void _record_history();

    [[nodiscard]] const bn::fixed_point& _segment_position(int index) const;

    void _start_slide(const bn::fixed_point& target, bn::random& random);

    void _drop_slide(enemy_projectiles& projectiles);

    void _update_sprites();
};

#endif
