#ifndef ROOM_H
#define ROOM_H

#include "bn_fixed_point.h"

#include "room_data.h"

namespace room
{
    constexpr int tile_size = 8;
    constexpr int pixel_width = room_data::width * tile_size;
    constexpr int pixel_height = room_data::height * tile_size;

    [[nodiscard]] bool is_solid(bn::fixed x, bn::fixed y);

    [[nodiscard]] bool area_is_blocked(bn::fixed left, bn::fixed top, bn::fixed right, bn::fixed bottom);

    [[nodiscard]] bool feet_are_blocked(const bn::fixed_point& position);

    [[nodiscard]] bn::fixed_point clamp_camera(const bn::fixed_point& position);
}

#endif
