#ifndef AUDIO_H
#define AUDIO_H

#include "bn_music_item.h"
#include "bn_sound_item.h"

namespace audio
{
    void init();

    void play_music(const bn::music_item& item);

    void play_floor_music(int floor_number);

    void stop_music();

    void play(const bn::sound_item& item);

    void play_quiet(const bn::sound_item& item);

    void set_levels(int music_level, int sound_level);
}

#endif
