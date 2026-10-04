#include "audio.h"

#include "bn_math.h"
#include "bn_music.h"
#include "bn_optional.h"
#include "bn_sound_handle.h"
#include "bn_vector.h"

#include "bn_music_items.h"

namespace
{
    constexpr bn::fixed full_music_volume = 0.45;
    constexpr bn::fixed full_sound_volume = 0.7;
    constexpr bn::fixed full_quiet_volume = 0.35;
    constexpr int max_level = 10;
    // Butano asserts once all 8 of its sound handles are in use; keep 2 free.
    constexpr int max_active_sounds = 6;

    int music_level = max_level;
    int sound_level = max_level;

    bn::fixed music_volume()
    {
        return full_music_volume * music_level / max_level;
    }

    constexpr const bn::music_item* floor_music[] = {
        &bn::music_items::lab, &bn::music_items::forest, &bn::music_items::cave, &bn::music_items::lake,
        &bn::music_items::plant, &bn::music_items::volcano, &bn::music_items::ice, &bn::music_items::chasm,
        &bn::music_items::hideout, &bn::music_items::dojo, &bn::music_items::tower, &bn::music_items::den,
        &bn::music_items::peak,
    };

    constexpr int floor_music_count = sizeof(floor_music) / sizeof(floor_music[0]);

    bn::vector<bn::sound_handle, max_active_sounds> active_sounds;

    bool sound_handle_free()
    {
        bn::erase_if(active_sounds, [](const bn::sound_handle& handle) { return ! handle.active(); });
        return ! active_sounds.full();
    }
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

    item.play(music_volume());
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
    if(sound_level && sound_handle_free())
    {
        active_sounds.push_back(item.play(full_sound_volume * sound_level / max_level));
    }
}

void play_quiet(const bn::sound_item& item)
{
    if(sound_level && sound_handle_free())
    {
        active_sounds.push_back(item.play(full_quiet_volume * sound_level / max_level));
    }
}

void set_levels(int music, int sound)
{
    music_level = bn::clamp(music, 0, max_level);
    sound_level = bn::clamp(sound, 0, max_level);

    if(bn::music::playing())
    {
        bn::music::set_volume(music_volume());
    }
}

}
