#include "combat.h"

namespace combat
{

attack make_attack(move_id move, pokemon_type user_type_1, pokemon_type user_type_2)
{
    const move_data& data = moves::get(move);
    bool stab = data.type != pokemon_type::none && (data.type == user_type_1 || data.type == user_type_2);
    return attack{ move, data.type, stab ? data.power * 3 / 2 : data.power };
}

hit_result resolve(const attack& value, pokemon_type target_type_1, pokemon_type target_type_2)
{
    int effectiveness = types::effectiveness(value.type, target_type_1, target_type_2);

    if(! effectiveness)
    {
        return hit_result{ 0, 0 };
    }

    if(! value.power)
    {
        return hit_result{ 0, effectiveness };
    }

    int damage = value.power * effectiveness / (10 * types::neutral);
    return hit_result{ damage > 0 ? damage : 1, effectiveness };
}

bn::string_view effectiveness_message(int effectiveness)
{
    if(! effectiveness)
    {
        return "It doesn't affect the foe...";
    }

    if(effectiveness > types::neutral)
    {
        return "It's super effective!";
    }

    if(effectiveness < types::neutral)
    {
        return "It's not very effective...";
    }

    return "";
}

}
