#include "ui.h"

#include "bn_color.h"

#include "types.h"

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

    constexpr bn::color outline(2, 2, 3);

    constexpr bn::color super_colors[16] = { unused, bn::color(31, 9, 7), outline };
    constexpr bn::color normal_colors[16] = { unused, bn::color(31, 31, 30), outline };
    constexpr bn::color weak_colors[16] = { unused, bn::color(20, 20, 22), outline };

    constexpr bn::sprite_palette_item super_numbers(super_colors, bn::bpp_mode::BPP_4);
    constexpr bn::sprite_palette_item normal_numbers(normal_colors, bn::bpp_mode::BPP_4);
    constexpr bn::sprite_palette_item weak_numbers(weak_colors, bn::bpp_mode::BPP_4);
}

namespace ui
{

const bn::sprite_palette_item& dark_text_palette()
{
    return dark_text;
}

const bn::sprite_palette_item& number_palette(int effectiveness)
{
    return effectiveness > types::neutral ? super_numbers : effectiveness < types::neutral ? weak_numbers : normal_numbers;
}

}
