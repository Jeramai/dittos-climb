#include "moves.h"

#include "projectile_frames.h"

namespace
{
    using enum move_pattern;

    constexpr move_data table[] = {
        { "STRUGGLE", "STRUGGLE", pokemon_type::none, 40, 0, wave, 24, 0, 0, 0, 10, 0 },
        { "TRANSFORM", "TRANSFORM", pokemon_type::normal, 0, 0, fail, 60, 0, 0, 0, 0, 0 },
        { "TACKLE", "TACKLE", pokemon_type::normal, 40, 35, dash, 36, 0, 0, 3.5, 10, 0 },
        { "HYPER FANG", "H.FANG", pokemon_type::normal, 80, 15, melee, 40, 0, 0, 0, 8, 0 },
        { "SCRATCH", "SCRATCH", pokemon_type::normal, 40, 35, melee, 16, 0, 0, 0, 6, 0 },
        { "PAY DAY", "PAY DAY", pokemon_type::normal, 40, 20, shot, 30, 3, 20, 2.5, 50, projectile_frames::coin },
    };
}

namespace moves
{

const move_data& get(move_id id)
{
    return table[int(id)];
}

}
