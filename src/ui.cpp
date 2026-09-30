#include "ui.h"

#include "bn_color.h"

namespace
{
    constexpr bn::color ink(12, 12, 13);
    constexpr bn::color shadow(26, 26, 25);
    constexpr bn::color unused(0, 0, 0);

    constexpr bn::color dark_text_colors[16] = {
        unused, ink, shadow, unused, unused, unused, unused, unused,
        unused, unused, unused, unused, shadow, unused, ink, unused,
    };

    constexpr bn::sprite_palette_item dark_text(dark_text_colors, bn::bpp_mode::BPP_4);
}

namespace ui
{

const bn::sprite_palette_item& dark_text_palette()
{
    return dark_text;
}

}
