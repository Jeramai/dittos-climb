#include "floor_theme.h"

#include "bn_math.h"

#include "bn_bg_palette_items_cave_palette.h"
#include "bn_bg_palette_items_forest_palette.h"
#include "bn_bg_palette_items_lake_palette.h"
#include "bn_bg_palette_items_plant_palette.h"
#include "bn_bg_palette_items_lab_palette.h"
#include "bn_regular_bg_tiles_items_cave_tiles.h"
#include "bn_regular_bg_tiles_items_forest_tiles.h"
#include "bn_regular_bg_tiles_items_lake_tiles.h"
#include "bn_regular_bg_tiles_items_plant_tiles.h"
#include "bn_regular_bg_tiles_items_lab_tiles.h"

namespace
{
    constexpr floor_theme themes[] = {
        { "CINNABAR LAB", &bn::regular_bg_tiles_items::lab_tiles, &bn::bg_palette_items::lab_palette,
          "The lab doors locked!", "The doors opened!",
          { { species_id::rattata, 45 }, { species_id::meowth, 35 }, { species_id::porygon, 20 } }, 3,
          false, false, false, false, 0, boss_kind::snorlax, species_id::machop },
        { "VIRIDIAN FOREST", &bn::regular_bg_tiles_items::forest_tiles, &bn::bg_palette_items::forest_palette,
          "Vines covered the exits!", "The vines withered away!",
          { { species_id::oddish, 30 }, { species_id::caterpie, 30 }, { species_id::paras, 20 },
            { species_id::beedrill, 20 } }, 4,
          true, false, false, false, 35, boss_kind::venusaur, species_id::charmander },
        { "ROCK TUNNEL", &bn::regular_bg_tiles_items::cave_tiles, &bn::bg_palette_items::cave_palette,
          "Rocks blocked the exits!", "The rocks crumbled away!",
          { { species_id::geodude, 35 }, { species_id::zubat, 35 }, { species_id::diglett, 30 } }, 3,
          false, true, false, false, 0, boss_kind::onix, species_id::bulbasaur },
        { "UNDERGROUND LAKE", &bn::regular_bg_tiles_items::lake_tiles, &bn::bg_palette_items::lake_palette,
          "The water rose over the exits!", "The water drained away!",
          { { species_id::magikarp, 30 }, { species_id::poliwag, 25 }, { species_id::staryu, 25 },
            { species_id::horsea, 20 } }, 4,
          false, false, true, false, 0, boss_kind::gyarados, species_id::pikachu },
        { "POWER PLANT", &bn::regular_bg_tiles_items::plant_tiles, &bn::bg_palette_items::plant_palette,
          "The doors are electrified!", "The doors lost power!",
          { { species_id::pikachu, 35 }, { species_id::voltorb, 30 }, { species_id::magnemite, 35 } }, 3,
          false, false, false, true, 0, boss_kind::zapdos, species_id::sandshrew },
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
