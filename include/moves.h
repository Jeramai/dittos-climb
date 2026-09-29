#ifndef MOVES_H
#define MOVES_H

#include "bn_fixed.h"

#include "types.h"

enum class move_pattern
{
    shot,
    melee,
    dash,
    wave,
    fail,
};

enum class move_id
{
    struggle,
    transform,
    tackle,
    hyper_fang,
    scratch,
    pay_day,
    tri_attack,
    psybeam,
    headbutt,
    body_slam,
};

struct move_data
{
    const char* name;
    const char* short_name;
    pokemon_type type;
    int power;
    int pp;
    move_pattern pattern;
    int cooldown;
    int shots;
    int spread_degrees;
    bn::fixed speed;
    int life;
    int projectile_frame;
};

namespace moves
{
    [[nodiscard]] const move_data& get(move_id id);
}

#endif
