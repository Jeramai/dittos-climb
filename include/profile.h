#ifndef PROFILE_H
#define PROFILE_H

#include "species.h"

namespace profile
{
    constexpr int form_words = 3;

    struct data
    {
        unsigned magic;
        unsigned forms[form_words];
        unsigned shiny_forms[form_words];
        int runs;
        int wins;
        int best_floor;
    };

    [[nodiscard]] const data& get();

    [[nodiscard]] bool has_form(species_id id);

    [[nodiscard]] bool has_shiny_form(species_id id);

    [[nodiscard]] int form_count();

    void register_form(species_id id, bool shiny);

    void record_run_start();

    void record_floor(int floor_number);

    void record_win();
}

#endif
