#include "room.h"

#include "bn_display.h"

namespace room
{

bool is_solid(bn::fixed x, bn::fixed y)
{
    int tile_x = (x.floor_integer() + pixel_width / 2) / tile_size;
    int tile_y = (y.floor_integer() + pixel_height / 2) / tile_size;

    if(tile_x < 0 || tile_y < 0 || tile_x >= room_data::width || tile_y >= room_data::height)
    {
        return true;
    }

    return room_data::rows[tile_y][tile_x] == '#';
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

bn::fixed_point clamp_camera(const bn::fixed_point& position)
{
    constexpr int max_x = (pixel_width - bn::display::width()) / 2;
    constexpr int max_y = (pixel_height - bn::display::height()) / 2;
    return bn::fixed_point(bn::clamp(position.x(), bn::fixed(-max_x), bn::fixed(max_x)),
                           bn::clamp(position.y(), bn::fixed(-max_y), bn::fixed(max_y)));
}

}
