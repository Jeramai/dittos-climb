#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

class message_box
{

public:
    using text = bn::string<48>;

    message_box();

    void show(const bn::string_view& message);

    void update();

    void clear();

    void set_visible(bool visible);

private:
    bn::regular_bg_ptr _bg;
    bn::sprite_text_generator _generator;
    bn::vector<text, 4> _queue;
    bn::vector<bn::sprite_ptr, 10> _sprites;
    text _current;
    int _frames = 0;

    void _display_next();
};

#endif
