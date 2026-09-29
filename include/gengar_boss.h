#ifndef GENGAR_BOSS_H
#define GENGAR_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "boss.h"

class gengar_boss final : public boss
{

public:
    gengar_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] bool vulnerable() const final
    {
        return _state != state::intro && _state != state::faded;
    }

    void set_revealed(bool revealed) final
    {
        _revealed = revealed;
    }

private:
    enum class state
    {
        intro,
        roaming,
        fading_out,
        faded,
        fading_in,
    };

    bn::sprite_ptr _sprite;
    attack _lick;
    attack _shadow_ball;
    attack _hypnosis;
    attack _dream_eater;
    state _state = state::intro;
    int _state_frames;
    int _ball_timer;
    int _fade_timer;
    int _hypnosis_timer;
    int _dream_frames = 0;
    int _walk_frames = 0;
    bool _revealed = true;
    bool _laughing = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _update_sprite();
};

#endif
