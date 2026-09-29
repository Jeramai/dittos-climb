#ifndef SPECIES_H
#define SPECIES_H

#include "bn_fixed.h"
#include "bn_sprite_item.h"

#include "moves.h"

enum class species_id
{
    ditto,
    rattata,
    meowth,
    porygon,
    snorlax,
    oddish,
    caterpie,
    paras,
    beedrill,
    venusaur,
};

namespace species_frames
{
    constexpr int walk = 0;
    constexpr int white = 2;
    constexpr int own_walk = 3;
    constexpr int ditto_flat = 5;
    constexpr int asleep = 5;
    constexpr int charging = 5;
}

struct species_data
{
    const char* name;
    pokemon_type type_1;
    pokemon_type type_2;
    int hp;
    bn::fixed speed;
    move_id move_a;
    move_id move_b;
    const bn::sprite_item* sprite;
};

namespace species
{
    [[nodiscard]] const species_data& get(species_id id);
}

#endif
