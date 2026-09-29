#include "hud.h"

#include "bn_sprite_items_hp_bar.h"

#include "common_fixed_8x8_sprite_font.h"

#include "player.h"

namespace
{
    constexpr int z_order = -32000;
    constexpr int bar_x = -40;
    constexpr int top_y = -72;
    constexpr int line_height = 10;
    constexpr int bar_fill = 28;
    constexpr int boss_y = -50;

    int bar_frame(int hp, int max_hp)
    {
        int fill = hp <= 0 ? 0 : bn::max((hp * bar_fill) / max_hp, 1);
        int color = hp * 2 > max_hp ? 0 : hp * 5 > max_hp ? 1 : 2;
        return color * (bar_fill + 1) + fill;
    }

    bn::sprite_ptr make_bar(int y)
    {
        bn::sprite_ptr bar = bn::sprite_items::hp_bar.create_sprite(bar_x, y, 0);
        bar.set_bg_priority(0);
        bar.set_z_order(z_order);
        return bar;
    }

    void append_move(bn::istring& output, const char* button, move_id move, int pp)
    {
        output.append(button);
        output.append(moves::get(move).short_name);

        if(pp >= 0)
        {
            output.append(" ");
            output.append(bn::to_string<4>(pp));
        }
    }
}

hud::hud() :
    _text(common::fixed_8x8_sprite_font),
    _hp_bar(make_bar(top_y))
{
    _text.set_bg_priority(0);
    _text.set_z_order(z_order);
}

void hud::update(const player& value)
{
    const form* current = value.active_form();
    const species_data& body = value.body();
    int hp = current ? current->hp : value.hp();
    int max_hp = current ? body.hp * player::form_hp_scale : body.hp;
    _hp_bar.set_tiles(bn::sprite_items::hp_bar.tiles_item(), bar_frame(hp, max_hp));

    bn::string<64> key;
    key.append(body.name);

    if(current)
    {
        key.append(bn::to_string<4>(current->pp_b));
    }

    if(key == _shown_key)
    {
        return;
    }

    _shown_key = key;
    _text_sprites.clear();

    _text.set_left_alignment();
    _text.generate(-118, top_y, body.name, _text_sprites);

    bn::string<24> line_a;
    bn::string<24> line_b;

    if(current)
    {
        append_move(line_a, "A ", body.move_a, -1);
        append_move(line_b, "B ", body.move_b, current->pp_b);
    }
    else
    {
        append_move(line_a, "A ", body.move_a, -1);
        append_move(line_b, "B ", body.move_b, -1);
    }

    _text.set_right_alignment();
    _text.generate(118, top_y, line_a, _text_sprites);
    _text.generate(118, top_y + line_height, line_b, _text_sprites);
}

void hud::show_boss(const char* name, int hp, int max_hp)
{
    if(! _boss_bar)
    {
        _boss_bar = bn::sprite_items::hp_bar.create_sprite(28, boss_y, 0);
        _boss_bar->set_bg_priority(0);
        _boss_bar->set_z_order(z_order);
        _text.set_right_alignment();
        _text.generate(8, boss_y, name, _boss_text);
    }

    _boss_bar->set_tiles(bn::sprite_items::hp_bar.tiles_item(), bar_frame(hp, max_hp));
}

void hud::hide_boss()
{
    _boss_bar.reset();
    _boss_text.clear();
}

void hud::set_visible(bool visible)
{
    _hp_bar.set_visible(visible);

    if(_boss_bar)
    {
        _boss_bar->set_visible(visible);
    }

    for(bn::sprite_ptr& sprite : _boss_text)
    {
        sprite.set_visible(visible);
    }

    for(bn::sprite_ptr& sprite : _text_sprites)
    {
        sprite.set_visible(visible);
    }
}
