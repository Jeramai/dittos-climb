#include "options.h"

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "bn_sound_items.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"

#include "audio.h"
#include "overlay.h"
#include "profile.h"
#include "ui.h"

namespace options
{

void show()
{
    overlay screen;
    screen.show_backdrop();
    screen.window(0, 0, 30, 3, window_style::blue);
    screen.window(0, 3, 30, 10, window_style::white);
    screen.window(0, 13, 30, 7, window_style::white);

    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();
    big.set_bg_priority(0);

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font, ui::dark_text_palette());
    small.set_center_alignment();
    small.set_bg_priority(0);

    bn::sprite_text_generator hint(common::variable_8x8_sprite_font, ui::dark_text_palette());
    hint.set_center_alignment();
    hint.set_bg_priority(0);

    int levels[2] = { profile::get().music_level, profile::get().sound_level };
    const char* labels[2] = { "MUSIC", "SOUND" };
    int cursor = 0;
    bn::vector<bn::sprite_ptr, 40> text;

    auto draw = [&]()
    {
        text.clear();
        big.generate(0, ui::row_y(1) + 2, "OPTIONS", text);

        for(int row = 0; row < 2; ++row)
        {
            bn::string<32> line(row == cursor ? "> " : "  ");
            line.append(labels[row]);
            line.append("  ");
            line.append(levels[row] > 0 ? "<" : " ");
            line.append(" ");

            for(int step = 0; step < profile::max_level; ++step)
            {
                line.append(step < levels[row] ? "=" : "-");
            }

            line.append(" ");
            line.append(levels[row] < profile::max_level ? ">" : " ");
            small.generate(0, ui::row_y(6) + row * 24, line, text);
        }

        hint.generate(0, ui::row_y(15), "UP/DOWN: CHOOSE", text);
        hint.generate(0, ui::row_y(16) + 2, "LEFT/RIGHT: CHANGE", text);
        hint.generate(0, ui::row_y(18), "B: BACK", text);
    };

    draw();
    bn::core::update();

    while(! bn::keypad::b_pressed())
    {
        if(bn::keypad::up_pressed() || bn::keypad::down_pressed())
        {
            cursor = 1 - cursor;
            audio::play_quiet(bn::sound_items::sfx_menu);
            draw();
        }
        else if(bn::keypad::left_pressed() || bn::keypad::right_pressed())
        {
            int change = bn::keypad::right_pressed() ? 1 : -1;
            int level = bn::clamp(levels[cursor] + change, 0, profile::max_level);

            if(level != levels[cursor])
            {
                levels[cursor] = level;
                audio::set_levels(levels[0], levels[1]);
                audio::play(bn::sound_items::sfx_menu);
                draw();
            }
        }

        bn::core::update();
    }

    profile::set_levels(levels[0], levels[1]);
    audio::play(bn::sound_items::sfx_menu);
}

}
