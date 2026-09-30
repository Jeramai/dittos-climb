#include "overlay.h"

#include "bn_bg_tiles.h"
#include "bn_math.h"
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
    constexpr int screen_left = 1;
    constexpr int screen_top = 6;
    constexpr int cell_step = 3;

    namespace tiles
    {
        constexpr int black = 1;
        constexpr int known = 2;
        constexpr int visited = 6;
        constexpr int current = 10;
        constexpr int stairs = 14;
        constexpr int connector_horizontal = 18;
        constexpr int connector_vertical = 19;
        constexpr int backdrop = 20;
        constexpr int windows = 24;
        constexpr int window_tiles = 9;
        constexpr int boss = 60;
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
    _fill(tiles::black);
}

void overlay::clear()
{
    _fill(0);
}

void overlay::show_backdrop()
{
    for(int row = 0; row < rows; ++row)
    {
        for(int column = 0; column < columns; ++column)
        {
            _set(column, row, tiles::backdrop + (column & 1) + (row & 1) * 2);
        }
    }

    _bg_map.reload_cells_ref();
    _bg.set_visible(true);
}

void overlay::window(int x, int y, int width, int height, window_style style)
{
    int first = tiles::windows + int(style) * tiles::window_tiles;

    for(int row = 0; row < height; ++row)
    {
        int band = row == 0 ? 0 : row == height - 1 ? 2 : 1;

        for(int column = 0; column < width; ++column)
        {
            int part = column == 0 ? 0 : column == width - 1 ? 2 : 1;
            _set(screen_left + x + column, screen_top + y + row, first + band * 3 + part);
        }
    }

    _bg_map.reload_cells_ref();
    _bg.set_visible(true);
}

void overlay::map(const floor_map& floor, int current_room, bool boss_alive, int x, int y, int width, int height)
{
    int min_x = floor_map::grid_size;
    int min_y = floor_map::grid_size;
    int max_x = -1;
    int max_y = -1;

    for(int index = 0; index < floor.size(); ++index)
    {
        if(floor.known(index))
        {
            min_x = bn::min(min_x, floor[index].x);
            min_y = bn::min(min_y, floor[index].y);
            max_x = bn::max(max_x, floor[index].x);
            max_y = bn::max(max_y, floor[index].y);
        }
    }

    int map_left = screen_left + x + (width - ((max_x - min_x) * cell_step + 2)) / 2 - min_x * cell_step;
    int map_top = screen_top + y + (height - ((max_y - min_y) * cell_step + 2)) / 2 - min_y * cell_step;

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
        else if(value.kind == room_kind::stairs && boss_alive)
        {
            first_tile = tiles::boss;
        }
        else if(value.visited)
        {
            first_tile = value.kind == room_kind::stairs ? tiles::stairs : tiles::visited;
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

        if(floor.neighbor(index, direction::east) >= 0)
        {
            _set(column + 2, row, tiles::connector_horizontal);
        }

        if(floor.neighbor(index, direction::south) >= 0)
        {
            _set(column, row + 2, tiles::connector_vertical);
        }

        if(floor.neighbor(index, direction::west) >= 0)
        {
            _set(column - 1, row, tiles::connector_horizontal);
        }

        if(floor.neighbor(index, direction::north) >= 0)
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

void overlay::_fill(int tile)
{
    for(int row = 0; row < rows; ++row)
    {
        for(int column = 0; column < columns; ++column)
        {
            _set(column, row, tile);
        }
    }

    _bg_map.reload_cells_ref();
    _bg.set_visible(true);
}

void overlay::_set(int column, int row, int tile)
{
    if(column < 0 || row < 0 || column >= columns || row >= rows)
    {
        return;
    }

    bn::regular_bg_map_cell_info info;
    info.set_tile_index(tile);
    map_cells[map_item.cell_index(column, row)] = info.cell();
}
