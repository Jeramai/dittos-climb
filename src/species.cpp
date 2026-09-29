#include "species.h"

#include "bn_sprite_items_ditto.h"
#include "bn_sprite_items_meowth.h"
#include "bn_sprite_items_rattata.h"

namespace
{
    constexpr species_data table[] = {
        { "DITTO", pokemon_type::normal, pokemon_type::none, 40, 1.25, move_id::struggle, move_id::transform,
          &bn::sprite_items::ditto },
        { "RATTATA", pokemon_type::normal, pokemon_type::none, 14, 1.5, move_id::tackle, move_id::hyper_fang,
          &bn::sprite_items::rattata },
        { "MEOWTH", pokemon_type::normal, pokemon_type::none, 16, 1.25, move_id::scratch, move_id::pay_day,
          &bn::sprite_items::meowth },
    };
}

namespace species
{

const species_data& get(species_id id)
{
    return table[int(id)];
}

}
