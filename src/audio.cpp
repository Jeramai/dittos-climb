#include "audio.h"

#include "bn_math.h"
#include "bn_music.h"
#include "bn_optional.h"

#include "bn_music_items.h"

namespace
{
    constexpr bn::fixed music_volume = 0.45;
    constexpr bn::fixed sound_volume = 0.7;
    constexpr bn::fixed quiet_volume = 0.35;

    constexpr const bn::music_item* floor_music[] = {
        &bn::music_items::lab, &bn::music_items::forest, &bn::music_items::cave, &bn::music_items::lake,
        &bn::music_items::plant, &bn::music_items::volcano, &bn::music_items::ice, &bn::music_items::chasm,
        &bn::music_items::hideout, &bn::music_items::dojo, &bn::music_items::tower, &bn::music_items::den,
        &bn::music_items::peak,
    };

    constexpr int floor_music_count = sizeof(floor_music) / sizeof(floor_music[0]);
}

namespace audio
{

void play_music(const bn::music_item& item)
{
    bn::optional<bn::music_item> current = bn::music::playing_item();

    if(current && *current == item)
    {
        return;
    }

    item.play(music_volume);
}

void play_floor_music(int floor_number)
{
    play_music(*floor_music[bn::min(floor_number, floor_music_count) - 1]);
}

void stop_music()
{
    if(bn::music::playing())
    {
        bn::music::stop();
    }
}

void play(const bn::sound_item& item)
{
    item.play(sound_volume);
}

void play_quiet(const bn::sound_item& item)
{
    item.play(quiet_volume);
}

}
