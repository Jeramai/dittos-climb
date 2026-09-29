#ifndef INTRO_H
#define INTRO_H

#include "bn_random.h"

namespace intro
{
    [[nodiscard]] bool title(bn::random& random, bool can_continue);

    void story();
}

#endif
