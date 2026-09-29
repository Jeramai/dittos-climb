#ifndef ITEMS_H
#define ITEMS_H

#include "types.h"

enum class item_id
{
    charcoal,
    miracle_seed,
    mystic_water,
    magnet,
    hard_stone,
    soft_sand,
    silver_powder,
    black_belt,
    leftovers,
    quick_claw,
    ether,
    rare_candy,
};

enum class item_kind
{
    type_boost,
    leftovers,
    quick_claw,
    ether,
    rare_candy,
};

struct item_data
{
    const char* name;
    item_kind kind;
    pokemon_type boosted_type;
};

namespace items
{
    constexpr int count = 12;

    [[nodiscard]] const item_data& get(item_id id);

    [[nodiscard]] constexpr bool held(item_kind kind)
    {
        return kind != item_kind::ether && kind != item_kind::rare_candy;
    }
}

#endif
