#include "save.h"

#include "bn_sram.h"

namespace
{
    constexpr unsigned save_magic = 0x44434c34;
}

namespace save
{

bool exists()
{
    unsigned magic = 0;
    bn::sram::read(magic);
    return magic == save_magic;
}

bn::optional<save_data> load()
{
    if(! exists())
    {
        return bn::nullopt;
    }

    save_data data;
    bn::sram::read(data);
    return data;
}

void write(const save_data& data)
{
    save_data copy = data;
    copy.magic = save_magic;
    bn::sram::write(copy);
}

void erase()
{
    unsigned magic = 0;
    bn::sram::write(magic);
}

}
