#include "types.h"

namespace
{
    constexpr int type_count = 16;

    // Row: attacker, column: defender, in pokemon_type order. 0 immune, 1 = ×½, 2 = ×1, 4 = ×2.
    constexpr int chart[type_count][type_count] = {
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 2 },
        { 2, 2, 1, 1, 2, 4, 4, 2, 2, 2, 2, 2, 4, 1, 2, 1 },
        { 2, 2, 4, 1, 2, 1, 2, 2, 2, 4, 2, 2, 2, 4, 2, 1 },
        { 2, 2, 2, 4, 1, 1, 2, 2, 2, 0, 4, 2, 2, 2, 2, 1 },
        { 2, 2, 1, 4, 2, 1, 2, 2, 1, 4, 1, 2, 1, 4, 2, 1 },
        { 2, 2, 1, 1, 2, 4, 1, 2, 2, 4, 4, 2, 2, 2, 2, 4 },
        { 2, 4, 2, 2, 2, 2, 4, 2, 1, 2, 1, 1, 1, 4, 0, 2 },
        { 2, 2, 2, 2, 2, 4, 2, 2, 1, 1, 2, 2, 2, 1, 1, 2 },
        { 2, 2, 4, 2, 4, 1, 2, 2, 4, 2, 0, 2, 1, 4, 2, 2 },
        { 2, 2, 2, 2, 1, 4, 2, 4, 2, 2, 2, 2, 4, 1, 2, 2 },
        { 2, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 1, 2, 2, 2, 2 },
        { 2, 2, 1, 2, 2, 4, 2, 1, 1, 2, 1, 4, 2, 2, 1, 2 },
        { 2, 2, 4, 2, 2, 2, 4, 1, 2, 1, 4, 2, 4, 2, 2, 2 },
        { 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 4, 2 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4 },
    };

    int single(pokemon_type attack, pokemon_type defender)
    {
        return chart[int(attack)][int(defender)];
    }
}

namespace types
{

int effectiveness(pokemon_type attack, pokemon_type defender_1, pokemon_type defender_2)
{
    return single(attack, defender_1) * single(attack, defender_2);
}


const char* name(pokemon_type type)
{
    constexpr const char* names[] = {
        "", "NORMAL", "FIRE", "WATER", "ELECTRIC", "GRASS", "ICE", "FIGHTING", "POISON", "GROUND", "FLYING",
        "PSYCHIC", "BUG", "ROCK", "GHOST", "DRAGON",
    };

    return names[int(type)];
}
}
