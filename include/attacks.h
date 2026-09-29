#ifndef ATTACKS_H
#define ATTACKS_H

#include "bn_math.h"

#include "bn_sprite_items_projectiles.h"
#include "bn_sprite_items_slash.h"

#include "projectiles.h"

namespace attacks
{
    constexpr int shot_half_size = 5;
    constexpr int melee_half_size = 11;
    constexpr int melee_reach = 12;

    [[nodiscard]] inline bn::fixed screen_degrees(const bn::fixed_point& direction)
    {
        bn::fixed degrees = bn::degrees_atan2((-direction.y() * 256).round_integer(),
                                              (direction.x() * 256).round_integer());
        return degrees < 0 ? degrees + 360 : degrees;
    }

    [[nodiscard]] inline bn::fixed_point rotate(const bn::fixed_point& direction, int degrees)
    {
        bn::fixed angle = degrees < 0 ? degrees + 360 : degrees;
        bn::pair<bn::fixed, bn::fixed> sin_cos = bn::degrees_lut_sin_and_cos(angle);
        return bn::fixed_point(direction.x() * sin_cos.second + direction.y() * sin_cos.first,
                               direction.y() * sin_cos.second - direction.x() * sin_cos.first);
    }

    template<int MaxSize>
    void shoot(projectile_pool<MaxSize>& pool, const attack& hit, const bn::fixed_point& origin,
               const bn::fixed_point& direction, bn::fixed speed_scale)
    {
        const move_data& data = moves::get(hit.move);
        int first_degrees = -data.spread_degrees * (data.shots - 1) / 2;

        for(int index = 0; index < data.shots; ++index)
        {
            bn::fixed_point shot_direction = rotate(direction, first_degrees + index * data.spread_degrees);
            pool.spawn(bn::sprite_items::projectiles.create_sprite(origin, data.projectile_frame), origin,
                       shot_direction * data.speed * speed_scale, hit, data.life, shot_half_size, true);
        }
    }

    template<int MaxSize>
    void slash(projectile_pool<MaxSize>& pool, const attack& hit, const bn::fixed_point& origin,
               const bn::fixed_point& direction)
    {
        bn::fixed_point position = origin + direction * melee_reach;
        bn::sprite_ptr sprite = bn::sprite_items::slash.create_sprite(position);
        sprite.set_rotation_angle(screen_degrees(direction));
        pool.spawn(bn::move(sprite), position, bn::fixed_point(), hit, moves::get(hit.move).life, melee_half_size,
                   false);
    }
}

#endif
