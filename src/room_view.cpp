#include "room_view.h"

#include "bn_bg_tiles.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_map_item.h"

#include "bn_math.h"

#include "room.h"

namespace
{
    namespace tiles
    {
        constexpr int empty = 0;
        constexpr int floor = 1;
        constexpr int floor_detail = 5;
        constexpr int floor_shadow = 9;
        constexpr int wall_top = 11;
        constexpr int wall_face_high = 15;
        constexpr int wall_face_low = 17;
        constexpr int door = 19;
        constexpr int stairs = 23;
        constexpr int tall_grass = 27;
        constexpr int ice = 27;
        constexpr int bush = 31;
        constexpr int water = 35;
        constexpr int pit = 35;
        constexpr int flow_right = 39;
        constexpr int flow_left = 43;
        constexpr int flow_down = 47;
        constexpr int flow_up = 51;
        constexpr int wind_east = 39;
        constexpr int wind_west = 43;
        constexpr int wind_south = 47;
        constexpr int wind_north = 51;
        constexpr int waterfall = 47;
        constexpr int special = 55;
        constexpr int plate = 55;
        constexpr int quad_size = 4;
    }

    constexpr int river_width = 3;
    constexpr int bridge_width = 3;

    constexpr int grass_patches = 3;

    alignas(int) bn::regular_bg_map_cell map_cells[room::columns * room::rows];

    const bn::regular_bg_map_item map_item(map_cells[0], bn::size(room::columns, room::rows));

    bn::regular_bg_ptr create_bg(const floor_theme& theme)
    {
        bn::bg_tiles::set_allow_offset(false);
        bn::regular_bg_item item(*theme.tiles, *theme.palette, map_item);
        bn::regular_bg_ptr result = item.create_bg(0, 0);
        bn::bg_tiles::set_allow_offset(true);
        return result;
    }

    bool walkable(char value)
    {
        return value == room::cells::floor || value == room::cells::stairs || value == room::cells::grass ||
               value == room::cells::plate || value == room::cells::ice || value == room::cells::pit ||
               value == room::cells::warp ||
               room::is_water(value) || room::is_wind(value) || room::is_spinner(value);
    }

    int next_seed(unsigned& seed)
    {
        seed = seed * 1103515245 + 12345;
        return int((seed >> 16) & 0x7fff);
    }

    int quad_tile(int first_tile, int column, int row)
    {
        return first_tile + (column & 1) + (row & 1) * 2;
    }

    int pair_tile(int first_tile, int column)
    {
        return first_tile + (column & 1);
    }

    bool shows_detail(int column, int row)
    {
        unsigned block = unsigned(column / 2) * 73856093u ^ unsigned(row / 2) * 19349663u;
        return (block >> 3) % 7 == 0;
    }
}

room_view::room_view() :
    _bg(create_bg(floor_themes::get(1))),
    _bg_map(_bg.map()),
    _theme(&floor_themes::get(1)),
    _layout(room_layouts::start)
{
}

