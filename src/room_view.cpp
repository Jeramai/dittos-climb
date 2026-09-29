#include "room_view.h"

#include "bn_bg_tiles.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_map_item.h"

#include "bn_bg_palette_items_lab_palette.h"
#include "bn_regular_bg_tiles_items_lab_tiles.h"

#include "room.h"

namespace
{
    namespace tiles
    {
        constexpr int empty = 0;
        constexpr int floor = 1;
        constexpr int floor_light = 2;
        constexpr int floor_shadow = 3;
        constexpr int wall_top = 4;
        constexpr int wall_face = 5;
        constexpr int door = 6;
        constexpr int stairs = 7;
        constexpr int center_floor = 11;
        constexpr int counter_top = 12;
        constexpr int counter_front = 13;
        constexpr int pc = 14;
        constexpr int floor_crack = 18;
    }

    alignas(int) bn::regular_bg_map_cell map_cells[room::columns * room::rows];

    const bn::regular_bg_map_item map_item(map_cells[0], bn::size(room::columns, room::rows));

    bn::regular_bg_ptr create_bg()
    {
        bn::bg_tiles::set_allow_offset(false);
        bn::regular_bg_item item(bn::regular_bg_tiles_items::lab_tiles, bn::bg_palette_items::lab_palette, map_item);
        bn::regular_bg_ptr result = item.create_bg(0, 0);
        bn::bg_tiles::set_allow_offset(true);
        return result;
    }

    bool walkable(char value)
    {
        return value == room::cells::floor || value == room::cells::stairs;
    }

    int quad_tile(int first_tile, int column, int row)
    {
        return first_tile + (column % 2) + (row % 2) * 2;
    }
}

room_view::room_view() :
    _bg(create_bg()),
    _bg_map(_bg.map()),
    _layout(room_layouts::start)
{
}

void room_view::build(const floor_room& value, const bool doors[4], bool locked)
{
    _kind = value.kind;

    switch(value.kind)
    {

    case room_kind::combat:
        _layout = room_layouts::combat[value.layout];
        break;

    case room_kind::center:
        _layout = room_layouts::center;
        break;

    case room_kind::stairs:
        _layout = room_layouts::stairs;
        break;

    default:
        _layout = room_layouts::start;
        break;
    }

    for(int side = 0; side < 4; ++side)
    {
        _doors[side] = doors[side];
    }

    _width = _layout.width + room_layouts::wall_side * 2;
    _height = _layout.height + room_layouts::wall_top + room_layouts::wall_bottom;
    _left = (room::columns - _width) / 2;
    _top = (room::rows - _height) / 2;

    room::clear();
    _fill(_left, _top, _width, _height, room::cells::wall);
    _fill(_interior_left(), _interior_top(), _layout.width, _layout.height, room::cells::floor);

    for(int index = 0; index < _layout.obstacle_count; ++index)
    {
        const obstacle& block = _layout.obstacles[index];
        _fill(_interior_left() + block.x, _interior_top() + block.y, block.width, block.height, room::cells::wall);
    }

    if(value.kind == room_kind::stairs)
    {
        _fill(_door_column() - 1, _door_row() - 1, 2, 2, room::cells::stairs);
    }
    else if(value.kind == room_kind::center)
    {
        _fill(_interior_left() + room_layouts::center_counter_x, _interior_top() + room_layouts::center_counter_y,
              room_layouts::center_counter_width, 2, room::cells::counter);
        _fill(_interior_left() + room_layouts::center_pc_x, _interior_top() + room_layouts::center_pc_y, 2, 2,
              room::cells::pc);
    }

    room::set_camera_bounds(_left, _top - 2, _left + _width - 1, _top + _height);
    set_locked(locked);
}

void room_view::set_locked(bool locked)
{
    _carve_doors(locked);
    _render();
}

void room_view::set_camera(const bn::camera_ptr& camera)
{
    _bg.set_camera(camera);
}

bn::fixed_point room_view::entry_position(direction side) const
{
    int column = _door_column();
    int row = _door_row();
    bn::fixed_point offset(-room::tile_size / 2, -room::tile_size / 2);

    switch(side)
    {

    case direction::north:
        return room::cell_center(column, _interior_top() + 1) + bn::fixed_point(offset.x(), 0);

    case direction::south:
        return room::cell_center(column, _interior_top() + _layout.height - 2) + bn::fixed_point(offset.x(), 0);

    case direction::west:
        return room::cell_center(_interior_left() + 1, row) + bn::fixed_point(0, offset.y());

    default:
        return room::cell_center(_interior_left() + _layout.width - 2, row) + bn::fixed_point(0, offset.y());
    }
}

