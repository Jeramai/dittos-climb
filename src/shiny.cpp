#include "shiny.h"

#include "bn_sprite_palette_items_rattata_shiny.h"
#include "bn_sprite_palette_items_meowth_shiny.h"
#include "bn_sprite_palette_items_porygon_shiny.h"
#include "bn_sprite_palette_items_snorlax_shiny.h"
#include "bn_sprite_palette_items_oddish_shiny.h"
#include "bn_sprite_palette_items_caterpie_shiny.h"
#include "bn_sprite_palette_items_paras_shiny.h"
#include "bn_sprite_palette_items_beedrill_shiny.h"
#include "bn_sprite_palette_items_venusaur_shiny.h"
#include "bn_sprite_palette_items_geodude_shiny.h"
#include "bn_sprite_palette_items_diglett_shiny.h"
#include "bn_sprite_palette_items_zubat_shiny.h"
#include "bn_sprite_palette_items_onix_shiny.h"
#include "bn_sprite_palette_items_magikarp_shiny.h"
#include "bn_sprite_palette_items_poliwag_shiny.h"
#include "bn_sprite_palette_items_staryu_shiny.h"
#include "bn_sprite_palette_items_horsea_shiny.h"
#include "bn_sprite_palette_items_gyarados_shiny.h"
#include "bn_sprite_palette_items_pikachu_shiny.h"
#include "bn_sprite_palette_items_voltorb_shiny.h"
#include "bn_sprite_palette_items_magnemite_shiny.h"
#include "bn_sprite_palette_items_zapdos_shiny.h"
#include "bn_sprite_palette_items_machop_shiny.h"
#include "bn_sprite_palette_items_charmander_shiny.h"
#include "bn_sprite_palette_items_bulbasaur_shiny.h"
#include "bn_sprite_palette_items_sandshrew_shiny.h"
#include "bn_sprite_palette_items_vulpix_shiny.h"
#include "bn_sprite_palette_items_ponyta_shiny.h"
#include "bn_sprite_palette_items_growlithe_shiny.h"
#include "bn_sprite_palette_items_magmar_shiny.h"
#include "bn_sprite_palette_items_squirtle_shiny.h"
#include "bn_sprite_palette_items_moltres_shiny.h"
#include "bn_sprite_palette_items_seel_shiny.h"
#include "bn_sprite_palette_items_jynx_shiny.h"
#include "bn_sprite_palette_items_shellder_shiny.h"
#include "bn_sprite_palette_items_omanyte_shiny.h"
#include "bn_sprite_palette_items_articuno_shiny.h"
#include "bn_sprite_palette_items_pidgey_shiny.h"
#include "bn_sprite_palette_items_spearow_shiny.h"
#include "bn_sprite_palette_items_aerodactyl_shiny.h"
#include "bn_sprite_palette_items_kabuto_shiny.h"
#include "bn_sprite_palette_items_pidgeot_shiny.h"
#include "bn_sprite_palette_items_koffing_shiny.h"
#include "bn_sprite_palette_items_ekans_shiny.h"
#include "bn_sprite_palette_items_grimer_shiny.h"
#include "bn_sprite_palette_items_slowpoke_shiny.h"
#include "bn_sprite_palette_items_arbok_shiny.h"
#include "bn_sprite_palette_items_weezing_shiny.h"
#include "bn_sprite_palette_items_mankey_shiny.h"
#include "bn_sprite_palette_items_machoke_shiny.h"
#include "bn_sprite_palette_items_farfetchd_shiny.h"
#include "bn_sprite_palette_items_hitmonlee_shiny.h"
#include "bn_sprite_palette_items_hitmonchan_shiny.h"
#include "bn_sprite_palette_items_gastly_shiny.h"
#include "bn_sprite_palette_items_haunter_shiny.h"
#include "bn_sprite_palette_items_cubone_shiny.h"
#include "bn_sprite_palette_items_exeggcute_shiny.h"
#include "bn_sprite_palette_items_gengar_shiny.h"
#include "bn_sprite_palette_items_dratini_shiny.h"
#include "bn_sprite_palette_items_dragonair_shiny.h"
#include "bn_sprite_palette_items_seadra_shiny.h"
#include "bn_sprite_palette_items_lapras_shiny.h"
#include "bn_sprite_palette_items_dragonite_shiny.h"
#include "bn_sprite_palette_items_abra_shiny.h"
#include "bn_sprite_palette_items_kadabra_shiny.h"
#include "bn_sprite_palette_items_drowzee_shiny.h"
#include "bn_sprite_palette_items_venomoth_shiny.h"
#include "bn_sprite_palette_items_mewtwo_shiny.h"
#include "bn_sprite_palette_items_mew_shiny.h"

