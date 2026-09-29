#include "species.h"

#include "bn_sprite_items_ditto.h"
#include "bn_sprite_items_meowth.h"
#include "bn_sprite_items_porygon.h"
#include "bn_sprite_items_rattata.h"
#include "bn_sprite_items_snorlax.h"

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
    };
}

namespace species
{

const species_data& get(species_id id)
{
    return table[int(id)];
}

}
