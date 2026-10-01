#ifndef TYPES_H
#define TYPES_H

enum class pokemon_type
{
    none,
    normal,
    fire,
    water,
    electric,
    grass,
    ice,
    fighting,
    poison,
    ground,
    flying,
    psychic,
    bug,
    rock,
    ghost,
    dragon,
};

namespace types
{
    constexpr int neutral = 4;

    // Result in quarters: 0 immune, 1 = ×¼, 2 = ×½, 4 = ×1, 8 = ×2, 16 = ×4.
    [[nodiscard]] int effectiveness(pokemon_type attack, pokemon_type defender_1, pokemon_type defender_2);

    [[nodiscard]] const char* name(pokemon_type type);
}

#endif