namespace
{
    constexpr const bn::sprite_palette_item* palettes[] = {
        nullptr,
        &bn::sprite_palette_items::rattata_shiny,
        &bn::sprite_palette_items::meowth_shiny,
        &bn::sprite_palette_items::porygon_shiny,
        &bn::sprite_palette_items::snorlax_shiny,
        &bn::sprite_palette_items::oddish_shiny,
        &bn::sprite_palette_items::caterpie_shiny,
        &bn::sprite_palette_items::paras_shiny,
        &bn::sprite_palette_items::beedrill_shiny,
        &bn::sprite_palette_items::venusaur_shiny,
        &bn::sprite_palette_items::geodude_shiny,
        &bn::sprite_palette_items::diglett_shiny,
        &bn::sprite_palette_items::zubat_shiny,
        &bn::sprite_palette_items::onix_shiny,
        &bn::sprite_palette_items::magikarp_shiny,
        &bn::sprite_palette_items::poliwag_shiny,
        &bn::sprite_palette_items::staryu_shiny,
        &bn::sprite_palette_items::horsea_shiny,
        &bn::sprite_palette_items::gyarados_shiny,
        &bn::sprite_palette_items::pikachu_shiny,
        &bn::sprite_palette_items::voltorb_shiny,
        &bn::sprite_palette_items::magnemite_shiny,
        &bn::sprite_palette_items::zapdos_shiny,
        &bn::sprite_palette_items::machop_shiny,
        &bn::sprite_palette_items::charmander_shiny,
        &bn::sprite_palette_items::bulbasaur_shiny,
        &bn::sprite_palette_items::sandshrew_shiny,
        &bn::sprite_palette_items::vulpix_shiny,
        &bn::sprite_palette_items::ponyta_shiny,
        &bn::sprite_palette_items::growlithe_shiny,
        &bn::sprite_palette_items::magmar_shiny,
        &bn::sprite_palette_items::squirtle_shiny,
        &bn::sprite_palette_items::moltres_shiny,
        &bn::sprite_palette_items::seel_shiny,
        &bn::sprite_palette_items::jynx_shiny,
        &bn::sprite_palette_items::shellder_shiny,
        &bn::sprite_palette_items::omanyte_shiny,
        &bn::sprite_palette_items::articuno_shiny,
        &bn::sprite_palette_items::pidgey_shiny,
        &bn::sprite_palette_items::spearow_shiny,
        &bn::sprite_palette_items::aerodactyl_shiny,
        &bn::sprite_palette_items::kabuto_shiny,
        &bn::sprite_palette_items::pidgeot_shiny,
        &bn::sprite_palette_items::koffing_shiny,
        &bn::sprite_palette_items::ekans_shiny,
        &bn::sprite_palette_items::grimer_shiny,
        &bn::sprite_palette_items::slowpoke_shiny,
        &bn::sprite_palette_items::arbok_shiny,
        &bn::sprite_palette_items::weezing_shiny,
        &bn::sprite_palette_items::mankey_shiny,
        &bn::sprite_palette_items::machoke_shiny,
        &bn::sprite_palette_items::farfetchd_shiny,
        &bn::sprite_palette_items::hitmonlee_shiny,
        &bn::sprite_palette_items::hitmonchan_shiny,
        &bn::sprite_palette_items::gastly_shiny,
        &bn::sprite_palette_items::haunter_shiny,
        &bn::sprite_palette_items::cubone_shiny,
        &bn::sprite_palette_items::exeggcute_shiny,
        &bn::sprite_palette_items::gengar_shiny,
        &bn::sprite_palette_items::dratini_shiny,
        &bn::sprite_palette_items::dragonair_shiny,
        &bn::sprite_palette_items::seadra_shiny,
        &bn::sprite_palette_items::lapras_shiny,
        &bn::sprite_palette_items::dragonite_shiny,
        &bn::sprite_palette_items::abra_shiny,
        &bn::sprite_palette_items::kadabra_shiny,
        &bn::sprite_palette_items::drowzee_shiny,
        &bn::sprite_palette_items::venomoth_shiny,
        &bn::sprite_palette_items::mewtwo_shiny,
        &bn::sprite_palette_items::mew_shiny,
    };
}

namespace shiny
{

bool roll(bn::random& random)
{
    #ifdef DITTO_TEST_SHINY
        static_cast<void>(random);
        return true;
    #else
        return random.get_int(odds) == 0;
    #endif
}

const bn::sprite_palette_item* palette(species_id id)
{
    return palettes[int(id)];
}

}
