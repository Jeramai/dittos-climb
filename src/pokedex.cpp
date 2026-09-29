#include "pokedex.h"

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_span.h"
#include "bn_sprite_palette_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "bn_sound_items.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "audio.h"
#include "profile.h"
#include "shiny.h"
#include "species.h"

namespace
{
    constexpr int entry_count = int(species_id::mew);
    constexpr int columns = 4;
    constexpr int rows = 3;
    constexpr int per_page = columns * rows;
    constexpr int page_count = (entry_count + per_page - 1) / per_page;

    constexpr bn::color silhouette_colors[16] = {
        bn::color(0, 0, 0), bn::color(8, 8, 12), bn::color(8, 8, 12), bn::color(8, 8, 12),
        bn::color(8, 8, 12), bn::color(8, 8, 12), bn::color(8, 8, 12), bn::color(8, 8, 12),
        bn::color(8, 8, 12), bn::color(8, 8, 12), bn::color(8, 8, 12), bn::color(8, 8, 12),
        bn::color(8, 8, 12), bn::color(8, 8, 12), bn::color(8, 8, 12), bn::color(8, 8, 12),
    };

    constexpr bn::sprite_palette_item silhouette(silhouette_colors, bn::bpp_mode::BPP_4);

    species_id entry_species(int entry)
    {
        return species_id(entry + 1);
    }

    bn::fixed_point cell_position(int slot)
    {
        return bn::fixed_point(-84 + (slot % columns) * 56, -30 + (slot / columns) * 34);
    }
}

namespace pokedex
{

void show()
{
    bn::sprite_text_generator big(common::variable_8x16_sprite_font);
    big.set_center_alignment();

    bn::sprite_text_generator small(common::fixed_8x8_sprite_font);
    small.set_center_alignment();

    const profile::data& stats = profile::get();
    bn::string<32> header("SEEN ");
    header.append(bn::to_string<4>(profile::seen_count()));
    header.append("/");
    header.append(bn::to_string<4>(entry_count));
    header.append("  USED ");
    header.append(bn::to_string<4>(profile::form_count()));

    bn::string<40> record("RUNS ");
    record.append(bn::to_string<6>(stats.runs));
    record.append("  WINS ");
    record.append(bn::to_string<6>(stats.wins));
    record.append("  BEST ");
    record.append(bn::to_string<4>(stats.best_floor));
    record.append("F");

    bn::vector<bn::sprite_ptr, 16> title;
    big.generate(0, -70, header, title);
    small.generate(0, -54, record, title);

    bn::vector<bn::sprite_ptr, per_page> icons;
    bn::vector<bn::sprite_ptr, 24> footer;
    int page = 0;
    int slot = 0;
    bool shiny_view = false;

    auto draw_page = [&]()
    {
        icons.clear();

        for(int index = 0; index < per_page; ++index)
        {
            int entry = page * per_page + index;

            if(entry >= entry_count)
            {
                break;
            }

            species_id id = entry_species(entry);
            const bn::sprite_item& item = *species::get(id).sprite;
            bn::sprite_ptr icon = item.create_sprite(cell_position(index), species_frames::walk);

            if(! profile::has_seen(id))
            {
                icon.set_palette(silhouette);
            }

            if(item.shape_size().width() > 16)
            {
                icon.set_scale(0.5);
            }

            icons.push_back(bn::move(icon));
        }
    };

    auto draw_footer = [&]()
    {
        footer.clear();
        int entry = page * per_page + slot;
        species_id id = entry_species(entry);
        bn::string<32> label("NO.");
        label.append(entry + 1 < 10 ? "00" : entry + 1 < 100 ? "0" : "");
        label.append(bn::to_string<4>(entry + 1));
        label.append(" ");
        label.append(profile::has_seen(id) ? species::get(id).name : "???");

        if(profile::has_form(id))
        {
            label.append(" USED");
        }

        if(profile::has_shiny_form(id))
        {
            label.append(" *");
        }

        small.generate(0, 62, label, footer);
        small.generate(0, 74, profile::has_shiny_form(id) ? "A: SHINY  L/R: PAGE  B: BACK" : "L/R: PAGE  B: BACK", footer);
    };

    auto set_shiny_view = [&](bool value)
    {
        shiny_view = value;
        species_id id = entry_species(page * per_page + slot);
        bn::sprite_ptr& icon = icons[slot];

        if(shiny_view)
        {
            icon.set_palette(*shiny::palette(id));
        }
        else
        {
            icon.set_palette(species::get(id).sprite->palette_item());
        }
    };

    auto move_to = [&](int new_page, int new_slot)
    {
        if(new_page < 0 || new_page >= page_count)
        {
            return;
        }

        if(shiny_view)
        {
            set_shiny_view(false);
        }

        int last_slot = bn::min(per_page, entry_count - new_page * per_page) - 1;
        new_slot = bn::clamp(new_slot, 0, last_slot);

        if(new_page == page && new_slot == slot)
        {
            return;
        }

        audio::play_quiet(bn::sound_items::sfx_menu);
        icons[slot].set_position(cell_position(slot));

        if(new_page != page)
        {
            page = new_page;
            slot = new_slot;
            draw_page();
        }
        else
        {
            slot = new_slot;
        }

        draw_footer();
    };

    draw_page();
    draw_footer();
    bn::core::update();
    int frame = 0;

    while(! bn::keypad::b_pressed())
    {
        if(bn::keypad::right_pressed())
        {
            slot % columns == columns - 1 ? move_to(page + 1, slot - (columns - 1)) : move_to(page, slot + 1);
        }
        else if(bn::keypad::left_pressed())
        {
            slot % columns == 0 ? move_to(page - 1, slot + (columns - 1)) : move_to(page, slot - 1);
        }
        else if(bn::keypad::down_pressed())
        {
            move_to(page, slot + columns);
        }
        else if(bn::keypad::up_pressed())
        {
            move_to(page, slot - columns);
        }
        else if(bn::keypad::r_pressed())
        {
            move_to(page + 1, slot);
        }
        else if(bn::keypad::l_pressed())
        {
            move_to(page - 1, slot);
        }
        else if(bn::keypad::a_pressed() && profile::has_shiny_form(entry_species(page * per_page + slot)))
        {
            audio::play(bn::sound_items::sfx_menu);
            set_shiny_view(! shiny_view);
        }

        ++frame;
        bn::fixed_point position = cell_position(slot);
        position.set_y(position.y() - 2 + bn::degrees_lut_sin((frame * 8) % 360) * 2);
        icons[slot].set_position(position);
        bn::core::update();
    }

    audio::play(bn::sound_items::sfx_menu);
}

}
