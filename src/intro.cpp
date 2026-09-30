#include "intro.h"

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_palettes.h"
#include "bn_math.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"

#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_regular_bg_items_story_bg.h"
#include "bn_regular_bg_items_title_bg.h"
#include "bn_sprite_items_ditto.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"

#include "audio.h"
#include "options.h"
#include "overlay.h"
#include "pokedex.h"
#include "profile.h"
#include "species.h"
#include "ui.h"

namespace
{
    struct page
    {
        const char* lines[3];
    };

    constexpr page pages[] = {
        { { "Long ago, on CINNABAR ISLAND,", "scientists tried to clone", "the POKEMON called MEW." } },
        { { "One attempt went wrong.", "It made a small pink blob", "with a silly face." } },
        { { "They wrote FAILURE in the log", "and flushed it down", "the waste pipe." } },
        { { "The pipe went deep, deep down,", "to the bottom of", "the UNKNOWN DUNGEON." } },
        { { "DITTO woke up in the dark.", "It knew only one move:", "TRANSFORM." } },
        { { "Then a soft voice came", "from far above...", "" } },
        { { "\"Come up to me, little one.", "I can tell you who", "you really are.\"" } },
        { { "So DITTO began to climb.", "", "" } },
    };

    constexpr int story_line_height = 16;
    constexpr int story_text_top = 22;
}

namespace intro
{

bool title(bn::random& random, bool can_continue)
{
    audio::play_music(bn::music_items::title);
    bn::bg_palettes::set_fade(bn::color(0, 0, 0), 0);
    bn::sprite_palettes::set_fade(bn::color(0, 0, 0), 0);
    bn::regular_bg_ptr background = bn::regular_bg_items::title_bg.create_bg(8, 48);
    bn::bg_palettes::set_transparent_color(bn::color(2, 2, 5));

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();

    bn::vector<bn::sprite_ptr, 20> text;
    if(can_continue)
    {
        small.generate(0, 40, "A: CONTINUE", text);
        small.generate(0, 52, "START: NEW RUN", text);
    }
    else
    {
        small.generate(0, 40, "PRESS START", text);
    }

    small.generate(0, 66, "SELECT: POKEDEX  R: OPTIONS", text);

    bn::sprite_ptr ditto = bn::sprite_items::ditto.create_sprite(0, 0, species_frames::own_walk);
    ditto.set_scale(2);

    int frame = 0;

    bool resume = false;

    while(! bn::keypad::start_pressed())
    {
        if(can_continue && bn::keypad::a_pressed())
        {
            resume = true;
            break;
        }

        bool open_pokedex = bn::keypad::select_pressed();

        if(open_pokedex || bn::keypad::r_pressed())
        {
            audio::play(bn::sound_items::sfx_menu);
            ditto.set_visible(false);
            background.set_visible(false);
            bn::bg_palettes::set_transparent_color(bn::nullopt);

            for(bn::sprite_ptr& sprite : text)
            {
                sprite.set_visible(false);
            }

            if(open_pokedex)
            {
                pokedex::show();
            }
            else
            {
                options::show();
            }

            ditto.set_visible(true);
            background.set_visible(true);
            bn::bg_palettes::set_transparent_color(bn::color(2, 2, 5));

            for(bn::sprite_ptr& sprite : text)
            {
                sprite.set_visible(true);
            }
        }

        ++frame;
        ditto.set_y(bn::degrees_lut_sin((frame * 4) % 360) * 4);
        ditto.set_tiles(bn::sprite_items::ditto.tiles_item(), species_frames::own_walk + (frame / 20) % 2);

        for(bn::sprite_ptr& sprite : text)
        {
            if(sprite.y() > 0 && sprite.y() < 60)
            {
                sprite.set_visible((frame / 30) % 2 == 0);
            }
        }

        random.update();
        bn::core::update();
    }

    audio::play(bn::sound_items::sfx_menu);
    bn::bg_palettes::set_transparent_color(bn::nullopt);
    bn::core::update();
    return resume;
}

void story()
{
    bn::regular_bg_ptr background = bn::regular_bg_items::story_bg.create_bg(8, 24);
    bn::bg_palettes::set_transparent_color(bn::color(2, 2, 5));
    overlay window;
    window.clear();
    window.window(0, 12, 30, 8, window_style::white);

    bn::sprite_text_generator generator(common::variable_8x16_sprite_font, ui::dark_text_palette());
    generator.set_center_alignment();
    generator.set_bg_priority(0);

    bn::sprite_text_generator hint_generator(common::variable_8x8_sprite_font, ui::dark_text_palette());
    hint_generator.set_right_alignment();
    hint_generator.set_bg_priority(0);

    for(const page& current : pages)
    {
        bn::vector<bn::sprite_ptr, 32> text;

        for(int line = 0; line < 3; ++line)
        {
            generator.generate(0, story_text_top + line * story_line_height, current.lines[line], text);
        }

        hint_generator.generate(ui::tile_x(29) - 2, ui::row_y(18) + 2, "A: NEXT   START: SKIP", text);

        while(true)
        {
            bn::core::update();

            if(bn::keypad::start_pressed())
            {
                bn::bg_palettes::set_transparent_color(bn::nullopt);
                bn::core::update();
                return;
            }

            if(bn::keypad::a_pressed())
            {
                audio::play(bn::sound_items::sfx_menu);
                break;
            }
        }
    }

    bn::bg_palettes::set_transparent_color(bn::nullopt);
    bn::core::update();
}

}