bn::optional<direction> room_view::exit_side(const bn::fixed_point& position) const
{
    bn::fixed_point top_left = room::cell_center(_interior_left(), _interior_top());
    bn::fixed_point bottom_right = room::cell_center(_interior_left() + _layout.width - 1,
                                                     _interior_top() + _layout.height - 1);

    if(_doors[int(direction::north)] && position.y() < top_left.y() - 14)
    {
        return direction::north;
    }

    if(_doors[int(direction::south)] && position.y() > bottom_right.y() + 6)
    {
        return direction::south;
    }

    if(_doors[int(direction::west)] && position.x() < top_left.x() - 10)
    {
        return direction::west;
    }

    if(_doors[int(direction::east)] && position.x() > bottom_right.x() + 10)
    {
        return direction::east;
    }

    return bn::nullopt;
}

bn::fixed_point room_view::random_floor_position(bn::random& random) const
{
    for(int attempt = 0; attempt < 40; ++attempt)
    {
        int column = _interior_left() + 1 + random.get_int(_layout.width - 2);
        int row = _interior_top() + 1 + random.get_int(_layout.height - 2);
        bn::fixed_point position = room::cell_center(column, row);

        if(! room::feet_are_blocked(position))
        {
            return position;
        }
    }

    return interior_center();
}

bn::fixed_point room_view::interior_center() const
{
    return room::cell_center(_door_column(), _door_row()) - bn::fixed_point(room::tile_size / 2, 0);
}

bn::fixed_point room_view::chansey_position() const
{
    int column = _interior_left() + room_layouts::center_counter_x + room_layouts::center_counter_width / 2;
    int row = _interior_top() + room_layouts::center_counter_y - 1;
    return room::cell_center(column, row) - bn::fixed_point(room::tile_size / 2, 2);
}

bn::fixed_point room_view::pc_position() const
{
    return room::cell_center(_interior_left() + room_layouts::center_pc_x, _interior_top() + room_layouts::center_pc_y) +
           bn::fixed_point(room::tile_size / 2, room::tile_size / 2);
}

void room_view::_fill(int column, int row, int width, int height, char value)
{
    for(int y = row; y < row + height; ++y)
    {
        for(int x = column; x < column + width; ++x)
        {
            room::set(x, y, value);
        }
    }
}

void room_view::_carve_doors(bool locked)
{
    char value = locked ? room::cells::door : room::cells::floor;
    int half = room_layouts::door_width / 2;

    if(_doors[int(direction::north)])
    {
        _fill(_door_column() - half, _top, room_layouts::door_width, room_layouts::wall_top, value);
    }

    if(_doors[int(direction::south)])
    {
        _fill(_door_column() - half, _top + _height - room_layouts::wall_bottom, room_layouts::door_width,
              room_layouts::wall_bottom, value);
    }

    if(_doors[int(direction::west)])
    {
        _fill(_left, _door_row() - half, room_layouts::wall_side, room_layouts::door_width, value);
    }

    if(_doors[int(direction::east)])
    {
        _fill(_left + _width - room_layouts::wall_side, _door_row() - half, room_layouts::wall_side,
              room_layouts::door_width, value);
    }
}

void room_view::_render()
{
    int stairs_column = _door_column() - 1;
    int stairs_row = _door_row() - 1;
    int pc_column = _interior_left() + room_layouts::center_pc_x;
    int pc_row = _interior_top() + room_layouts::center_pc_y;

    for(int row = 0; row < room::rows; ++row)
    {
        for(int column = 0; column < room::columns; ++column)
        {
            char value = room::get(column, row);
            int tile = tiles::empty;

            switch(value)
            {

            case room::cells::wall:
                tile = walkable(room::get(column, row + 1)) ? tiles::wall_face : tiles::wall_top;
                break;

            case room::cells::floor:
                if(_kind == room_kind::center)
                {
                    tile = tiles::center_floor;
                }
                else if(room::get(column, row - 1) == room::cells::wall ||
                        room::get(column, row - 1) == room::cells::door)
                {
                    tile = tiles::floor_shadow;
                }
                else if((column * 7 + row * 13) % 29 == 0)
                {
                    tile = tiles::floor_crack;
                }
                else
                {
                    tile = column % 2 == 0 && row % 2 == 0 ? tiles::floor_light : tiles::floor;
                }
                break;

            case room::cells::door:
                tile = tiles::door;
                break;

            case room::cells::stairs:
                tile = quad_tile(tiles::stairs, column - stairs_column, row - stairs_row);
                break;

            case room::cells::counter:
                tile = room::get(column, row + 1) == room::cells::counter ? tiles::counter_top : tiles::counter_front;
                break;

            case room::cells::pc:
                tile = quad_tile(tiles::pc, column - pc_column, row - pc_row);
                break;

            default:
                break;
            }

            bn::regular_bg_map_cell_info info;
            info.set_tile_index(tile);
            map_cells[map_item.cell_index(column, row)] = info.cell();
        }
    }

    _bg_map.reload_cells_ref();
}
