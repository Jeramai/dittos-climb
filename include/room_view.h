#ifndef ROOM_VIEW_H
#define ROOM_VIEW_H

#include "bn_camera_ptr.h"
#include "bn_optional.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "floor_map.h"
#include "room_layouts.h"

class room_view
{

public:
    room_view();

    void build(const floor_room& value, const bool doors[4], bool locked);

    void set_locked(bool locked);

    void set_camera(const bn::camera_ptr& camera);

    [[nodiscard]] bn::fixed_point entry_position(direction side) const;

    [[nodiscard]] bn::optional<direction> exit_side(const bn::fixed_point& position) const;

    [[nodiscard]] bn::fixed_point random_floor_position(bn::random& random) const;

    [[nodiscard]] bn::fixed_point interior_center() const;

private:
    bn::regular_bg_ptr _bg;
    bn::regular_bg_map_ptr _bg_map;
    room_layout _layout;
    room_kind _kind = room_kind::start;
    bool _doors[4] = {};
    int _left = 0;
    int _top = 0;
    int _width = 0;
    int _height = 0;

    [[nodiscard]] int _interior_left() const
    {
        return _left + room_layouts::wall_side;
    }

    [[nodiscard]] int _interior_top() const
    {
        return _top + room_layouts::wall_top;
    }

    [[nodiscard]] int _door_column() const
    {
        return _interior_left() + _layout.width / 2;
    }

    [[nodiscard]] int _door_row() const
    {
        return _interior_top() + _layout.height / 2;
    }

    void _fill(int column, int row, int width, int height, char value);

    void _carve_doors(bool locked);

    void _render();
};

#endif
