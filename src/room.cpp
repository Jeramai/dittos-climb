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

    [[nodiscard]] bool solid_cell(char value, bool can_swim, bool over_pits)
    {
        if(is_water(value))
        {
            return ! can_swim;
        }

        if(value == cells::pit)
        {
            return ! over_pits;
        }

        return value != cells::floor && value != cells::stairs && value != cells::grass && value != cells::plate &&
               value != cells::ice && ! is_wind(value) && ! is_spinner(value);
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

bool is_water(char value)
{
    return value == cells::water || value == cells::flow_right || value == cells::flow_left ||
           value == cells::flow_down || value == cells::flow_up;
}

bool is_wind(char value)
{
    return value == cells::wind_east || value == cells::wind_west || value == cells::wind_south ||
           value == cells::wind_north;
}

bool is_spinner(char value)
{
    return value == cells::spin_right || value == cells::spin_left || value == cells::spin_down ||
           value == cells::spin_up;
}

bn::fixed_point spinner_at(const bn::fixed_point& position)
{
    switch(at(position.x(), position.y() + 4))
    {

    case cells::spin_right:
        return bn::fixed_point(1, 0);

    case cells::spin_left:
        return bn::fixed_point(-1, 0);

    case cells::spin_down:
        return bn::fixed_point(0, 1);

    case cells::spin_up:
        return bn::fixed_point(0, -1);

    default:
        return bn::fixed_point();
    }
}

bool is_solid(bn::fixed x, bn::fixed y, bool can_swim, bool over_pits)
{
    return solid_cell(at(x, y), can_swim, over_pits);
}

bool blocks_projectiles(bn::fixed x, bn::fixed y)
{
    return solid_cell(at(x, y), true, true);
}

bool area_is_blocked(bn::fixed left, bn::fixed top, bn::fixed right, bn::fixed bottom, bool can_swim,
                     bool over_pits)
{
    return is_solid(left, top, can_swim, over_pits) || is_solid(right, top, can_swim, over_pits) ||
           is_solid(left, bottom, can_swim, over_pits) || is_solid(right, bottom, can_swim, over_pits);
}

bool feet_are_blocked(const bn::fixed_point& position, bool can_swim, bool over_pits)
{
    bn::fixed x = position.x();
    bn::fixed y = position.y();
    return area_is_blocked(x - 5, y + 2, x + 4, y + 7, can_swim, over_pits);
}

bool feet_over_pit(const bn::fixed_point& position)
{
    return at(position.x(), position.y() + 4) == cells::pit;
}

bn::fixed_point wind_at(const bn::fixed_point& position)
{
    switch(at(position.x(), position.y() + 4))
    {

    case cells::wind_east:
        return bn::fixed_point(1, 0);

    case cells::wind_west:
        return bn::fixed_point(-1, 0);

    case cells::wind_south:
        return bn::fixed_point(0, 1);

    case cells::wind_north:
        return bn::fixed_point(0, -1);

    default:
        return bn::fixed_point();
    }
}

bool feet_in_water(const bn::fixed_point& position)
{
    return is_water(at(position.x(), position.y() + 4));
}

bn::fixed_point flow_at(const bn::fixed_point& position)
{
    switch(at(position.x(), position.y() + 4))
    {

    case cells::flow_right:
        return bn::fixed_point(1, 0);

    case cells::flow_left:
        return bn::fixed_point(-1, 0);

    case cells::flow_down:
        return bn::fixed_point(0, 1);

    case cells::flow_up:
        return bn::fixed_point(0, -1);

    default:
        return bn::fixed_point();
    }
}

bn::optional<bn::fixed_point> nearest_standable(const bn::fixed_point& position, bool can_swim)
{
    int origin_column = (position.x().floor_integer() + pixel_width / 2) / tile_size;
    int origin_row = (position.y().floor_integer() + pixel_height / 2) / tile_size;

    for(int radius = 1; radius <= 12; ++radius)
    {
        for(int dy = -radius; dy <= radius; ++dy)
        {
            for(int dx = -radius; dx <= radius; ++dx)
            {
                if(bn::abs(dx) != radius && bn::abs(dy) != radius)
                {
                    continue;
                }

                bn::fixed_point candidate = cell_center(origin_column + dx, origin_row + dy);

                if(! feet_are_blocked(candidate, can_swim))
                {
                    return candidate;
                }
            }
        }
    }

    return bn::nullopt;
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
