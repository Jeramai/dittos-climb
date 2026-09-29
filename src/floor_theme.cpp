#include "floor_theme.h"

#include "bn_math.h"

#include "bn_bg_palette_items_cave_palette.h"
#include "bn_bg_palette_items_chasm_palette.h"
#include "bn_bg_palette_items_den_palette.h"
#include "bn_bg_palette_items_dojo_palette.h"
#include "bn_bg_palette_items_forest_palette.h"
#include "bn_bg_palette_items_hideout_palette.h"
#include "bn_bg_palette_items_ice_palette.h"
#include "bn_bg_palette_items_lake_palette.h"
#include "bn_bg_palette_items_peak_palette.h"
#include "bn_bg_palette_items_plant_palette.h"
#include "bn_bg_palette_items_tower_palette.h"
#include "bn_bg_palette_items_volcano_palette.h"
#include "bn_bg_palette_items_lab_palette.h"
#include "bn_regular_bg_tiles_items_cave_tiles.h"
#include "bn_regular_bg_tiles_items_chasm_tiles.h"
#include "bn_regular_bg_tiles_items_den_tiles.h"
#include "bn_regular_bg_tiles_items_dojo_tiles.h"
#include "bn_regular_bg_tiles_items_forest_tiles.h"
#include "bn_regular_bg_tiles_items_hideout_tiles.h"
#include "bn_regular_bg_tiles_items_ice_tiles.h"
#include "bn_regular_bg_tiles_items_lake_tiles.h"
#include "bn_regular_bg_tiles_items_peak_tiles.h"
#include "bn_regular_bg_tiles_items_plant_tiles.h"
#include "bn_regular_bg_tiles_items_tower_tiles.h"
#include "bn_regular_bg_tiles_items_volcano_tiles.h"
#include "bn_regular_bg_tiles_items_lab_tiles.h"

