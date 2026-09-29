#ifndef SAVE_H
#define SAVE_H

#include "bn_optional.h"

#include "floor_map.h"
#include "player.h"

struct save_data
{
    unsigned magic;
    int floor_number;
    int room;
    int journal_pages;
    int journal_mask;
    int run_frames;
    int defeated;
    int shinies;
    unsigned run_forms[3];
    int flute_room;
    bool has_flute;
    bool has_silph_scope;
    bool boss_defeated;
    player_state player;
    int room_count;
    floor_room rooms[floor_map::max_rooms];
};

namespace save
{
    [[nodiscard]] bool exists();

    [[nodiscard]] bn::optional<save_data> load();

    void write(const save_data& data);

    void erase();
}

#endif
