#include "profile.h"

#include "bn_sram.h"

namespace
{
    constexpr unsigned profile_magic = 0x44435032;
    constexpr int sram_offset = 4096;

    profile::data current;
    bool loaded = false;

    profile::data& loaded_data()
    {
        if(! loaded)
        {
            bn::sram::read_offset(current, sram_offset);

            if(current.magic != profile_magic)
            {
                current = profile::data();
                current.magic = profile_magic;
            }

            #ifdef DITTO_TEST_DEX
                for(int word = 0; word < profile::form_words; ++word)
                {
                    current.seen[word] = 0xb5ad6f5b;
                    current.forms[word] = 0x21084211;
                    current.shiny_forms[word] = 0x01000402;
                }

                current.runs = 12;
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

}
