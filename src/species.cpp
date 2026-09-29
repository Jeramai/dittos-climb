#include "species.h"

#include "bn_sprite_items_beedrill.h"
#include "bn_sprite_items_caterpie.h"
#include "bn_sprite_items_diglett.h"
#include "bn_sprite_items_ditto.h"
#include "bn_sprite_items_geodude.h"
#include "bn_sprite_items_meowth.h"
#include "bn_sprite_items_oddish.h"
#include "bn_sprite_items_onix.h"
#include "bn_sprite_items_paras.h"
#include "bn_sprite_items_porygon.h"
#include "bn_sprite_items_rattata.h"
#include "bn_sprite_items_snorlax.h"
#include "bn_sprite_items_venusaur.h"
#include "bn_sprite_items_zubat.h"

namespace
{
    constexpr species_data table[] = {
        { "DITTO", pokemon_type::normal, pokemon_type::none, 40, 1.25, move_id::struggle, move_id::transform,
          &bn::sprite_items::ditto },
        { "RATTATA", pokemon_type::normal, pokemon_type::none, 14, 1.5, move_id::tackle, move_id::hyper_fang,
          &bn::sprite_items::rattata },
        { "MEOWTH", pokemon_type::normal, pokemon_type::none, 16, 1.25, move_id::scratch, move_id::pay_day,
          &bn::sprite_items::meowth },
        { "PORYGON", pokemon_type::normal, pokemon_type::none, 18, 1, move_id::tri_attack, move_id::psybeam,
          &bn::sprite_items::porygon },
        { "SNORLAX", pokemon_type::normal, pokemon_type::none, 60, 0.8, move_id::headbutt, move_id::body_slam,
          &bn::sprite_items::snorlax },
        { "ODDISH", pokemon_type::grass, pokemon_type::poison, 16, 0.8, move_id::absorb, move_id::stun_spore,
          &bn::sprite_items::oddish },
        { "CATERPIE", pokemon_type::bug, pokemon_type::none, 14, 0.9, move_id::tackle, move_id::string_shot,
          &bn::sprite_items::caterpie },
        { "PARAS", pokemon_type::bug, pokemon_type::grass, 16, 0.7, move_id::scratch, move_id::leech_life,
          &bn::sprite_items::paras },
        { "BEEDRILL", pokemon_type::bug, pokemon_type::poison, 18, 1.6, move_id::twineedle, move_id::fury_attack,
          &bn::sprite_items::beedrill },
        { "VENUSAUR", pokemon_type::grass, pokemon_type::poison, 60, 0.7, move_id::razor_leaf, move_id::solar_beam,
          &bn::sprite_items::venusaur },
        { "GEODUDE", pokemon_type::rock, pokemon_type::ground, 20, 0.6, move_id::rock_throw, move_id::rollout,
          &bn::sprite_items::geodude },
        { "DIGLETT", pokemon_type::ground, pokemon_type::none, 12, 1.4, move_id::scratch, move_id::dig,
          &bn::sprite_items::diglett, species_behavior::burrower },
        { "ZUBAT", pokemon_type::poison, pokemon_type::flying, 14, 1.8, move_id::leech_life, move_id::supersonic,
          &bn::sprite_items::zubat, species_behavior::flyer },
        { "ONIX", pokemon_type::rock, pokemon_type::ground, 70, 0.9, move_id::rock_throw, move_id::slam,
          &bn::sprite_items::onix },
    };
}

namespace species
{

const species_data& get(species_id id)
{
    return table[int(id)];
}

}
