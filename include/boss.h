#ifndef BOSS_H
#define BOSS_H

#include "bn_optional.h"
#include "bn_random.h"

#include "message_box.h"
#include "projectiles.h"
#include "species.h"

struct boss_area_hit
{
    bn::fixed_point center;
    int radius;
    attack hit;
};

class boss
{

public:
    virtual ~boss() = default;

    virtual void update(const bn::fixed_point& target, enemy_projectiles& projectiles, bn::random& random,
                        message_box& messages) = 0;

    [[nodiscard]] virtual bool asleep() const
    {
        return false;
    }

    virtual void wake(message_box&)
    {
    }

    [[nodiscard]] virtual bool vulnerable() const
    {
        return true;
    }

    [[nodiscard]] virtual bn::optional<boss_area_hit> area_hit() const
    {
        return bn::nullopt;
    }

    [[nodiscard]] virtual bool touches(const bn::fixed_point&) const
    {
        return false;
    }

    [[nodiscard]] virtual bool blocks(const bn::fixed_point&, int) const
    {
        return false;
    }

    [[nodiscard]] virtual attack contact_attack() const
    {
        return wild_attack(species::get(_species).move_b);
    }

    [[nodiscard]] species_id species() const
    {
        return _species;
    }

    [[nodiscard]] const char* name() const
    {
        return species::get(_species).name;
    }

    [[nodiscard]] int hp() const
    {
        return _hp;
    }

    [[nodiscard]] int max_hp() const
    {
        return _max_hp;
    }

    [[nodiscard]] bool dead() const
    {
        return _hp <= 0;
    }

    [[nodiscard]] const bn::fixed_point& position() const
    {
        return _position;
    }

    [[nodiscard]] bool contains(const bn::fixed_point& point, int half_size) const;

    hit_result take_hit(const attack& hit);

    [[nodiscard]] bool hit_by_area(int serial);

protected:
    species_id _species;
    int _max_hp;
    int _hp;
    bn::fixed_point _position;
    int _flash_frames = 0;
    int _last_area_serial = -1;

    boss(species_id id, int max_hp, const bn::fixed_point& position);

    [[nodiscard]] attack wild_attack(move_id move) const;

    [[nodiscard]] bool walk(const bn::fixed_point& step);
};

#endif
