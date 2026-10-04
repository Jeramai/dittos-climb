#ifndef TEAM_ROCKET_BOSS_H
#define TEAM_ROCKET_BOSS_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"

#include "boss.h"

class team_rocket_boss final : public boss
{

public:
    team_rocket_boss(const bn::fixed_point& position, const bn::camera_ptr& camera);

    void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                message_box& messages) final;

    [[nodiscard]] const char* name() const final
    {
        return "TEAM ROCKET";
    }

    void announce_defeat(message_box& messages) const final;

    [[nodiscard]] bn::optional<species_id> extra_outline() const final
    {
        return species_id::weezing;
    }

    [[nodiscard]] bool vulnerable() const final
    {
        return _intro_frames == 0;
    }

    [[nodiscard]] bool contains(const bn::fixed_point& point, int half_size) const final;

    [[nodiscard]] bool touches(const bn::fixed_point& point) const final;

    [[nodiscard]] attack contact_attack() const final
    {
        return _wrap;
    }

    void set_test_hp(int hp) final;

    hit_result take_hit(const attack& hit, const bn::fixed_point& point, int half_size) final;

    [[nodiscard]] bn::fixed_point last_hit_position() const final
    {
        return _last_hit_position;
    }

private:
    bn::sprite_ptr _arbok;
    bn::sprite_ptr _weezing;
    bn::sprite_ptr _balloon;
    bn::fixed_point _weezing_position;
    bn::fixed_point _balloon_anchor;
    bn::fixed_point _wrap_direction;
    bn::fixed_point _last_hit_position;
    attack _poison_sting;
    attack _wrap;
    attack _sludge;
    attack _smog;
    attack _pay_day;
    int _intro_frames;
    int _sting_timer;
    int _wrap_timer;
    int _wrap_frames = 0;
    int _windup_frames = 0;
    int _sludge_timer;
    int _smog_timer;
    int _coin_timer;
    int _frame_counter = 0;
    int _arbok_hp;
    int _weezing_hp;
    int _arbok_flash = 0;
    int _weezing_flash = 0;
    bool _arbok_fainted = false;
    bool _weezing_fainted = false;
    bool _desperate = false;

    [[nodiscard]] int _scaled(int frames) const;

    void _move_weezing(const bn::fixed_point& target);

    void _update_sprites();
};

#endif
