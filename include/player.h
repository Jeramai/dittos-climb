#ifndef PLAYER_H
#define PLAYER_H

#include "bn_camera_ptr.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"

#include "items.h"
#include "message_box.h"
#include "projectiles.h"
#include "species.h"

struct form
{
    species_id species;
    int hp;
    int pp_b;
};

class player
{

public:
    static constexpr int form_hp_scale = 2;

    player(const bn::camera_ptr& camera, const bn::fixed_point& position);

    [[nodiscard]] bool update(player_projectiles& projectiles, message_box& messages,
                              const species_id* outline_below);

    [[nodiscard]] const bn::fixed_point& position() const
    {
        return _position;
    }

    [[nodiscard]] bool vulnerable() const;

    [[nodiscard]] bool transforming() const
    {
        return _transform_frames;
    }

    [[nodiscard]] const species_data& body() const;

    [[nodiscard]] const form* active_form() const;

    [[nodiscard]] int hp() const
    {
        return _hp;
    }

    [[nodiscard]] bool fainted() const
    {
        return _hp <= 0;
    }

    [[nodiscard]] int max_hp() const;

    [[nodiscard]] const bn::optional<item_id>& held_item() const
    {
        return _held;
    }

    [[nodiscard]] bool give_item(item_id id, message_box& messages);

    [[nodiscard]] bool area_active() const
    {
        return _area_frames;
    }

    [[nodiscard]] const attack& area_attack() const
    {
        return _area_attack;
    }

    [[nodiscard]] int area_serial() const
    {
        return _area_serial;
    }

    [[nodiscard]] int area_half_size() const
    {
        return _area_half_size;
    }

    [[nodiscard]] hit_result take_hit(const attack& hit, message_box& messages);

    void apply_status(status_effect effect, message_box& messages);

    [[nodiscard]] status_effect status() const
    {
        return _status;
    }

    void heal(int amount);

    void recoil(message_box& messages);

    void start_transform(species_id target);

    void evolve(species_id target);

    void set_position(const bn::fixed_point& position);


    void set_visible(bool visible);

private:
    bn::camera_ptr _camera;
    bn::sprite_ptr _sprite;
    bn::optional<bn::sprite_ptr> _wave_sprite;
    bn::fixed_point _position;
    bn::optional<form> _form;
    int _hp;
    int _aim = 0;
    int _cooldown = 0;
    int _dodge_frames = 0;
    int _dodge_cooldown = 0;
    int _dodge_direction = 0;
    int _invulnerable_frames = 0;
    int _walk_frames = 0;
    int _dash_frames = 0;
    bool _digging = false;
    bool _self_destructing = false;
    bn::fixed_point _dash_velocity;
    int _area_frames = 0;
    int _area_serial = 0;
    int _area_half_size = 0;
    attack _area_attack;
    int _transform_frames = 0;
    species_id _transform_target = species_id::ditto;
    bn::optional<species_id> _evolving_from;
    int _switch_flash_frames = 0;
    status_effect _status = status_effect::none;
    int _status_frames = 0;
    int _poison_timer = 0;
    int _charge_frames = 0;
    attack _charge_attack;
    int _frame_counter = 0;
    int _bonus_hp = 0;
    bn::optional<item_id> _held;

    void _use_move(bool move_a, player_projectiles& projectiles, message_box& messages);

    void _finish_transform(message_box& messages);

    void _move(const bn::fixed_point& delta);

    void _update_water(message_box& messages);

    void _start_area(const attack& hit, int frames, int half_size);

    void _update_wave();

    [[nodiscard]] bool _update_status(message_box& messages);

    void _lose_hp(int amount);

    void _faint_form(message_box& messages);

    void _update_sprite(bool moving);
};

#endif
