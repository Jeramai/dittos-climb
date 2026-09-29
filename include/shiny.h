#ifndef SHINY_H
#define SHINY_H

#include "bn_random.h"
#include "bn_sprite_palette_item.h"

#include "species.h"

namespace shiny
{
    constexpr int odds = 8192;

    [[nodiscard]] bool roll(bn::random& random);

    [[nodiscard]] const bn::sprite_palette_item* palette(species_id id);
}

#endif
