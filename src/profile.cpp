#include "profile.h"

#include "bn_math.h"
#include "bn_sram.h"

namespace
{
    constexpr unsigned profile_magic = 0x44435034;
    constexpr unsigned coins_profile_magic = 0x44435033;
    constexpr int sram_offset = 4096;

    struct coins_profile
    {
        unsigned magic;
        unsigned seen[profile::form_words];
        unsigned forms[profile::form_words];
        unsigned shiny_forms[profile::form_words];
        int runs;
        int wins;
        int best_floor;
        int coins;
    };

    profile::data upgraded(const coins_profile& old)
    {
        profile::data result = {};

        for(int word = 0; word < profile::form_words; ++word)
        {
            result.seen[word] = old.seen[word];
            result.forms[word] = old.forms[word];
            result.shiny_forms[word] = old.shiny_forms[word];
        }

        result.runs = old.runs;
        result.wins = old.wins;
        result.best_floor = old.best_floor;
        result.coins = old.coins;
        return result;
    }

    profile::data current;
    bool loaded = false;

    profile::data& loaded_data()
    {
        if(! loaded)
        {
            bn::sram::read_offset(current, sram_offset);

            if(current.magic != profile_magic)
            {
                coins_profile old;
                bn::sram::read_offset(old, sram_offset);
                current = old.magic == coins_profile_magic ? upgraded(old) : profile::data();
                current.magic = profile_magic;
                current.music_level = profile::max_level;
                current.sound_level = profile::max_level;
            }

            #ifdef DITTO_TEST_MART
                current.coins = 500;
            #endif

            #ifdef DITTO_TEST_DEX
                for(int word = 0; word < profile::form_words; ++word)
                {
                    current.seen[word] = 0xb5ad6f5b;
                    current.forms[word] = 0x21084211;
                    current.shiny_forms[word] = 0x01000402;
                }

                current.runs = 12;
                current.coins = 500;
                current.wins = 2;
                current.best_floor = 13;
            #endif

            loaded = true;
        }

        return current;
    }

    void write()
    {
        bn::sram::write_offset(loaded_data(), sram_offset);
    }

    bool has_bit(const unsigned* words, species_id id)
    {
        return words[int(id) / 32] & (1u << (int(id) % 32));
    }
}

namespace profile
{

const data& get()
{
    return loaded_data();
}

bool has_seen(species_id id)
{
    return has_bit(loaded_data().seen, id) || has_form(id);
}

bool has_form(species_id id)
{
    return has_bit(loaded_data().forms, id);
}

bool has_shiny_form(species_id id)
{
    return has_bit(loaded_data().shiny_forms, id);
}

int seen_count()
{
    int result = 0;

    for(int index = 1; index <= int(species_id::mew); ++index)
    {
        result += has_seen(species_id(index));
    }

    return result;
}

int form_count()
{
    int result = 0;

    for(int index = 1; index <= int(species_id::mew); ++index)
    {
        result += has_form(species_id(index));
    }

    return result;
}

void register_seen(species_id id, bool shiny)
{
    data& value = loaded_data();
    unsigned bit = 1u << (int(id) % 32);
    unsigned& seen = value.seen[int(id) / 32];
    unsigned& shiny_form = value.shiny_forms[int(id) / 32];
    bool changed = ! (seen & bit) || (shiny && ! (shiny_form & bit));
    seen |= bit;

    if(shiny)
    {
        shiny_form |= bit;
    }

    if(changed)
    {
        write();
    }
}

void register_form(species_id id, bool shiny)
{
    data& value = loaded_data();
    unsigned bit = 1u << (int(id) % 32);
    unsigned& form = value.forms[int(id) / 32];
    unsigned& shiny_form = value.shiny_forms[int(id) / 32];
    bool changed = ! (form & bit) || (shiny && ! (shiny_form & bit));
    form |= bit;

    if(shiny)
    {
        shiny_form |= bit;
    }

    if(changed)
    {
        write();
    }
}

void record_run_start()
{
    ++loaded_data().runs;
    write();
}

void record_floor(int floor_number)
{
    if(floor_number > loaded_data().best_floor)
    {
        loaded_data().best_floor = floor_number;
        write();
    }
}

void record_win()
{
    ++loaded_data().wins;
    write();
}

void add_coins(int amount)
{
    loaded_data().coins = bn::min(loaded_data().coins + amount, 99999);
    write();
}

void set_levels(int music_level, int sound_level)
{
    loaded_data().music_level = music_level;
    loaded_data().sound_level = sound_level;
    write();
}

bool spend_coins(int amount)
{
    if(loaded_data().coins < amount)
    {
        return false;
    }

    loaded_data().coins -= amount;
    write();
    return true;
}

}
