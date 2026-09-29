#ifndef ROOM_LAYOUTS_H
#define ROOM_LAYOUTS_H

struct obstacle
{
    int x;
    int y;
    int width;
    int height;
};

struct room_layout
{
    int width;
    int height;
    int obstacle_count;
    obstacle obstacles[8];
};

namespace room_layouts
{
    constexpr int wall_top = 3;
    constexpr int wall_bottom = 2;
    constexpr int wall_side = 2;
    constexpr int door_width = 4;

    constexpr room_layout combat[] = {
        { 26, 15, 0, {} },
        { 40, 19, 4, { { 8, 4, 2, 2 }, { 30, 4, 2, 2 }, { 8, 13, 2, 2 }, { 30, 13, 2, 2 } } },
        { 56, 24, 8, { { 10, 6, 2, 2 }, { 22, 6, 2, 2 }, { 34, 6, 2, 2 }, { 44, 6, 2, 2 },
                       { 10, 16, 2, 2 }, { 22, 16, 2, 2 }, { 34, 16, 2, 2 }, { 44, 16, 2, 2 } } },
        { 30, 24, 1, { { 12, 9, 6, 6 } } },
        { 48, 16, 2, { { 10, 5, 10, 2 }, { 28, 9, 10, 2 } } },
    };

    constexpr int combat_count = sizeof(combat) / sizeof(combat[0]);

    constexpr room_layout start = { 26, 15, 0, {} };
    constexpr room_layout stairs = { 26, 15, 0, {} };
    constexpr room_layout center = { 26, 13, 0, {} };

    constexpr int center_counter_x = 8;
    constexpr int center_counter_width = 10;
    constexpr int center_counter_y = 2;
    constexpr int center_pc_x = 21;
    constexpr int center_pc_y = 1;
}

#endif
