#ifndef OVERLAY_H
#define OVERLAY_H

#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"

class floor_map;

enum class window_style
{
    white,
    paper,
    blue,
    dark,
};

class overlay
{

public:
    static constexpr int screen_columns = 30;
    static constexpr int screen_rows = 20;

    overlay();

    void show_black();

    void clear();

    void show_backdrop();

    void window(int x, int y, int width, int height, window_style style);

    void map(const floor_map& floor, int current_room, bool boss_alive, int x, int y, int width, int height);

    void hide();

private:
    bn::regular_bg_ptr _bg;
    bn::regular_bg_map_ptr _bg_map;

    void _set(int column, int row, int tile);

    void _fill(int tile);
};

#endif
