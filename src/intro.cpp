#include "intro.h"

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"

#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_ditto.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "audio.h"
#include "species.h"

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

    constexpr int line_height = 18;
    constexpr int text_top = -30;
}

namespace intro
{

bool title(bn::random& random, bool can_continue)
{
    audio::play_music(bn::music_items::title);
    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();

    bn::vector<bn::sprite_ptr, 12> text;
    big.generate(0, -40, "DITTO'S CLIMB", text);
    if(can_continue)
    {
        small.generate(0, 40, "A: CONTINUE", text);
        small.generate(0, 52, "START: NEW RUN", text);
    }
    else
    {
        small.generate(0, 40, "PRESS START", text);
    }

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

        ++frame;
        ditto.set_y(bn::degrees_lut_sin((frame * 4) % 360) * 4);
        ditto.set_tiles(bn::sprite_items::ditto.tiles_item(), species_frames::own_walk + (frame / 20) % 2);

        for(bn::sprite_ptr& sprite : text)
        {
            if(sprite.y() > 0)
            {
                sprite.set_visible((frame / 30) % 2 == 0);
            }
        }

        random.update();
        bn::core::update();
    }

    audio::play(bn::sound_items::sfx_menu);
    bn::core::update();
    return resume;
}

void story()
{
    bn::sprite_text_generator generator(common::variable_8x16_sprite_font);
    generator.set_center_alignment();

    bn::sprite_text_generator hint_generator(common::fixed_8x8_sprite_font);
    hint_generator.set_right_alignment();

    for(const page& current : pages)
    {
        bn::vector<bn::sprite_ptr, 32> text;

        for(int line = 0; line < 3; ++line)
        {
            generator.generate(0, text_top + line * line_height, current.lines[line], text);
        }

        hint_generator.generate(116, 72, "A: NEXT  START: SKIP", text);

        while(true)
        {
            bn::core::update();

            if(bn::keypad::start_pressed())
            {
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

    bn::core::update();
}

}
