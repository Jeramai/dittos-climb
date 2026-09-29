#include "items.h"

namespace
{
    constexpr item_data table[] = {
        { "CHARCOAL", item_kind::type_boost, pokemon_type::fire },
        { "MIRACLE SEED", item_kind::type_boost, pokemon_type::grass },
        { "MYSTIC WATER", item_kind::type_boost, pokemon_type::water },
        { "MAGNET", item_kind::type_boost, pokemon_type::electric },
        { "HARD STONE", item_kind::type_boost, pokemon_type::rock },
        { "SOFT SAND", item_kind::type_boost, pokemon_type::ground },
        { "SILVERPOWDER", item_kind::type_boost, pokemon_type::bug },
        { "BLACK BELT", item_kind::type_boost, pokemon_type::fighting },
        { "LEFTOVERS", item_kind::leftovers, pokemon_type::none },
        { "QUICK CLAW", item_kind::quick_claw, pokemon_type::none },
        { "ETHER", item_kind::ether, pokemon_type::none },
        { "RARE CANDY", item_kind::rare_candy, pokemon_type::none },
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
