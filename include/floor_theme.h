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
    int overgrown_percent;
    boss_kind boss;
};

namespace floor_themes
{
    [[nodiscard]] const floor_theme& get(int floor_number);

    [[nodiscard]] species_id pick_species(const floor_theme& theme, int roll);
}

#endif
