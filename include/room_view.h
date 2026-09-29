#ifndef ROOM_VIEW_H
#define ROOM_VIEW_H

#include "bn_camera_ptr.h"
#include "bn_optional.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"

#include "floor_map.h"
#include "floor_theme.h"
#include "room_layouts.h"

class room_view
{

public:
    room_view();

    void build(const floor_room& value, const bool doors[4], bool locked, const floor_theme& theme, int seed);

    void set_locked(bool locked);

    void set_plate_phase(int phase);

    void set_camera(const bn::camera_ptr& camera);

    [[nodiscard]] const bn::regular_bg_ptr& bg() const
    {
        return _bg;
    }

    [[nodiscard]] bn::fixed_point entry_position(direction side) const;

    [[nodiscard]] bn::optional<direction> exit_side(const bn::fixed_point& position) const;

    [[nodiscard]] bn::fixed_point random_floor_position(bn::random& random) const;

    [[nodiscard]] bn::optional<bn::fixed_point> random_grass_position(bn::random& random) const;

    [[nodiscard]] bn::optional<bn::fixed_point> random_water_position(bn::random& random) const;

    [[nodiscard]] bool cut_bushes(const bn::fixed_point& center, int half_size);

    [[nodiscard]] bool bushes_remaining(direction side) const;

    [[nodiscard]] bn::fixed_point interior_center() const;

private:
    bn::regular_bg_ptr _bg;
    bn::regular_bg_map_ptr _bg_map;
    bn::optional<bn::camera_ptr> _camera;
    const floor_theme* _theme;
    room_layout _layout;
    room_kind _kind = room_kind::start;
    bool _doors[4] = {};
    int _left = 0;
    int _top = 0;
    int _width = 0;
    int _height = 0;
    int _plate_phase = 0;

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

    void _set_theme(const floor_theme& theme);

    void _plant_grass(int seed);

    void _plant_water(int seed);

    void _plant_plates(int seed);

    void _plant_bushes(const floor_room& value);

    void _bush_area(direction side, int& column, int& row, int& width, int& height) const;

    void _render();
};

#endif