namespace
{
    constexpr floor_theme themes[] = {
        { "CINNABAR LAB", &bn::regular_bg_tiles_items::lab_tiles, &bn::bg_palette_items::lab_palette,
          "The lab doors locked!", "The doors opened!",
          { { species_id::rattata, 45 }, { species_id::meowth, 35 }, { species_id::porygon, 20 } }, 3,
          false, false, false, hazard_kind::none, false, false, gate_kind::none, false, false, key_item::poke_flute, 1, false, false, 0, boss_kind::snorlax, species_id::machop },
        { "VIRIDIAN FOREST", &bn::regular_bg_tiles_items::forest_tiles, &bn::bg_palette_items::forest_palette,
          "Vines covered the exits!", "The vines withered away!",
          { { species_id::oddish, 30 }, { species_id::caterpie, 30 }, { species_id::paras, 20 },
            { species_id::beedrill, 20 } }, 4,
          true, false, false, hazard_kind::none, false, false, gate_kind::bush, false, false, key_item::none, 1, false, false, 35, boss_kind::venusaur, species_id::charmander },
        { "ROCK TUNNEL", &bn::regular_bg_tiles_items::cave_tiles, &bn::bg_palette_items::cave_palette,
          "Rocks blocked the exits!", "The rocks crumbled away!",
          { { species_id::geodude, 35 }, { species_id::zubat, 35 }, { species_id::diglett, 30 } }, 3,
          false, true, false, hazard_kind::none, false, false, gate_kind::none, false, false, key_item::none, 1, false, false, 0, boss_kind::onix, species_id::bulbasaur },
        { "UNDERGROUND LAKE", &bn::regular_bg_tiles_items::lake_tiles, &bn::bg_palette_items::lake_palette,
          "The water rose over the exits!", "The water drained away!",
          { { species_id::magikarp, 30 }, { species_id::poliwag, 25 }, { species_id::staryu, 25 },
            { species_id::horsea, 20 } }, 4,
          false, false, true, hazard_kind::none, false, false, gate_kind::none, false, false, key_item::none, 1, false, false, 0, boss_kind::gyarados, species_id::pikachu },
        { "POWER PLANT", &bn::regular_bg_tiles_items::plant_tiles, &bn::bg_palette_items::plant_palette,
          "The doors are electrified!", "The doors lost power!",
          { { species_id::pikachu, 35 }, { species_id::voltorb, 30 }, { species_id::magnemite, 35 } }, 3,
          false, false, false, hazard_kind::electric, false, false, gate_kind::none, false, false, key_item::none, 1, false, false, 0, boss_kind::zapdos, species_id::sandshrew },
        { "VOLCANO", &bn::regular_bg_tiles_items::volcano_tiles, &bn::bg_palette_items::volcano_palette,
          "Magma sealed the exits!", "The magma cooled down!",
          { { species_id::vulpix, 25 }, { species_id::ponyta, 25 }, { species_id::growlithe, 25 },
            { species_id::magmar, 25 } }, 4,
          false, false, false, hazard_kind::lava, true, false, gate_kind::none, false, false, key_item::none, 1, false, false, 0, boss_kind::moltres,
          species_id::squirtle },
        { "SEAFOAM CAVE", &bn::regular_bg_tiles_items::ice_tiles, &bn::bg_palette_items::ice_palette,
          "Ice froze over the exits!", "The ice cracked open!",
          { { species_id::seel, 35 }, { species_id::jynx, 30 }, { species_id::shellder, 35 } }, 3,
          false, false, false, hazard_kind::none, false, true, gate_kind::ice, false, false, key_item::none, 1, false, false, 35, boss_kind::articuno,
          species_id::omanyte },
        { "THE CHASM", &bn::regular_bg_tiles_items::chasm_tiles, &bn::bg_palette_items::chasm_palette,
          "A gale sealed the exits!", "The wind calmed down!",
          { { species_id::pidgey, 40 }, { species_id::spearow, 35 }, { species_id::aerodactyl, 25 } }, 3,
          false, false, false, hazard_kind::none, false, false, gate_kind::none, true, false, key_item::none, 1, false, false, 0, boss_kind::pidgeot,
          species_id::kabuto },
        { "ROCKET HIDEOUT", &bn::regular_bg_tiles_items::hideout_tiles, &bn::bg_palette_items::hideout_palette,
          "The shutters slammed shut!", "The shutters opened!",
          { { species_id::koffing, 35 }, { species_id::ekans, 35 }, { species_id::grimer, 30 } }, 3,
          false, false, false, hazard_kind::gas, false, false, gate_kind::none, false, true, key_item::silph_scope, 1, false, false, 0,
          boss_kind::team_rocket, species_id::slowpoke },
        { "FIGHTING DOJO", &bn::regular_bg_tiles_items::dojo_tiles, &bn::bg_palette_items::dojo_palette,
          "The dojo doors slid shut!", "The dojo doors slid open!",
          { { species_id::mankey, 35 }, { species_id::machop, 35 }, { species_id::machoke, 30 } }, 3,
          false, false, false, hazard_kind::none, false, false, gate_kind::cracked, false, false, key_item::none, 3,
          true, false, 35, boss_kind::hitmon, species_id::farfetchd },
        { "POKEMON TOWER", &bn::regular_bg_tiles_items::tower_tiles, &bn::bg_palette_items::tower_palette,
          "A chill sealed the exits!", "The chill faded away!",
          { { species_id::gastly, 40 }, { species_id::haunter, 30 }, { species_id::cubone, 30 } }, 3,
          false, true, false, hazard_kind::none, false, false, gate_kind::spirit, false, false, key_item::none, 1,
          false, true, 35, boss_kind::gengar, species_id::exeggcute },
        { "DRAGON'S DEN", &bn::regular_bg_tiles_items::den_tiles, &bn::bg_palette_items::den_palette,
          "The water surged over the exits!", "The water calmed down!",
          { { species_id::dratini, 40 }, { species_id::dragonair, 30 }, { species_id::seadra, 30 } }, 3,
          false, false, true, hazard_kind::none, false, false, gate_kind::none, false, false, key_item::none, 1,
          false, false, 0, boss_kind::dragonite, species_id::lapras, true, true, false },
        { "CERULEAN CAVE", &bn::regular_bg_tiles_items::peak_tiles, &bn::bg_palette_items::peak_palette,
          "A strange power sealed the exits!", "The strange power faded!",
          { { species_id::abra, 30 }, { species_id::kadabra, 30 }, { species_id::drowzee, 40 } }, 3,
          false, false, false, hazard_kind::none, false, false, gate_kind::none, false, false, key_item::none, 1,
          false, false, 0, boss_kind::mewtwo, species_id::venomoth, false, false, true },
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
