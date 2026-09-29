#include "floor_theme.h"

#include "bn_math.h"

#include "bn_bg_palette_items_cave_palette.h"
#include "bn_bg_palette_items_forest_palette.h"
#include "bn_bg_palette_items_lab_palette.h"
#include "bn_regular_bg_tiles_items_cave_tiles.h"
#include "bn_regular_bg_tiles_items_forest_tiles.h"
#include "bn_regular_bg_tiles_items_lab_tiles.h"

namespace
{
    constexpr floor_theme themes[] = {
        { "CINNABAR LAB", &bn::regular_bg_tiles_items::lab_tiles, &bn::bg_palette_items::lab_palette,
          "The lab doors locked!", "The doors opened!",
          { { species_id::rattata, 45 }, { species_id::meowth, 35 }, { species_id::porygon, 20 } }, 3,
          false, false, 0, boss_kind::snorlax },
        { "VIRIDIAN FOREST", &bn::regular_bg_tiles_items::forest_tiles, &bn::bg_palette_items::forest_palette,
          "Vines covered the exits!", "The vines withered away!",
          { { species_id::oddish, 30 }, { species_id::caterpie, 30 }, { species_id::paras, 20 },
            { species_id::beedrill, 20 } }, 4,
          true, false, 35, boss_kind::venusaur },
        { "ROCK TUNNEL", &bn::regular_bg_tiles_items::cave_tiles, &bn::bg_palette_items::cave_palette,
          "Rocks blocked the exits!", "The rocks crumbled away!",
          { { species_id::geodude, 35 }, { species_id::zubat, 35 }, { species_id::diglett, 30 } }, 3,
          false, true, 0, boss_kind::onix },
    };

    constexpr int theme_count = sizeof(themes) / sizeof(themes[0]);
}

namespace floor_themes
{

const floor_theme& get(int floor_number)
{
    return themes[bn::min(floor_number, theme_count) - 1];
}

species_id pick_species(const floor_theme& theme, int roll)
{
    for(int index = 0; index < theme.spawn_count; ++index)
    {
        roll -= theme.spawns[index].weight;

        if(roll < 0)
        {
            return theme.spawns[index].species;
        }
    }

    return theme.spawns[0].species;
}

}
