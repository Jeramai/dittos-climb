#include "room.h"

#include "bn_display.h"
#include "bn_math.h"

namespace room
{

namespace
{
    char grid[rows][columns];
    bn::fixed camera_min_x;
    bn::fixed camera_max_x;
    bn::fixed camera_min_y;
    bn::fixed camera_max_y;

    [[nodiscard]] bool solid_cell(char value)
    {
        return value != cells::floor && value != cells::stairs && value != cells::grass;
    }

    void set_axis(int first, int last, int screen_size, bn::fixed& min, bn::fixed& max, int map_size)
    {
        bn::fixed low = first * tile_size - map_size / 2 + screen_size / 2;
        bn::fixed high = (last + 1) * tile_size - map_size / 2 - screen_size / 2;

        if(low > high)
        {
            low = (low + high) / 2;
            high = low;
        }

        min = low;
        max = high;
    }
}

void clear()
{
    for(auto& row : grid)
    {
        for(char& value : row)
        {
            value = cells::empty;
        }
    }
}

void set(int column, int row, char value)
{
    grid[row][column] = value;
}

char get(int column, int row)
{
    if(column < 0 || row < 0 || column >= columns || row >= rows)
    {
        return cells::empty;
    }

    return grid[row][column];
}

char at(bn::fixed x, bn::fixed y)
{
    int column = (x.floor_integer() + pixel_width / 2) / tile_size;
    int row = (y.floor_integer() + pixel_height / 2) / tile_size;
    return get(column, row);
}

bool is_solid(bn::fixed x, bn::fixed y)
{
    return solid_cell(at(x, y));
}

bool area_is_blocked(bn::fixed left, bn::fixed top, bn::fixed right, bn::fixed bottom)
{
    return is_solid(left, top) || is_solid(right, top) || is_solid(left, bottom) || is_solid(right, bottom);
}

bool feet_are_blocked(const bn::fixed_point& position)
{
    bn::fixed x = position.x();
    bn::fixed y = position.y();
    return area_is_blocked(x - 5, y + 2, x + 4, y + 7);
}

bn::fixed_point cell_center(int column, int row)
{
    return bn::fixed_point(column * tile_size - pixel_width / 2 + tile_size / 2,
                           row * tile_size - pixel_height / 2 + tile_size / 2);
}

void set_camera_bounds(int left_column, int top_row, int right_column, int bottom_row)
{
    set_axis(left_column, right_column, bn::display::width(), camera_min_x, camera_max_x, pixel_width);
    set_axis(top_row, bottom_row, bn::display::height(), camera_min_y, camera_max_y, pixel_height);
}

bn::fixed_point clamp_camera(const bn::fixed_point& position)
{
    return bn::fixed_point(bn::clamp(position.x(), camera_min_x, camera_max_x),
                           bn::clamp(position.y(), camera_min_y, camera_max_y));
}

}
