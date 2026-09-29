#ifndef MEWTWO_BOSS_H
#define MEWTWO_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "boss.h"

class mewtwo_boss final : public boss
{

public:
    mewtwo_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _intro_frames == 0 && _barrier_frames == 0;
    }

    void announce_defeat(message_box& messages) const final;

private:
    bn::sprite_ptr _sprite;
    attack _confusion;
    attack _psywave;
    attack _swift;
    int _intro_frames;
    int _barrier_frames = 0;
    int _fan_timer;
    int _wave_timer;
    int _barrier_timer;
    int _teleport_timer;
    int _swift_timer;
    int _frame_counter = 0;
    bool _rising = false;
    bool _recovered = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _update_sprite();
};

#endif
