#ifndef FLOOR_MAP_H
#define FLOOR_MAP_H

#include "bn_random.h"
#include "bn_vector.h"

#include "items.h"

enum class room_kind
{
    start,
    combat,
    stairs,
};

enum class room_reward
{
    none,
    journal,
    rare,
    item,
};

enum class direction
{
    north,
    east,
    south,
    west,
};

struct floor_room
{
    int x;
    int y;
    int layout;
    room_kind kind;
    bool visited;
    bool cleared;
    bool overgrown[4];
    room_reward reward = room_reward::none;
    item_id item = item_id::ether;
    bool reward_taken = false;
};

class floor_map
{

public:
    static constexpr int grid_size = 5;
    static constexpr int max_rooms = 11;

    void generate(int floor_number, int overgrown_percent, bool items_allowed, bn::random& random);

    void clear_overgrown(int index, direction side);

    [[nodiscard]] int room_at(int x, int y) const;

    [[nodiscard]] int neighbor(int index, direction side) const;

    [[nodiscard]] bool known(int index) const;

    [[nodiscard]] floor_room& operator[](int index)
    {
        return _rooms[index];
    }

    [[nodiscard]] const floor_room& operator[](int index) const
    {
        return _rooms[index];
    }

    [[nodiscard]] int size() const
    {
        return _rooms.size();
    }

private:
    bn::vector<floor_room, max_rooms> _rooms;
    int _grid[grid_size][grid_size];

    [[nodiscard]] int _neighbor_count(int x, int y) const;
};

namespace directions_of_floor
{
    [[nodiscard]] constexpr direction opposite(direction side)
    {
        return direction((int(side) + 2) % 4);
    }

    [[nodiscard]] constexpr int dx(direction side)
    {
        return side == direction::east ? 1 : side == direction::west ? -1 : 0;
    }

    [[nodiscard]] constexpr int dy(direction side)
    {
        return side == direction::south ? 1 : side == direction::north ? -1 : 0;
    }
}

#endif
