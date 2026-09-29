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
        { "TRI ATTACK", "TRI ATK", pokemon_type::normal, 30, 25, shot, 26, 3, 25, 2.2, 45, projectile_frames::tri },
        { "PSYBEAM", "PSYBEAM", pokemon_type::psychic, 65, 15, shot, 30, 1, 0, 3.5, 40, projectile_frames::psybeam },
        { "HEADBUTT", "HEADBUTT", pokemon_type::normal, 70, 20, melee, 30, 0, 0, 0, 8, 0 },
        { "BODY SLAM", "B.SLAM", pokemon_type::normal, 85, 10, dash, 50, 0, 0, 3, 14, 0 },
        { "ABSORB", "ABSORB", pokemon_type::grass, 30, 0, shot, 20, 1, 0, 2.5, 35, projectile_frames::leaf,
          status_effect::none, 0, true },
        { "STUN SPORE", "STUN SPR", pokemon_type::grass, 0, 10, cloud, 60, 1, 0, 0.8, 80, cloud_frames::stun,
          status_effect::paralysis, 100 },
        { "STRING SHOT", "STRING", pokemon_type::bug, 10, 20, shot, 30, 1, 0, 2.5, 40, projectile_frames::string,
          status_effect::paralysis, 100 },
        { "LEECH LIFE", "LEECH", pokemon_type::bug, 50, 15, melee, 30, 0, 0, 0, 8, 0, status_effect::none, 0, true },
        { "TWINEEDLE", "TWINEEDL", pokemon_type::bug, 25, 0, shot, 22, 2, 10, 3.2, 35, projectile_frames::needle,
          status_effect::poison, 20 },
        { "FURY ATTACK", "FURY", pokemon_type::normal, 60, 15, dash, 40, 0, 0, 3.5, 12, 0 },
        { "RAZOR LEAF", "R.LEAF", pokemon_type::grass, 35, 0, shot, 20, 3, 15, 3, 40, projectile_frames::leaf },
        { "SOLAR BEAM", "S.BEAM", pokemon_type::grass, 120, 5, beam, 80, 6, 0, 5, 24, projectile_frames::beam },
        { "SLEEP POWDER", "SLEEP", pokemon_type::grass, 0, 10, cloud, 60, 1, 0, 0.5, 150, cloud_frames::sleep,
          status_effect::sleep, 100 },
    };
}

namespace moves
{

const move_data& get(move_id id)
{
    return table[int(id)];
}

}
