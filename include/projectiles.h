#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "bn_camera_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_vector.h"

#include "combat.h"
#include "room.h"

struct projectile
{
    bn::sprite_ptr sprite;
    bn::fixed_point position;
    bn::fixed_point velocity;
    attack hit;
    int life;
    int half_size;
    bool stops_at_walls;
};

template<int MaxSize>
class projectile_pool
{

public:
    explicit projectile_pool(const bn::camera_ptr& camera) :
        _camera(camera)
    {
    }

    [[nodiscard]] bool full() const
    {
        return _projectiles.full();
    }

    void spawn(bn::sprite_ptr&& sprite, const bn::fixed_point& position, const bn::fixed_point& velocity,
               const attack& hit, int life, int half_size, bool stops_at_walls)
    {
        if(_projectiles.full())
        {
            return;
        }

        sprite.set_camera(_camera);
        sprite.set_position(position);
        sprite.set_z_order(-1000);
        _projectiles.push_back(projectile{ bn::move(sprite), position, velocity, hit, life, half_size,
                                           stops_at_walls });
    }

    template<typename OnWallHit>
    void update(const OnWallHit& on_wall_hit)
    {
        bn::erase_if(_projectiles, [&on_wall_hit](projectile& value)
        {
            value.position += value.velocity;

            if(--value.life <= 0)
            {
                return true;
            }

            if(value.stops_at_walls && room::blocks_projectiles(value.position.x(), value.position.y()))
            {
                on_wall_hit(value.position);
                return true;
            }

            value.sprite.set_position(value.position);
            return false;
        });
    }

    template<typename Pred>
    void remove_if(const Pred& pred)
    {
        bn::erase_if(_projectiles, pred);
    }

    void clear()
    {
        _projectiles.clear();
    }

private:
    bn::camera_ptr _camera;
    bn::vector<projectile, MaxSize> _projectiles;
};

using player_projectiles = projectile_pool<20>;
using enemy_projectiles = projectile_pool<40>;

#endif
