#include "moves.h"

#include "bn_sprite_items_electric_projectiles.h"
#include "bn_sprite_items_water_projectiles.h"

#include "projectile_frames.h"

namespace
{
    using enum move_pattern;

    constexpr const bn::sprite_item* water = &bn::sprite_items::water_projectiles;
    constexpr const bn::sprite_item* electric = &bn::sprite_items::electric_projectiles;
    constexpr status_effect no_status = status_effect::none;

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
        { "ROCK THROW", "R.THROW", pokemon_type::rock, 50, 0, shot, 30, 1, 0, 2, 50, projectile_frames::rock },
        { "ROLLOUT", "ROLLOUT", pokemon_type::rock, 60, 15, dash, 50, 0, 0, 3.5, 18, 0 },
        { "DIG", "DIG", pokemon_type::ground, 80, 10, dig, 50, 0, 0, 3, 20, 0 },
        { "SUPERSONIC", "SUPERSON", pokemon_type::normal, 0, 15, shot, 45, 1, 0, 2, 50, projectile_frames::supersonic,
          status_effect::confusion, 100 },
        { "SLAM", "SLAM", pokemon_type::normal, 80, 10, dash, 45, 0, 0, 3, 14, 0 },
        { "SPLASH", "SPLASH", pokemon_type::water, 0, 0, fail, 40, 0, 0, 0, 0, 0 },
        { "BUBBLE", "BUBBLE", pokemon_type::water, 25, 0, shot, 24, 2, 20, 1.5, 60, water_frames::bubble,
          no_status, 0, false, water },
        { "HYPNOSIS", "HYPNOSIS", pokemon_type::psychic, 0, 15, shot, 50, 1, 0, 1.8, 50, projectile_frames::psybeam,
          status_effect::sleep, 100 },
        { "WATER GUN", "W.GUN", pokemon_type::water, 40, 0, shot, 22, 1, 0, 3, 40, water_frames::drop,
          no_status, 0, false, water },
        { "SWIFT", "SWIFT", pokemon_type::normal, 25, 15, shot, 40, 5, 15, 2.5, 45, water_frames::star,
          no_status, 0, false, water },
        { "BUBBLEBEAM", "B.BEAM", pokemon_type::water, 45, 15, shot, 36, 3, 12, 2.5, 45, water_frames::bubble,
          status_effect::paralysis, 30, false, water },
        { "BITE", "BITE", pokemon_type::normal, 60, 0, melee, 26, 0, 0, 0, 8, 0 },
        { "HYDRO PUMP", "H.PUMP", pokemon_type::water, 110, 5, beam, 80, 7, 0, 5, 24, water_frames::drop,
          no_status, 0, false, water },
        { "DRAGON RAGE", "D.RAGE", pokemon_type::dragon, 40, 10, shot, 40, 3, 20, 2, 60, water_frames::dragon,
          no_status, 0, false, water },
        { "THUNDERSHOCK", "T.SHOCK", pokemon_type::electric, 40, 0, shot, 22, 1, 0, 3, 40, electric_frames::spark,
          status_effect::paralysis, 10, false, electric },
        { "QUICK ATTACK", "QUICK", pokemon_type::normal, 40, 30, dash, 30, 0, 0, 4.5, 10, 0 },
        { "SONICBOOM", "S.BOOM", pokemon_type::normal, 40, 0, shot, 26, 1, 0, 2.5, 45, electric_frames::wave,
          no_status, 0, false, electric },
        { "SELFDESTRUCT", "S.DESTR", pokemon_type::normal, 130, 1, explode, 60, 0, 0, 0, 8, 0 },
        { "THUNDER WAVE", "T.WAVE", pokemon_type::electric, 0, 20, shot, 45, 1, 0, 2, 50, electric_frames::wave,
          status_effect::paralysis, 100, false, electric },
        { "THUNDER", "THUNDER", pokemon_type::electric, 110, 5, beam, 80, 7, 0, 5, 24, electric_frames::bolt,
          status_effect::paralysis, 30, false, electric },
        { "DRILL PECK", "D.PECK", pokemon_type::flying, 80, 15, dash, 45, 0, 0, 3.5, 14, 0 },
        { "KARATE CHOP", "K.CHOP", pokemon_type::fighting, 50, 0, melee, 18, 0, 0, 0, 6, 0 },
        { "LOW KICK", "LOW KICK", pokemon_type::fighting, 60, 15, dash, 40, 0, 0, 3.5, 12, 0 },
        { "EMBER", "EMBER", pokemon_type::fire, 40, 0, shot, 22, 1, 0, 2.8, 40, electric_frames::ember,
          no_status, 0, false, electric },
        { "FLAMETHROWER", "F.THROW", pokemon_type::fire, 55, 10, shot, 40, 3, 6, 3, 40, electric_frames::ember,
          no_status, 0, false, electric },
        { "VINE WHIP", "V.WHIP", pokemon_type::grass, 45, 0, melee, 18, 0, 0, 0, 6, 0 },
        { "LEECH SEED", "L.SEED", pokemon_type::grass, 30, 10, shot, 36, 1, 0, 2, 50, projectile_frames::leaf,
          no_status, 0, true },
        { "FIRE SPIN", "F.SPIN", pokemon_type::fire, 35, 15, cloud, 50, 1, 0, 0.9, 80, cloud_frames::fire },
        { "CONFUSE RAY", "C.RAY", pokemon_type::ghost, 0, 15, shot, 50, 1, 0, 1.8, 50, projectile_frames::psybeam,
          status_effect::confusion, 100 },
        { "FIRE PUNCH", "F.PUNCH", pokemon_type::fire, 55, 0, melee, 20, 0, 0, 0, 6, 0 },
        { "SMOG", "SMOG", pokemon_type::poison, 20, 20, cloud, 50, 1, 0, 0.8, 90, cloud_frames::smog,
          status_effect::poison, 40 },
        { "SKY ATTACK", "SKY ATK", pokemon_type::flying, 140, 5, dash, 70, 0, 0, 4.5, 16, 0 },
        { "AURORA BEAM", "AURORA", pokemon_type::ice, 50, 15, shot, 34, 1, 0, 2.8, 45, water_frames::ice_shard,
          status_effect::freeze, 10, false, water },
        { "ICE PUNCH", "I.PUNCH", pokemon_type::ice, 55, 0, melee, 20, 0, 0, 0, 6, 0, status_effect::freeze, 10 },
        { "LOVELY KISS", "L.KISS", pokemon_type::normal, 0, 10, shot, 50, 1, 0, 1.8, 50, water_frames::heart,
          status_effect::sleep, 100, false, water },
        { "CLAMP", "CLAMP", pokemon_type::water, 35, 0, melee, 18, 0, 0, 0, 6, 0 },
        { "ICE BEAM", "I.BEAM", pokemon_type::ice, 90, 10, beam, 70, 6, 0, 5, 24, water_frames::ice_shard,
          status_effect::freeze, 30, false, water },
        { "ROCK SLIDE", "R.SLIDE", pokemon_type::rock, 60, 10, shot, 40, 3, 12, 2.4, 50, projectile_frames::rock },
        { "ICE SHARD", "I.SHARD", pokemon_type::ice, 40, 0, shot, 20, 1, 0, 3.4, 40, water_frames::ice_shard,
          no_status, 0, false, water },
        { "BLIZZARD", "BLIZZARD", pokemon_type::ice, 60, 5, cloud, 70, 1, 0, 1, 90, cloud_frames::snow,
          status_effect::freeze, 30 },
    };
}

namespace moves
{

const move_data& get(move_id id)
{
    return table[int(id)];
}

}
