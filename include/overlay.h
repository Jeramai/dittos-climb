#ifndef OVERLAY_H
#define OVERLAY_H

#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"

class floor_map;

class overlay
{

public:
    overlay();

    void show_black();

    void show_map(const floor_map& floor, int current_room);

    void hide();

private:
    bn::regular_bg_ptr _bg;
    bn::regular_bg_map_ptr _bg_map;

    void _set(int column, int row, int tile);
};

#endif
