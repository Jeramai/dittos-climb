#include "boss.h"

#include "bn_math.h"

#include "room.h"

boss::boss(species_id id, int max_hp, const bn::fixed_point& position) :
    _species(id),
    _max_hp(max_hp),
    _hp(max_hp),
    _position(position)
{
}

void boss::announce_defeat(message_box& messages) const
{
    message_box::text message(name());
    message.append(" fainted!");
    messages.show(message);
}

bool boss::contains(const bn::fixed_point& point, int half_size) const
{
    bn::fixed_point delta = point - _position;
    return bn::abs(delta.x()) < 12 + half_size && bn::abs(delta.y()) < 13 + half_size;
}

hit_result boss::take_hit(const attack& hit, const bn::fixed_point&, int)
{
    if(! vulnerable())
    {
        return hit_result{ 0, types::neutral };
    }

    const species_data& data = species::get(_species);
    hit_result result = combat::resolve(hit, data.type_1, data.type_2);
    _hp -= result.damage;
    _flash_frames = 4;
    return result;
}

bool boss::hit_by_area(int serial)
{
    if(_last_area_serial == serial)
    {
        return false;
    }

    _last_area_serial = serial;
    return true;
}

attack boss::wild_attack(move_id move) const
{
    const species_data& data = species::get(_species);
    attack result = combat::make_attack(move, data.type_1, data.type_2);
    result.power = result.power * 2 / 3;
    return result;
}

bool boss::walk(const bn::fixed_point& step, bool can_swim)
{
    bool moved = false;
    bn::fixed_point next(_position.x() + step.x(), _position.y());

    if(! room::area_is_blocked(next.x() - 10, next.y() + 6, next.x() + 10, next.y() + 14, can_swim, can_swim))
    {
        _position = next;
        moved = true;
    }

    next = bn::fixed_point(_position.x(), _position.y() + step.y());

    if(! room::area_is_blocked(next.x() - 10, next.y() + 6, next.x() + 10, next.y() + 14, can_swim, can_swim))
    {
        _position = next;
        moved = true;
    }

    return moved;
}
