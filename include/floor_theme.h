#ifndef FLOOR_THEME_H
#define FLOOR_THEME_H

#include "bn_bg_palette_item.h"
#include "bn_regular_bg_tiles_item.h"

#include "species.h"

enum class boss_kind
{
    none,
    snorlax,
    venusaur,
    onix,
    gyarados,
    zapdos,
    moltres,
    articuno,
    pidgeot,
    team_rocket,
    hitmon,
};

enum class key_item
{
    none,
    poke_flute,
    silph_scope,
};

enum class gate_kind
{
    none,
    bush,
    ice,
    cracked,
};

enum class hazard_kind
{
    none,
    electric,
    lava,
    gas,
};

struct spawn_weight
{
    species_id species;
    int weight;
};

struct floor_theme
{
    const char* name;
    const bn::regular_bg_tiles_item* tiles;
    const bn::bg_palette_item* palette;
    const char* lock_message;
    const char* unlock_message;
    spawn_weight spawns[4];
    int spawn_count;
    bool tall_grass;
    bool dark;
    bool water;
    hazard_kind hazard;
    bool ember_rain;
    bool ice_floor;
    gate_kind gate;
    bool chasm;
    bool spinners;
    key_item key;
    int waves;
    bool no_items;
    int overgrown_percent;
    boss_kind boss;
    species_id rare;
};

namespace floor_themes
{
    [[nodiscard]] const floor_theme& get(int floor_number);

    [[nodiscard]] species_id pick_species(const floor_theme& theme, int roll);
}

#endif
