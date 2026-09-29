#include "bn_core.h"
#include "bn_random.h"

#include "game.h"
#include "intro.h"
#include "save.h"

int main()
{
    bn::core::init();
    bn::random random;

    bool resume = intro::title(random, save::exists());

    if(! resume)
    {
        intro::story();
    }

    while(true)
    {
        bool back_to_title;

        {
            bn::optional<save_data> saved = resume ? save::load() : bn::nullopt;
            save::erase();

            game current(random, saved ? &*saved : nullptr);
            current.run();
            back_to_title = current.won() || current.quit();
        }

        bn::core::update();
        resume = back_to_title && intro::title(random, save::exists());
    }
}
