#ifndef DIRECTIONS_H
#define DIRECTIONS_H

#include "bn_fixed_point.h"
#include "bn_math.h"

namespace directions
{
    constexpr bn::fixed diagonal = 0.7071;

    // Index 0 points right and each step turns 45 degrees counterclockwise on screen.
    constexpr bn::fixed_point vectors[8] = {
        bn::fixed_point(1, 0),
        bn::fixed_point(diagonal, -diagonal),
        bn::fixed_point(0, -1),
        bn::fixed_point(-diagonal, -diagonal),
        bn::fixed_point(-1, 0),
        bn::fixed_point(-diagonal, diagonal),
        bn::fixed_point(0, 1),
        bn::fixed_point(diagonal, diagonal),
    };

    [[nodiscard]] constexpr int from_input(int x, int y)
    {
        constexpr int table[3][3] = {
            { 3, 2, 1 },
            { 4, -1, 0 },
            { 5, 6, 7 },
        };
        return table[y + 1][x + 1];
    }

    [[nodiscard]] inline bn::fixed_point toward(const bn::fixed_point& from, const bn::fixed_point& to,
                                                 bn::fixed turn_offset = 0)
    {
        bn::fixed_point delta = to - from;
        bn::fixed_t<16> angle = bn::atan2(-delta.y().round_integer(), delta.x().round_integer());
        angle += bn::fixed_t<16>(turn_offset);

        if(angle < 0)
        {
            angle += 1;
        }
        else if(angle >= 1)
        {
            angle -= 1;
        }

        bn::pair<bn::fixed, bn::fixed> sin_cos = bn::sin_and_cos(angle);
        return bn::fixed_point(sin_cos.second, -sin_cos.first);
    }
}

#endif
