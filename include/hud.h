#ifndef HUD_H
#define HUD_H

#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

class player;

class hud
{

public:
    hud();

    void update(const player& value);

    void set_visible(bool visible);

    void show_boss(const char* name, int hp, int max_hp);

    void hide_boss();

private:
    bn::sprite_text_generator _text;
    bn::sprite_ptr _hp_bar;
    bn::vector<bn::sprite_ptr, 24> _text_sprites;
    bn::string<64> _shown_key;
    bn::optional<bn::sprite_ptr> _boss_bar;
    bn::vector<bn::sprite_ptr, 4> _boss_text;
};

#endif
