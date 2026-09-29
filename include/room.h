#ifndef ROOM_H
#define ROOM_H

#include "bn_fixed_point.h"
#include "bn_optional.h"

namespace room
{
    constexpr int tile_size = 8;
    constexpr int columns = 64;
    constexpr int rows = 32;
    constexpr int pixel_width = columns * tile_size;
    constexpr int pixel_height = rows * tile_size;

    namespace cells
    {
        constexpr char empty = ' ';
        constexpr char wall = '#';
        constexpr char floor = '.';
        constexpr char door = 'D';
        constexpr char stairs = 'S';
        constexpr char grass = '"';
        constexpr char bush = 'B';
        constexpr char water = '~';
        constexpr char flow_right = '>';
        constexpr char flow_left = '<';
        constexpr char flow_down = 'v';
        constexpr char flow_up = '^';
        constexpr char plate = 'Z';
    }

    void clear();

    void set(int column, int row, char value);

    [[nodiscard]] char get(int column, int row);

    [[nodiscard]] char at(bn::fixed x, bn::fixed y);

    [[nodiscard]] bool is_water(char value);

    [[nodiscard]] bool is_solid(bn::fixed x, bn::fixed y, bool can_swim = false);

    [[nodiscard]] bool blocks_projectiles(bn::fixed x, bn::fixed y);

    [[nodiscard]] bool area_is_blocked(bn::fixed left, bn::fixed top, bn::fixed right, bn::fixed bottom,
                                       bool can_swim = false);

    [[nodiscard]] bool feet_are_blocked(const bn::fixed_point& position, bool can_swim = false);

    [[nodiscard]] bool feet_in_water(const bn::fixed_point& position);

    [[nodiscard]] bn::fixed_point flow_at(const bn::fixed_point& position);

    [[nodiscard]] bn::optional<bn::fixed_point> nearest_standable(const bn::fixed_point& position, bool can_swim);

    [[nodiscard]] bn::fixed_point cell_center(int column, int row);

    void set_camera_bounds(int left_column, int top_row, int right_column, int bottom_row);

    [[nodiscard]] bn::fixed_point clamp_camera(const bn::fixed_point& position);
}

#endif
