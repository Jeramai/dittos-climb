#include "floor_map.h"

#include "bn_math.h"

#include "journal.h"
#include "room_layouts.h"

using namespace directions_of_floor;

void floor_map::generate(int floor_number, int overgrown_percent, bn::random& random)
{
    for(auto& row : _grid)
    {
        for(int& value : row)
        {
            value = -1;
        }
    }

    _rooms.clear();

    int center = grid_size / 2;
    _rooms.push_back(floor_room{ center, center, 0, room_kind::start, true, true, {} });
    _grid[center][center] = 0;

    int target = bn::min(7 + floor_number / 2, max_rooms);

    for(int attempt = 0; attempt < 500 && _rooms.size() < target; ++attempt)
    {
        const floor_room& from = _rooms[random.get_int(_rooms.size())];
        direction side = direction(random.get_int(4));
        int x = from.x + dx(side);
        int y = from.y + dy(side);

        if(x < 0 || y < 0 || x >= grid_size || y >= grid_size || _grid[y][x] >= 0 || _neighbor_count(x, y) > 1)
        {
            continue;
        }

        _grid[y][x] = _rooms.size();
        _rooms.push_back(floor_room{ x, y, random.get_int(room_layouts::combat_count), room_kind::combat,
                                     false, false, {} });
    }

    int distances[max_rooms];
    int parents[max_rooms];

    for(int index = 0; index < max_rooms; ++index)
    {
        distances[index] = -1;
        parents[index] = -1;
    }

    int queue[max_rooms];
    int head = 0;
    int tail = 0;
    queue[tail++] = 0;
    distances[0] = 0;

    while(head < tail)
    {
        int current = queue[head++];

        for(int side = 0; side < 4; ++side)
        {
            int next = neighbor(current, direction(side));

            if(next >= 0 && distances[next] < 0)
            {
                distances[next] = distances[current] + 1;
                parents[next] = current;
                queue[tail++] = next;
            }
        }
    }

    int stairs = 0;

    for(int index = 1; index < _rooms.size(); ++index)
    {
        if(distances[index] > distances[stairs])
        {
            stairs = index;
        }
    }

    _rooms[stairs].kind = room_kind::stairs;
    _rooms[stairs].cleared = true;

    bool on_path[max_rooms] = {};

    for(int index = stairs; index >= 0; index = parents[index])
    {
        on_path[index] = true;
    }

    bn::vector<int, max_rooms> side_rooms;
    bn::vector<int, max_rooms> path_rooms;

    for(int index = 1; index < _rooms.size(); ++index)
    {
        if(_rooms[index].kind == room_kind::combat)
        {
            if(on_path[index])
            {
                path_rooms.push_back(index);
            }
            else
            {
                side_rooms.push_back(index);
            }
        }
    }

    if(floor_number <= journal::page_count)
    {
        bn::ivector<int>& pool = side_rooms.empty() ? path_rooms : side_rooms;

        if(! pool.empty())
        {
            int pick = random.get_int(pool.size());
            _rooms[pool[pick]].reward = room_reward::journal;
            pool.erase(pool.begin() + pick);
        }
    }

    for(int index : side_rooms)
    {
        floor_room& value = _rooms[index];

        if(random.get_int(2))
        {
            value.reward = room_reward::rare;
        }
        else
        {
            value.reward = room_reward::item;
            value.item = item_id(random.get_int(items::count));
        }
    }

    for(int index = 1; index < _rooms.size(); ++index)
    {
        for(direction side : { direction::east, direction::south })
        {
            int other = neighbor(index, side);
            bool path_door = other >= 0 && on_path[index] && on_path[other];

            if(other > 0 && ! path_door && random.get_int(100) < overgrown_percent)
            {
                _rooms[index].overgrown[int(side)] = true;
                _rooms[other].overgrown[int(opposite(side))] = true;
            }
        }
    }
}

void floor_map::clear_overgrown(int index, direction side)
{
    _rooms[index].overgrown[int(side)] = false;
    int other = neighbor(index, side);

    if(other >= 0)
    {
        _rooms[other].overgrown[int(opposite(side))] = false;
    }
}

int floor_map::room_at(int x, int y) const
{
    if(x < 0 || y < 0 || x >= grid_size || y >= grid_size)
    {
        return -1;
    }

    return _grid[y][x];
}

int floor_map::neighbor(int index, direction side) const
{
    const floor_room& value = _rooms[index];
    return room_at(value.x + dx(side), value.y + dy(side));
}

bool floor_map::known(int index) const
{
    if(_rooms[index].visited)
    {
        return true;
    }

    for(int side = 0; side < 4; ++side)
    {
        int next = neighbor(index, direction(side));

        if(next >= 0 && _rooms[next].visited)
        {
            return true;
        }
    }

    return false;
}

int floor_map::_neighbor_count(int x, int y) const
{
    int count = 0;

    for(int side = 0; side < 4; ++side)
    {
        if(room_at(x + dx(direction(side)), y + dy(direction(side))) >= 0)
        {
            ++count;
        }
    }

    return count;
}
