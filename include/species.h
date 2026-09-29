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
    geodude,
    diglett,
    zubat,
    onix,
    magikarp,
    poliwag,
    staryu,
    horsea,
    gyarados,
    pikachu,
    voltorb,
    magnemite,
    zapdos,
};

namespace species_frames
{
    constexpr int walk = 0;
    constexpr int white = 2;
    constexpr int own_walk = 3;
    constexpr int ditto_flat = 5;
    constexpr int asleep = 5;
    constexpr int charging = 5;
    constexpr int mound = 5;
}

enum class species_behavior
{
    normal,
    burrower,
    flyer,
    aquatic,
};

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
    species_behavior behavior = species_behavior::normal;
};

namespace species
{
    [[nodiscard]] const species_data& get(species_id id);

    [[nodiscard]] bool can_swim(const species_data& data);
}

#endif
