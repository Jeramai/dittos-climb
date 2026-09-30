#ifndef UI_H
#define UI_H

#include "bn_fixed.h"
#include "bn_sprite_palette_item.h"

namespace ui
{
    [[nodiscard]] const bn::sprite_palette_item& dark_text_palette();

    [[nodiscard]] constexpr int tile_x(int column)
    {
        return column * 8 - 120;
    }

    [[nodiscard]] constexpr int row_y(int row)
    {
        return row * 8 - 76;
    }
}

#endif
