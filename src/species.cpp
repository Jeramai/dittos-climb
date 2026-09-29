#include "species.h"

#include "bn_sprite_items_abra.h"
#include "bn_sprite_items_aerodactyl.h"
#include "bn_sprite_items_arbok.h"
#include "bn_sprite_items_articuno.h"
#include "bn_sprite_items_beedrill.h"
#include "bn_sprite_items_bulbasaur.h"
#include "bn_sprite_items_charmander.h"
#include "bn_sprite_items_cubone.h"
#include "bn_sprite_items_caterpie.h"
#include "bn_sprite_items_diglett.h"
#include "bn_sprite_items_ditto.h"
#include "bn_sprite_items_dragonair.h"
#include "bn_sprite_items_dragonite.h"
#include "bn_sprite_items_dratini.h"
#include "bn_sprite_items_drowzee.h"
#include "bn_sprite_items_ekans.h"
#include "bn_sprite_items_exeggcute.h"
#include "bn_sprite_items_farfetchd.h"
#include "bn_sprite_items_gastly.h"
#include "bn_sprite_items_gengar.h"
#include "bn_sprite_items_geodude.h"
#include "bn_sprite_items_grimer.h"
#include "bn_sprite_items_haunter.h"
#include "bn_sprite_items_hitmonchan.h"
#include "bn_sprite_items_hitmonlee.h"
#include "bn_sprite_items_growlithe.h"
#include "bn_sprite_items_gyarados.h"
#include "bn_sprite_items_horsea.h"
#include "bn_sprite_items_jynx.h"
#include "bn_sprite_items_kabuto.h"
#include "bn_sprite_items_kadabra.h"
#include "bn_sprite_items_koffing.h"
#include "bn_sprite_items_lapras.h"
#include "bn_sprite_items_machoke.h"
#include "bn_sprite_items_machop.h"
#include "bn_sprite_items_magikarp.h"
#include "bn_sprite_items_magmar.h"
#include "bn_sprite_items_magnemite.h"
#include "bn_sprite_items_mankey.h"
#include "bn_sprite_items_meowth.h"
#include "bn_sprite_items_mew.h"
#include "bn_sprite_items_mewtwo.h"
#include "bn_sprite_items_moltres.h"
#include "bn_sprite_items_oddish.h"
#include "bn_sprite_items_omanyte.h"
#include "bn_sprite_items_onix.h"
#include "bn_sprite_items_paras.h"
#include "bn_sprite_items_pidgeot.h"
#include "bn_sprite_items_pidgey.h"
#include "bn_sprite_items_pikachu.h"
#include "bn_sprite_items_poliwag.h"
#include "bn_sprite_items_ponyta.h"
#include "bn_sprite_items_porygon.h"
#include "bn_sprite_items_rattata.h"
#include "bn_sprite_items_sandshrew.h"
#include "bn_sprite_items_seadra.h"
#include "bn_sprite_items_seel.h"
#include "bn_sprite_items_shellder.h"
#include "bn_sprite_items_slowpoke.h"
#include "bn_sprite_items_snorlax.h"
#include "bn_sprite_items_spearow.h"
#include "bn_sprite_items_squirtle.h"
#include "bn_sprite_items_staryu.h"
#include "bn_sprite_items_venomoth.h"
#include "bn_sprite_items_venusaur.h"
#include "bn_sprite_items_voltorb.h"
#include "bn_sprite_items_vulpix.h"
#include "bn_sprite_items_weezing.h"
#include "bn_sprite_items_zapdos.h"
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
        { "MAGIKARP", pokemon_type::water, pokemon_type::none, 10, 0.8, move_id::splash, move_id::tackle,
          &bn::sprite_items::magikarp, species_behavior::aquatic },
        { "POLIWAG", pokemon_type::water, pokemon_type::none, 16, 1, move_id::bubble, move_id::hypnosis,
          &bn::sprite_items::poliwag },
        { "STARYU", pokemon_type::water, pokemon_type::none, 16, 1.3, move_id::water_gun, move_id::swift,
          &bn::sprite_items::staryu },
        { "HORSEA", pokemon_type::water, pokemon_type::none, 14, 1.1, move_id::water_gun, move_id::bubblebeam,
          &bn::sprite_items::horsea, species_behavior::aquatic },
        { "GYARADOS", pokemon_type::water, pokemon_type::flying, 70, 1.1, move_id::bite, move_id::hydro_pump,
          &bn::sprite_items::gyarados },
        { "PIKACHU", pokemon_type::electric, pokemon_type::none, 16, 1.5, move_id::thundershock, move_id::quick_attack,
          &bn::sprite_items::pikachu },
        { "VOLTORB", pokemon_type::electric, pokemon_type::none, 16, 1.2, move_id::sonicboom, move_id::selfdestruct,
          &bn::sprite_items::voltorb },
        { "MAGNEMITE", pokemon_type::electric, pokemon_type::none, 14, 0.9, move_id::thundershock,
          move_id::thunder_wave, &bn::sprite_items::magnemite, species_behavior::flyer },
        { "ZAPDOS", pokemon_type::electric, pokemon_type::flying, 70, 1.4, move_id::thundershock, move_id::thunder,
          &bn::sprite_items::zapdos },
        { "MACHOP", pokemon_type::fighting, pokemon_type::none, 18, 1.1, move_id::karate_chop, move_id::low_kick,
          &bn::sprite_items::machop },
        { "CHARMANDER", pokemon_type::fire, pokemon_type::none, 16, 1.2, move_id::ember, move_id::flamethrower,
          &bn::sprite_items::charmander },
        { "BULBASAUR", pokemon_type::grass, pokemon_type::poison, 18, 1, move_id::vine_whip, move_id::leech_seed,
          &bn::sprite_items::bulbasaur },
        { "SANDSHREW", pokemon_type::ground, pokemon_type::none, 18, 1, move_id::scratch, move_id::dig,
          &bn::sprite_items::sandshrew },
        { "VULPIX", pokemon_type::fire, pokemon_type::none, 16, 1.3, move_id::ember, move_id::confuse_ray,
          &bn::sprite_items::vulpix },
        { "PONYTA", pokemon_type::fire, pokemon_type::none, 18, 1.8, move_id::tackle, move_id::fire_spin,
          &bn::sprite_items::ponyta },
        { "GROWLITHE", pokemon_type::fire, pokemon_type::none, 18, 1.4, move_id::bite, move_id::flamethrower,
          &bn::sprite_items::growlithe },
        { "MAGMAR", pokemon_type::fire, pokemon_type::none, 20, 1, move_id::fire_punch, move_id::smog,
          &bn::sprite_items::magmar },
        { "SQUIRTLE", pokemon_type::water, pokemon_type::none, 18, 1, move_id::water_gun, move_id::bubblebeam,
          &bn::sprite_items::squirtle },
        { "MOLTRES", pokemon_type::fire, pokemon_type::flying, 70, 1.4, move_id::ember, move_id::sky_attack,
          &bn::sprite_items::moltres },
        { "SEEL", pokemon_type::water, pokemon_type::none, 18, 1, move_id::headbutt, move_id::aurora_beam,
          &bn::sprite_items::seel },
        { "JYNX", pokemon_type::ice, pokemon_type::psychic, 18, 1, move_id::ice_punch, move_id::lovely_kiss,
          &bn::sprite_items::jynx },
        { "SHELLDER", pokemon_type::water, pokemon_type::none, 14, 0.9, move_id::clamp, move_id::ice_beam,
          &bn::sprite_items::shellder },
        { "OMANYTE", pokemon_type::rock, pokemon_type::water, 16, 0.8, move_id::water_gun, move_id::rock_slide,
          &bn::sprite_items::omanyte },
        { "ARTICUNO", pokemon_type::ice, pokemon_type::flying, 70, 1.4, move_id::ice_shard, move_id::blizzard,
          &bn::sprite_items::articuno },
        { "PIDGEY", pokemon_type::normal, pokemon_type::flying, 14, 1.5, move_id::gust, move_id::quick_attack,
          &bn::sprite_items::pidgey, species_behavior::flyer },
        { "SPEAROW", pokemon_type::normal, pokemon_type::flying, 14, 1.8, move_id::peck, move_id::fury_attack,
          &bn::sprite_items::spearow, species_behavior::flyer },
        { "AERODACTYL", pokemon_type::rock, pokemon_type::flying, 24, 1.7, move_id::bite, move_id::rock_slide,
          &bn::sprite_items::aerodactyl, species_behavior::flyer },
        { "KABUTO", pokemon_type::rock, pokemon_type::water, 16, 0.9, move_id::scratch, move_id::rock_slide,
          &bn::sprite_items::kabuto },
        { "PIDGEOT", pokemon_type::normal, pokemon_type::flying, 70, 1.5, move_id::gust, move_id::wing_attack,
          &bn::sprite_items::pidgeot },
        { "KOFFING", pokemon_type::poison, pokemon_type::none, 16, 0.7, move_id::tackle, move_id::selfdestruct,
          &bn::sprite_items::koffing, species_behavior::flyer },
        { "EKANS", pokemon_type::poison, pokemon_type::none, 16, 1.3, move_id::poison_sting, move_id::glare,
          &bn::sprite_items::ekans },
        { "GRIMER", pokemon_type::poison, pokemon_type::none, 20, 0.6, move_id::sludge, move_id::poison_gas,
          &bn::sprite_items::grimer },
        { "SLOWPOKE", pokemon_type::water, pokemon_type::psychic, 20, 0.7, move_id::water_gun, move_id::confusion,
          &bn::sprite_items::slowpoke },
        { "ARBOK", pokemon_type::poison, pokemon_type::none, 60, 1.2, move_id::poison_sting, move_id::wrap,
          &bn::sprite_items::arbok },
        { "WEEZING", pokemon_type::poison, pokemon_type::none, 60, 0.8, move_id::sludge, move_id::selfdestruct,
          &bn::sprite_items::weezing },
        { "MANKEY", pokemon_type::fighting, pokemon_type::none, 16, 1.6, move_id::karate_chop, move_id::thrash,
          &bn::sprite_items::mankey },
        { "MACHOKE", pokemon_type::fighting, pokemon_type::none, 22, 1.1, move_id::karate_chop, move_id::submission,
          &bn::sprite_items::machoke },
        { "FARFETCH'D", pokemon_type::normal, pokemon_type::flying, 16, 1.3, move_id::peck, move_id::wing_attack,
          &bn::sprite_items::farfetchd, species_behavior::flyer },
        { "HITMONLEE", pokemon_type::fighting, pokemon_type::none, 60, 1.3, move_id::rolling_kick,
          move_id::hi_jump_kick, &bn::sprite_items::hitmonlee },
        { "HITMONCHAN", pokemon_type::fighting, pokemon_type::none, 60, 1.2, move_id::fire_punch, move_id::mega_punch,
          &bn::sprite_items::hitmonchan },
        { "GASTLY", pokemon_type::ghost, pokemon_type::poison, 14, 1.2, move_id::lick, move_id::confuse_ray,
          &bn::sprite_items::gastly, species_behavior::flyer },
        { "HAUNTER", pokemon_type::ghost, pokemon_type::poison, 18, 1.4, move_id::lick, move_id::night_shade,
          &bn::sprite_items::haunter, species_behavior::flyer },
        { "CUBONE", pokemon_type::ground, pokemon_type::none, 18, 1, move_id::bone_club, move_id::bonemerang,
          &bn::sprite_items::cubone },
        { "EXEGGCUTE", pokemon_type::grass, pokemon_type::psychic, 18, 0.9, move_id::psywave, move_id::confusion,
          &bn::sprite_items::exeggcute },
        { "GENGAR", pokemon_type::ghost, pokemon_type::poison, 70, 1.3, move_id::lick, move_id::shadow_ball,
          &bn::sprite_items::gengar },
        { "DRATINI", pokemon_type::dragon, pokemon_type::none, 18, 1.2, move_id::twister, move_id::thunder_wave,
          &bn::sprite_items::dratini },
        { "DRAGONAIR", pokemon_type::dragon, pokemon_type::none, 24, 1.4, move_id::twister, move_id::dragon_rage,
          &bn::sprite_items::dragonair },
        { "SEADRA", pokemon_type::water, pokemon_type::none, 20, 1.2, move_id::water_gun, move_id::bubblebeam,
          &bn::sprite_items::seadra },
        { "LAPRAS", pokemon_type::water, pokemon_type::ice, 24, 1, move_id::ice_shard, move_id::ice_beam,
          &bn::sprite_items::lapras },
        { "DRAGONITE", pokemon_type::dragon, pokemon_type::flying, 70, 1.3, move_id::twister, move_id::hyper_beam,
          &bn::sprite_items::dragonite },
        { "ABRA", pokemon_type::psychic, pokemon_type::none, 12, 1, move_id::psywave, move_id::teleport,
          &bn::sprite_items::abra, species_behavior::teleporter },
        { "KADABRA", pokemon_type::psychic, pokemon_type::none, 20, 1.2, move_id::psywave, move_id::psybeam,
          &bn::sprite_items::kadabra, species_behavior::teleporter },
        { "DROWZEE", pokemon_type::psychic, pokemon_type::none, 20, 0.9, move_id::psywave, move_id::hypnosis,
          &bn::sprite_items::drowzee },
        { "VENOMOTH", pokemon_type::bug, pokemon_type::poison, 20, 1.3, move_id::leech_life, move_id::psybeam,
          &bn::sprite_items::venomoth, species_behavior::flyer },
        { "MEWTWO", pokemon_type::psychic, pokemon_type::none, 80, 1.4, move_id::psywave, move_id::psychic,
          &bn::sprite_items::mewtwo },
        { "MEW", pokemon_type::psychic, pokemon_type::none, 80, 1.4, move_id::psywave, move_id::psychic,
          &bn::sprite_items::mew },
    };
}

namespace species
{

const species_data& get(species_id id)
{
    return table[int(id)];
}

bool can_swim(const species_data& data)
{
    return data.type_1 == pokemon_type::water || data.type_2 == pokemon_type::water ||
           data.type_1 == pokemon_type::flying || data.type_2 == pokemon_type::flying;
}

}
