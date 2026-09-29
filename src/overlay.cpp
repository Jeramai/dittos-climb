#include "overlay.h"

#include "bn_bg_tiles.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_map_item.h"

#include "bn_bg_palette_items_overlay_palette.h"
#include "bn_regular_bg_tiles_items_overlay_tiles.h"

#include "floor_map.h"

namespace
{
    constexpr int columns = 32;
    constexpr int rows = 32;
    constexpr int map_left = 9;
    constexpr int map_top = 11;
    constexpr int cell_step = 3;

    namespace tiles
    {
        constexpr int black = 1;
        constexpr int known = 2;
        constexpr int visited = 6;
        constexpr int current = 10;
        constexpr int stairs = 14;
        constexpr int center = 18;
        constexpr int connector_horizontal = 22;
        constexpr int connector_vertical = 23;
    }

    alignas(int) bn::regular_bg_map_cell map_cells[columns * rows];

    const bn::regular_bg_map_item map_item(map_cells[0], bn::size(columns, rows));

    bn::regular_bg_ptr create_bg()
    {
        bn::bg_tiles::set_allow_offset(false);
        bn::regular_bg_item item(bn::regular_bg_tiles_items::overlay_tiles, bn::bg_palette_items::overlay_palette,
                                 map_item);
        bn::regular_bg_ptr result = item.create_bg(0, 0);
        bn::bg_tiles::set_allow_offset(true);
        result.set_priority(0);
        result.set_visible(false);
        return result;
    }
}

overlay::overlay() :
    _bg(create_bg()),
    _bg_map(_bg.map())
{
}

void overlay::show_black()
{
    for(int row = 0; row < rows; ++row)
    {
        for(int column = 0; column < columns; ++column)
        {
            _set(column, row, tiles::black);
        }
    }

    _bg_map.reload_cells_ref();
    _bg.set_visible(true);
}

void overlay::show_map(const floor_map& floor, int current_room)
{
    for(int row = 0; row < rows; ++row)
    {
        for(int column = 0; column < columns; ++column)
        {
            _set(column, row, tiles::black);
        }
    }

    for(int index = 0; index < floor.size(); ++index)
    {
        if(! floor.known(index))
        {
            continue;
        }

        const floor_room& value = floor[index];
        int first_tile = tiles::known;

        if(index == current_room)
        {
            first_tile = tiles::current;
        }
        else if(value.visited)
        {
            first_tile = value.kind == room_kind::stairs ? tiles::stairs :
                         value.kind == room_kind::center ? tiles::center : tiles::visited;
        }

        int column = map_left + value.x * cell_step;
        int row = map_top + value.y * cell_step;
        _set(column, row, first_tile);
        _set(column + 1, row, first_tile + 1);
        _set(column, row + 1, first_tile + 2);
        _set(column + 1, row + 1, first_tile + 3);

        if(! value.visited)
        {
            continue;
        }

        int east = floor.neighbor(index, direction::east);
        int south = floor.neighbor(index, direction::south);

        if(east >= 0)
        {
            _set(column + 2, row, tiles::connector_horizontal);
        }

        if(south >= 0)
        {
            _set(column, row + 2, tiles::connector_vertical);
        }

        int west = floor.neighbor(index, direction::west);
        int north = floor.neighbor(index, direction::north);

        if(west >= 0)
        {
            _set(column - 1, row, tiles::connector_horizontal);
        }

        if(north >= 0)
        {
            _set(column, row - 1, tiles::connector_vertical);
        }
    }

    _bg_map.reload_cells_ref();
    _bg.set_visible(true);
}

void overlay::hide()
{
    _bg.set_visible(false);
}

void overlay::_set(int column, int row, int tile)
{
    bn::regular_bg_map_cell_info info;
    info.set_tile_index(tile);
    map_cells[map_item.cell_index(column, row)] = info.cell();
}
