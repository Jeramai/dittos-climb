#include "bn_core.h"
#include "bn_random.h"

#include "game.h"
#include "intro.h"

int main()
{
    bn::core::init();
    bn::random random;

    intro::title(random);
    intro::story();

    while(true)
    {
        game current(random);
        current.run();
        bn::core::update();
    }
}