void room_view::build(const floor_room& value, const bool doors[4], bool locked, const floor_theme& theme, int seed)
{
    _set_theme(theme);
    _kind = value.kind;

    switch(value.kind)
    {

    case room_kind::combat:
        _layout = room_layouts::combat[value.layout];
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
    else if(value.kind == room_kind::combat && theme.tall_grass)
    {
        _plant_grass(seed);
    }
    else if(value.kind == room_kind::combat && theme.water)
    {
        _plant_water(seed, theme.whirlpools);
    }

    _plate_phase = 0;

    if(theme.hazard != hazard_kind::none && value.kind != room_kind::start)
    {
        _plant_plates(seed);
    }

    if(theme.ice_floor && value.kind != room_kind::start)
    {
        _plant_ice(seed);
    }

    if(theme.chasm && value.kind != room_kind::start)
    {
        _plant_chasm(seed);
    }

    if(theme.spinners && value.kind != room_kind::start)
    {
        _plant_spinners(seed);
    }

    _warps.clear();

    if(theme.warps && value.kind == room_kind::combat)
    {
        _plant_warps(seed);
    }

    _plant_bushes(value);

    room::set_camera_bounds(_left, _top - 2, _left + _width - 1, _top + _height);
    set_locked(locked);
}

void room_view::set_locked(bool locked)
{
    _carve_doors(locked);
    _render();
}

void room_view::set_plate_phase(int phase)
{
    if(phase != _plate_phase)
    {
        _plate_phase = phase;
        _render();
    }
}

void room_view::set_camera(const bn::camera_ptr& camera)
{
    _camera = camera;
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

bn::optional<bn::fixed_point> room_view::random_grass_position(bn::random& random) const
{
    for(int attempt = 0; attempt < 40; ++attempt)
    {
        int column = _interior_left() + 1 + random.get_int(_layout.width - 2);
        int row = _interior_top() + 1 + random.get_int(_layout.height - 2);

        if(room::get(column, row) == room::cells::grass)
        {
            bn::fixed_point position = room::cell_center(column, row);

            if(! room::feet_are_blocked(position))
            {
                return position;
            }
        }
    }

    return bn::nullopt;
}

bn::optional<bn::fixed_point> room_view::random_water_position(bn::random& random) const
{
    for(int attempt = 0; attempt < 60; ++attempt)
    {
        int column = _interior_left() + 1 + random.get_int(_layout.width - 2);
        int row = _interior_top() + 1 + random.get_int(_layout.height - 2);

        if(room::is_water(room::get(column, row)) && room::is_water(room::get(column, row + 1)))
        {
            return room::cell_center(column, row) - bn::fixed_point(0, 4);
        }
    }

    return bn::nullopt;
}

bool room_view::cut_bushes(const bn::fixed_point& center, int half_size)
{
    bool cut = false;

    for(int side = 0; side < 4; ++side)
    {
        int column, row, width, height;
        _bush_area(direction(side), column, row, width, height);

        for(int y = row; y < row + height; ++y)
        {
            for(int x = column; x < column + width; ++x)
            {
                bn::fixed_point cell = room::cell_center(x, y);
                bn::fixed_point delta = cell - center;

                if(room::get(x, y) == room::cells::bush && bn::abs(delta.x()) < half_size + 4 &&
                   bn::abs(delta.y()) < half_size + 4)
                {
                    room::set(x, y, room::cells::floor);
                    cut = true;
                }
            }
        }
    }

    if(cut)
    {
        _render();
    }

    return cut;
}

bool room_view::bushes_remaining(direction side) const
{
    int column, row, width, height;
    _bush_area(side, column, row, width, height);

    for(int y = row; y < row + height; ++y)
    {
        for(int x = column; x < column + width; ++x)
        {
            if(room::get(x, y) == room::cells::bush)
            {
                return true;
            }
        }
    }

    return false;
}

bn::fixed_point room_view::open_spot_near(const bn::fixed_point& position) const
{
    if(! room::feet_are_blocked(position))
    {
        return position;
    }

    bn::optional<bn::fixed_point> spot = room::nearest_standable(position, false);
    return spot ? *spot : position;
}

bn::fixed_point room_view::interior_center() const
{
    return room::cell_center(_door_column(), _door_row()) - bn::fixed_point(room::tile_size / 2, 0);
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

void room_view::_set_theme(const floor_theme& theme)
{
    if(_theme == &theme)
    {
        return;
    }

    _theme = &theme;
    _bg = create_bg(theme);
    _bg_map = _bg.map();

    if(_camera)
    {
        _bg.set_camera(*_camera);
    }
}

void room_view::_plant_grass(int initial_seed)
{
    unsigned seed = unsigned(initial_seed);

    for(int patch = 0; patch < grass_patches; ++patch)
    {
        int width = 4 + next_seed(seed) % 6;
        int height = 3 + next_seed(seed) % 3;
        int column = _interior_left() + 1 + next_seed(seed) % bn::max(_layout.width - width - 2, 1);
        int row = _interior_top() + 1 + next_seed(seed) % bn::max(_layout.height - height - 2, 1);

        for(int y = row; y < row + height; ++y)
        {
            for(int x = column; x < column + width; ++x)
            {
                if(room::get(x, y) == room::cells::floor)
                {
                    room::set(x, y, room::cells::grass);
                }
            }
        }
    }
}

void room_view::_plant_water(int initial_seed, bool whirlpools)
{
    unsigned seed = unsigned(initial_seed);
    bool horizontal = next_seed(seed) % 2 && ! whirlpools;
    bool forward = next_seed(seed) % 2 || whirlpools;

    if(horizontal && _layout.height >= 12)
    {
        int first = _interior_top() + 3;
        int last = _interior_top() + _layout.height - 4 - river_width;
        int row = first + next_seed(seed) % bn::max(last - first + 1, 1);

        if(row <= _door_row() + 1 && row + river_width > _door_row() - 2)
        {
            row = _door_row() + 2 <= last ? _door_row() + 2 : _door_row() - 2 - river_width;
        }

        char flow = forward ? room::cells::flow_right : room::cells::flow_left;
        int bridge = _interior_left() + 2 + next_seed(seed) % bn::max(_layout.width - bridge_width - 4, 1);

        for(int y = row; y < row + river_width; ++y)
        {
            for(int x = _interior_left(); x < _interior_left() + _layout.width; ++x)
            {
                if(room::get(x, y) == room::cells::floor && (x < bridge || x >= bridge + bridge_width))
                {
                    room::set(x, y, flow);
                }
            }
        }
    }
    else if(_layout.width >= 14)
    {
        int first = _interior_left() + 3;
        int last = _interior_left() + _layout.width - 4 - river_width;
        int column = first + next_seed(seed) % bn::max(last - first + 1, 1);

        if(column <= _door_column() + 1 && column + river_width > _door_column() - 2)
        {
            column = _door_column() + 2 <= last ? _door_column() + 2 : _door_column() - 2 - river_width;
        }

        char flow = whirlpools ? room::cells::waterfall : forward ? room::cells::flow_down : room::cells::flow_up;
        int bridge = _interior_top() + 2 + next_seed(seed) % bn::max(_layout.height - bridge_width - 4, 1);

        for(int x = column; x < column + river_width; ++x)
        {
            for(int y = _interior_top(); y < _interior_top() + _layout.height; ++y)
            {
                if(room::get(x, y) == room::cells::floor && (y < bridge || y >= bridge + bridge_width))
                {
                    room::set(x, y, flow);
                }
            }
        }
    }

    int width = 4 + next_seed(seed) % 3;
    int height = 3 + next_seed(seed) % 2;
    int column = _interior_left() + 2 + next_seed(seed) % bn::max(_layout.width - width - 4, 1);
    int row = _interior_top() + 2 + next_seed(seed) % bn::max(_layout.height - height - 4, 1);

    for(int y = row; y < row + height; ++y)
    {
        for(int x = column; x < column + width; ++x)
        {
            if(room::get(x, y) == room::cells::floor)
            {
                room::set(x, y, whirlpools ? room::cells::whirlpool : room::cells::water);
            }
        }
    }

    if(whirlpools)
    {
        room::set_whirlpool_center(room::cell_center(column + width / 2, row + height / 2) - bn::fixed_point(4, 4));
    }
}

void room_view::_plant_plates(int initial_seed)
{
    unsigned seed = unsigned(initial_seed) * 7 + 3;

    for(int patch = 0; patch < 3; ++patch)
    {
        int width = 3 + next_seed(seed) % 4;
        int height = 2 + next_seed(seed) % 3;
        int column = _interior_left() + 1 + next_seed(seed) % bn::max(_layout.width - width - 2, 1);
        int row = _interior_top() + 1 + next_seed(seed) % bn::max(_layout.height - height - 2, 1);

        for(int y = row; y < row + height; ++y)
        {
            for(int x = column; x < column + width; ++x)
            {
                if(room::get(x, y) == room::cells::floor)
                {
                    room::set(x, y, room::cells::plate);
                }
            }
        }
    }
}

void room_view::_plant_ice(int initial_seed)
{
    unsigned seed = unsigned(initial_seed) * 11 + 5;

    for(int patch = 0; patch < 2; ++patch)
    {
        int width = 6 + next_seed(seed) % 5;
        int height = 4 + next_seed(seed) % 3;
        int column = _interior_left() + 1 + next_seed(seed) % bn::max(_layout.width - width - 2, 1);
        int row = _interior_top() + 1 + next_seed(seed) % bn::max(_layout.height - height - 2, 1);

        for(int y = row; y < row + height; ++y)
        {
            for(int x = column; x < column + width; ++x)
            {
                if(room::get(x, y) == room::cells::floor)
                {
                    room::set(x, y, room::cells::ice);
                }
            }
        }
    }
}

void room_view::_plant_chasm(int initial_seed)
{
    unsigned seed = unsigned(initial_seed) * 13 + 9;
    bool horizontal = next_seed(seed) % 2;
    bool forward = next_seed(seed) % 2;
    char wind = horizontal ? (forward ? room::cells::wind_east : room::cells::wind_west) :
                             (forward ? room::cells::wind_south : room::cells::wind_north);

    if(horizontal && _layout.height >= 10)
    {
        int row = _interior_top() + 3 + next_seed(seed) % bn::max(_layout.height - 9, 1);

        for(int y = row; y < row + 3; ++y)
        {
            for(int x = _interior_left(); x < _interior_left() + _layout.width; ++x)
            {
                if(room::get(x, y) == room::cells::floor)
                {
                    room::set(x, y, wind);
                }
            }
        }
    }
    else if(! horizontal && _layout.width >= 12)
    {
        int column = _interior_left() + 3 + next_seed(seed) % bn::max(_layout.width - 9, 1);

        for(int x = column; x < column + 3; ++x)
        {
            for(int y = _interior_top(); y < _interior_top() + _layout.height; ++y)
            {
                if(room::get(x, y) == room::cells::floor)
                {
                    room::set(x, y, wind);
                }
            }
        }
    }

    for(int patch = 0; patch < 2; ++patch)
    {
        int width = 3 + next_seed(seed) % 3;
        int height = 2 + next_seed(seed) % 2;
        int column = _interior_left() + 3 + next_seed(seed) % bn::max(_layout.width - width - 6, 1);
        int row = _interior_top() + 3 + next_seed(seed) % bn::max(_layout.height - height - 6, 1);

        for(int y = row; y < row + height; ++y)
        {
            for(int x = column; x < column + width; ++x)
            {
                char current = room::get(x, y);

                if(current == room::cells::floor || room::is_wind(current))
                {
                    room::set(x, y, room::cells::pit);
                }
            }
        }
    }
}

void room_view::_plant_spinners(int initial_seed)
{
    unsigned seed = unsigned(initial_seed) * 17 + 1;

    for(int lane = 0; lane < 2; ++lane)
    {
        bool horizontal = next_seed(seed) % 2;
        bool forward = next_seed(seed) % 2;
        int length = 6 + next_seed(seed) % 6;
        char arrow = horizontal ? (forward ? room::cells::spin_right : room::cells::spin_left) :
                                  (forward ? room::cells::spin_down : room::cells::spin_up);
        int width = horizontal ? length : 2;
        int height = horizontal ? 2 : length;
        int column = _interior_left() + 2 + next_seed(seed) % bn::max(_layout.width - width - 4, 1);
        int row = _interior_top() + 2 + next_seed(seed) % bn::max(_layout.height - height - 4, 1);

        for(int y = row; y < row + height; ++y)
        {
            for(int x = column; x < column + width; ++x)
            {
                if(room::get(x, y) == room::cells::floor)
                {
                    room::set(x, y, arrow);
                }
            }
        }
    }
}

void room_view::_plant_warps(int initial_seed)
{
    unsigned seed = unsigned(initial_seed) * 19 + 7;

    for(int attempt = 0; attempt < 40 && ! _warps.full(); ++attempt)
    {
        int column = _interior_left() + 2 + next_seed(seed) % bn::max(_layout.width - 4, 1);
        int row = _interior_top() + 2 + next_seed(seed) % bn::max(_layout.height - 4, 1);
        bn::fixed_point position = room::cell_center(column, row);
        bool clear = room::get(column, row) == room::cells::floor && ! room::feet_are_blocked(position);

        for(const bn::fixed_point& other : _warps)
        {
            bn::fixed_point delta = other - position;
            clear = clear && bn::abs(delta.x()) + bn::abs(delta.y()) > 64;
        }

        if(clear)
        {
            room::set(column, row, room::cells::warp);
            _warps.push_back(position);
        }
    }

    if(_warps.size() % 2)
    {
        bn::fixed_point last = _warps.back();
        room::set((last.x().round_integer() + room::pixel_width / 2) / room::tile_size,
                  (last.y().round_integer() + room::pixel_height / 2) / room::tile_size, room::cells::floor);
        _warps.pop_back();
    }
}

bn::optional<bn::fixed_point> room_view::warp_partner(const bn::fixed_point& position) const
{
    for(int index = 0; index < _warps.size(); ++index)
    {
        bn::fixed_point delta = _warps[index] - position;

        if(bn::abs(delta.x()) < room::tile_size && bn::abs(delta.y()) < room::tile_size)
        {
            return _warps[index ^ 1];
        }
    }

    return bn::nullopt;
}

void room_view::_plant_bushes(const floor_room& value)
{
    for(int side = 0; side < 4; ++side)
    {
        if(_doors[side] && value.overgrown[side])
        {
            int column, row, width, height;
            _bush_area(direction(side), column, row, width, height);
            _fill(column, row, width, height, room::cells::bush);
        }
    }
}

void room_view::_bush_area(direction side, int& column, int& row, int& width, int& height) const
{
    int half = room_layouts::door_width / 2;

    switch(side)
    {

    case direction::north:
        column = _door_column() - half;
        row = _interior_top();
        width = room_layouts::door_width;
        height = 1;
        break;

    case direction::south:
        column = _door_column() - half;
        row = _interior_top() + _layout.height - 1;
        width = room_layouts::door_width;
        height = 1;
        break;

    case direction::west:
        column = _interior_left();
        row = _door_row() - half;
        width = 1;
        height = room_layouts::door_width;
        break;

    default:
        column = _interior_left() + _layout.width - 1;
        row = _door_row() - half;
        width = 1;
        height = room_layouts::door_width;
        break;
    }
}

void room_view::_render()
{
    int stairs_column = _door_column() - 1;
    int stairs_row = _door_row() - 1;

    for(int row = 0; row < room::rows; ++row)
    {
        for(int column = 0; column < room::columns; ++column)
        {
            char value = room::get(column, row);
            int tile = tiles::empty;

            switch(value)
            {

            case room::cells::wall:
                if(walkable(room::get(column, row + 1)))
                {
                    tile = pair_tile(tiles::wall_face_low, column);
                }
                else if(room::get(column, row + 1) == room::cells::wall && walkable(room::get(column, row + 2)))
                {
                    tile = pair_tile(tiles::wall_face_high, column);
                }
                else
                {
                    tile = quad_tile(tiles::wall_top, column, row);
                }
                break;

            case room::cells::floor:
                if(room::get(column, row - 1) == room::cells::wall ||
                   room::get(column, row - 1) == room::cells::door || room::get(column, row - 1) == room::cells::bush)
                {
                    tile = pair_tile(tiles::floor_shadow, column);
                }
                else
                {
                    tile = quad_tile(shows_detail(column, row) ? tiles::floor_detail : tiles::floor, column, row);
                }
                break;

            case room::cells::door:
                tile = quad_tile(tiles::door, column, row);
                break;

            case room::cells::stairs:
                tile = quad_tile(tiles::stairs, column - stairs_column, row - stairs_row);
                break;

            case room::cells::grass:
                tile = quad_tile(tiles::tall_grass, column, row);
                break;

            case room::cells::bush:
                tile = quad_tile(tiles::bush, column, row);
                break;

            case room::cells::water:
                tile = quad_tile(tiles::water, column, row);
                break;

            case room::cells::ice:
                tile = quad_tile(tiles::ice, column, row);
                break;

            case room::cells::pit:
                tile = quad_tile(tiles::pit, column, row);
                break;

            case room::cells::waterfall:
                tile = quad_tile(tiles::waterfall, column, row);
                break;

            case room::cells::whirlpool:
            case room::cells::warp:
                tile = quad_tile(tiles::special, column, row);
                break;

            case room::cells::wind_east:
            case room::cells::spin_right:
                tile = quad_tile(tiles::wind_east, column, row);
                break;

            case room::cells::wind_west:
            case room::cells::spin_left:
                tile = quad_tile(tiles::wind_west, column, row);
                break;

            case room::cells::wind_south:
            case room::cells::spin_down:
                tile = quad_tile(tiles::wind_south, column, row);
                break;

            case room::cells::wind_north:
            case room::cells::spin_up:
                tile = quad_tile(tiles::wind_north, column, row);
                break;

            case room::cells::plate:
                tile = quad_tile(tiles::plate + _plate_phase * tiles::quad_size, column, row);
                break;

            case room::cells::flow_right:
                tile = quad_tile(tiles::flow_right, column, row);
                break;

            case room::cells::flow_left:
                tile = quad_tile(tiles::flow_left, column, row);
                break;

            case room::cells::flow_down:
                tile = quad_tile(tiles::flow_down, column, row);
                break;

            case room::cells::flow_up:
                tile = quad_tile(tiles::flow_up, column, row);
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
