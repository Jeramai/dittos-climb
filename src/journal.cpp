#include "journal.h"

namespace
{
    constexpr const char* pages[journal::page_count][journal::lines_per_page] = {
        { "A new POKEMON was found", "deep in a jungle.", "We named it MEW." },
        { "MEW can learn every move.", "Its cells hold the code", "of all living POKEMON." },
        { "The team wants to copy MEW.", "The first samples all", "melted into goo." },
        { "Sample 132 did not melt.", "It is a small pink blob.", "It copies what it sees." },
        { "Sample 132 copied my face", "today. Everyone laughed.", "I did not." },
        { "The director calls 132 a", "failure. He wants a clone", "that can fight." },
        { "The new clone is ready.", "It has MEW's power, but", "none of its kindness." },
        { "We named the new clone", "MEWTWO. It will not stop", "staring at sample 132." },
        { "The director ordered 132", "destroyed. I flushed it", "down the pipe instead." },
        { "MEWTWO broke free last", "night. The lab is on fire.", "It fled underground." },
        { "MEWTWO hears MEW calling", "from the deep dungeon.", "It wants to be the only one." },
        { "If 132 still lives, it is", "the only one that can", "become anything. Even MEWTWO." },
        { "To 132, if you read this:", "you were never a failure.", "You are MEW's gift." },
    };
}

namespace journal
{

const char* line(int page, int line)
{
    return pages[page][line];
}

}
