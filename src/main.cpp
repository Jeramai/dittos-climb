#include "bn_core.h"
#include "bn_random.h"

#include "game.h"
#include "audio.h"
#include "intro.h"
#include "profile.h"
#include "save.h"

int main()
{
    bn::core::init();
    bn::random random;
    audio::set_levels(profile::get().music_level, profile::get().sound_level);

    bool resume = intro::title(random, save::exists());

    if(! resume)
    {
        intro::story();
    }

    while(true)
    {
        {
            bn::optional<save_data> saved = resume ? save::load() : bn::nullopt;
            save::erase();

            game current(random, saved ? &*saved : nullptr);
            current.run();
        }

        bn::core::update();
        resume = intro::title(random, save::exists());
    }
}
