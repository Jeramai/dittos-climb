#include "message_box.h"

#include "bn_regular_bg_items_text_box.h"

#include "common_variable_8x16_sprite_font.h"

namespace
{
    constexpr int display_frames = 100;
    constexpr int hurried_display_frames = 55;
    constexpr int text_y = 68;
}

message_box::message_box() :
    _bg(bn::regular_bg_items::text_box.create_bg(0, 0)),
    _generator(common::variable_8x16_sprite_font)
{
    _bg.set_priority(0);
    _bg.set_visible(false);
    _generator.set_center_alignment();
    _generator.set_bg_priority(0);
    _generator.set_z_order(-32000);
}

void message_box::show(const bn::string_view& message)
{
    if(message.empty() || (_frames && _current == message))
    {
        return;
    }

    for(const text& queued : _queue)
    {
        if(queued == message)
        {
            return;
        }
    }

    if(_queue.full())
    {
        _queue.erase(_queue.begin());
    }

    _queue.push_back(text(message));

    if(! _frames)
    {
        _display_next();
    }
}

void message_box::update()
{
    if(! _frames)
    {
        return;
    }

    int limit = _queue.empty() ? display_frames : hurried_display_frames;

    if(++_frames > limit)
    {
        _display_next();
    }
}

void message_box::clear()
{
    _queue.clear();
    _sprites.clear();
    _current.clear();
    _frames = 0;
    _bg.set_visible(false);
}

void message_box::_display_next()
{
    _sprites.clear();

    if(_queue.empty())
    {
        _current.clear();
        _frames = 0;
        _bg.set_visible(false);
        return;
    }

    _current = _queue.front();
    _queue.erase(_queue.begin());
    _generator.generate(0, text_y, _current, _sprites);
    _bg.set_visible(true);
    _frames = 1;
}
