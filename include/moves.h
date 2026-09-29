#ifndef MOVES_H
#define MOVES_H

#include "bn_fixed.h"
#include "bn_sprite_item.h"

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
    explode,
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
    splash,
    bubble,
    hypnosis,
    water_gun,
    swift,
    bubblebeam,
    bite,
    hydro_pump,
    dragon_rage,
    thundershock,
    quick_attack,
    sonicboom,
    selfdestruct,
    thunder_wave,
    thunder,
    drill_peck,
    karate_chop,
    low_kick,
    ember,
    flamethrower,
    vine_whip,
    leech_seed,
    fire_spin,
    confuse_ray,
    fire_punch,
    smog,
    sky_attack,
    aurora_beam,
    ice_punch,
    lovely_kiss,
    clamp,
    ice_beam,
    rock_slide,
    ice_shard,
    blizzard,
    gust,
    peck,
    wing_attack,
    whirlwind,
    poison_sting,
    glare,
    sludge,
    poison_gas,
    wrap,
    confusion,
    rolling_kick,
    hi_jump_kick,
    thunderpunch,
    mega_punch,
    submission,
    thrash,
};

enum class status_effect
{
    none,
    paralysis,
    poison,
    sleep,
    confusion,
    freeze,
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
    const bn::sprite_item* sheet = nullptr;
};

namespace moves
{
    [[nodiscard]] const move_data& get(move_id id);
}

#endif
