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
    cloud,
    beam,
    dig,
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
    absorb,
    stun_spore,
    string_shot,
    leech_life,
    twineedle,
    fury_attack,
    razor_leaf,
    solar_beam,
    sleep_powder,
    rock_throw,
    rollout,
    dig,
    supersonic,
    slam,
};

enum class status_effect
{
    none,
    paralysis,
    poison,
    sleep,
    confusion,
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
    status_effect status = status_effect::none;
    int status_chance = 0;
    bool drain = false;
};

namespace moves
{
    [[nodiscard]] const move_data& get(move_id id);
}

#endif
