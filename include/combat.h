#ifndef COMBAT_H
#define COMBAT_H

#include "bn_string_view.h"

#include "moves.h"

struct attack
{
    move_id move;
    pokemon_type type;
    int power;
};

struct hit_result
{
    int damage;
    int effectiveness;
};

namespace combat
{
    [[nodiscard]] attack make_attack(move_id move, pokemon_type user_type_1, pokemon_type user_type_2);

    [[nodiscard]] hit_result resolve(const attack& value, pokemon_type target_type_1, pokemon_type target_type_2);

    [[nodiscard]] bn::string_view effectiveness_message(int effectiveness);
}

#endif
