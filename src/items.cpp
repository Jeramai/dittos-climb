#include "items.h"

namespace
{
    constexpr item_data table[] = {
        { "CHARCOAL", item_kind::type_boost, pokemon_type::fire, 60, "Held: boosts FIRE moves." },
        { "MIRACLE SEED", item_kind::type_boost, pokemon_type::grass, 60, "Held: boosts GRASS moves." },
        { "MYSTIC WATER", item_kind::type_boost, pokemon_type::water, 60, "Held: boosts WATER moves." },
        { "MAGNET", item_kind::type_boost, pokemon_type::electric, 60, "Held: boosts ELECTRIC moves." },
        { "HARD STONE", item_kind::type_boost, pokemon_type::rock, 60, "Held: boosts ROCK moves." },
        { "SOFT SAND", item_kind::type_boost, pokemon_type::ground, 60, "Held: boosts GROUND moves." },
        { "SILVERPOWDER", item_kind::type_boost, pokemon_type::bug, 60, "Held: boosts BUG moves." },
        { "BLACK BELT", item_kind::type_boost, pokemon_type::fighting, 60, "Held: boosts FIGHTING moves." },
        { "LEFTOVERS", item_kind::leftovers, pokemon_type::none, 150, "Held: slowly restores HP." },
        { "QUICK CLAW", item_kind::quick_claw, pokemon_type::none, 100, "Held: moves recharge faster." },
        { "ETHER", item_kind::ether, pokemon_type::none, 40, "Restores the PP of move B." },
        { "RARE CANDY", item_kind::rare_candy, pokemon_type::none, 120, "Raises DITTO's max HP." },
        { "POTION", item_kind::potion, pokemon_type::none, 30, "Restores half of the HP." },
    };

    static_assert(sizeof(table) / sizeof(table[0]) == items::count);
}

namespace items
{

const item_data& get(item_id id)
{
    return table[int(id)];
}

}
