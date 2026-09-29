#ifndef JOURNAL_H
#define JOURNAL_H

namespace journal
{
    constexpr int page_count = 13;
    constexpr int lines_per_page = 3;

    [[nodiscard]] const char* line(int page, int line);
}

#endif
