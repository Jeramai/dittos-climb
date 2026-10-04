#include "player.h"

#include "bn_keypad.h"
#include "bn_sprite_affine_mat_ptr.h"

#include "bn_sprite_items_aim_arrow.h"
#include "bn_sprite_items_ditto.h"
#include "bn_sprite_items_wave.h"

#include "bn_sound_items.h"

#include "attacks.h"
#include "audio.h"
#include "shiny.h"
#include "directions.h"

namespace
{
    constexpr bn::fixed dodge_speed = 2.5;
    constexpr bn::fixed dodge_end_speed = 1;
    constexpr int dodge_frames = 26;
    constexpr int dodge_fast_frames = 16;
    constexpr int dodge_invulnerable_frames = 18;
    constexpr int dodge_cooldown_frames = 12;
    constexpr bn::fixed dodge_squash = 0.5;
    constexpr int hurt_invulnerable_frames = 60;
    constexpr int transform_frames = 30;
    constexpr int switch_flash_frames = 8;
    constexpr int dash_half_size = 6;
    constexpr int wave_half_size = 14;
    constexpr int explode_half_size = 30;
    constexpr int teleport_distance = 56;
    constexpr int teleport_invulnerable_frames = 20;
    constexpr int beam_charge_frames = 40;
    constexpr int paralysis_frames = 180;
    constexpr int poison_frames = 300;
    constexpr int poison_tick_frames = 45;
    constexpr int sleep_frames = 60;
    constexpr int confusion_frames = 150;
    constexpr int freeze_frames = 80;
    constexpr bn::fixed slide_speed = 2.2;
    constexpr bn::fixed current_speed = 0.6;
    constexpr bn::fixed wind_speed = 0.9;
    constexpr bn::fixed spin_speed = 2.8;
    constexpr int leftovers_frames = 120;
    constexpr int rare_candy_hp = 5;
    constexpr int aim_arrow_distance = 14;

    const species_data& ditto()
    {
        return species::get(species_id::ditto);
    }
}

player::player(const bn::camera_ptr& camera, const bn::fixed_point& position) :
    _camera(camera),
    _sprite(bn::sprite_items::ditto.create_sprite(position, species_frames::own_walk)),
    _aim_arrow(bn::sprite_items::aim_arrow.create_sprite(position)),
    _position(position),
    _hp(ditto().hp),
    _area_attack(combat::make_attack(move_id::struggle, pokemon_type::normal, pokemon_type::none)),
    _charge_attack(_area_attack)
{
    _sprite.set_camera(camera);
    _aim_arrow.set_camera(camera);
    _aim_arrow.set_z_order(-1000);
    _aim_arrow.set_visible(false);
}

bool player::update(player_projectiles& projectiles, message_box& messages, const species_id* outline_below,
                    bool outline_shiny)
{
    bool used_outline = false;

    if(_area_frames && ! --_area_frames && _self_destructing)
    {
        _self_destructing = false;
        _faint_form(messages);
    }

    _update_wave();

    if(_invulnerable_frames)
    {
        --_invulnerable_frames;
    }

    if(_switch_flash_frames)
    {
        --_switch_flash_frames;
    }

    if(_transform_frames)
    {
        if(! --_transform_frames)
        {
            _finish_transform(messages);
        }

        _update_sprite(false);
        return false;
    }

    ++_frame_counter;
    _update_water(messages);

    if(_held && _items_allowed && items::get(*_held).kind == item_kind::leftovers &&
       _frame_counter % leftovers_frames == 0)
    {
        heal(1);
    }

    if(! _update_status(messages))
    {
        _update_sprite(false);
        return false;
    }

    if(_charge_frames)
    {
        if(! --_charge_frames)
        {
            attacks::beam(projectiles, _charge_attack, _position, directions::vectors[_aim]);
        }

        _update_sprite(false);
        return false;
    }

    bool slowed = _status == status_effect::paralysis;

    int input_x = int(bn::keypad::right_held()) - int(bn::keypad::left_held());
    int input_y = int(bn::keypad::down_held()) - int(bn::keypad::up_held());

    if(_status == status_effect::confusion)
    {
        input_x = -input_x;
        input_y = -input_y;
    }
    int move_direction = directions::from_input(input_x, input_y);
    bool moving = move_direction >= 0;

    if(moving && ! bn::keypad::r_held() && ! _dash_frames)
    {
        _aim = move_direction;
    }

    if(_cooldown && (! slowed || _frame_counter % 2))
    {
        --_cooldown;
    }

    if(_dodge_cooldown)
    {
        --_dodge_cooldown;
    }

    if(_dash_frames)
    {
        --_dash_frames;
        _move(_dash_velocity);
        _digging = _digging && _dash_frames;
    }
    else if(_dodge_frames)
    {
        --_dodge_frames;
        bn::fixed speed = _dodge_frames > dodge_frames - dodge_fast_frames ? dodge_speed : dodge_end_speed;
        _move(directions::vectors[_dodge_direction] * speed);

        if(! _dodge_frames)
        {
            _dodge_cooldown = dodge_cooldown_frames;
        }
    }
    else
    {
        if(bn::keypad::l_pressed() && ! _dodge_cooldown)
        {
            _dodge_frames = dodge_frames;
            _dodge_direction = moving ? move_direction : _aim;
        }
        else
        {
            bn::fixed_point spin = room::spinner_at(_position);

            if(spin != bn::fixed_point())
            {
                _slide = bn::fixed_point();
                _move(spin * spin_speed);

                if(moving)
                {
                    _move(directions::vectors[move_direction] * body().speed);
                }
            }
            else if(_slide != bn::fixed_point())
            {
                if(! _on_slippery_ice() || ! _move(_slide))
                {
                    _slide = bn::fixed_point();
                }
            }
            else if(moving)
            {
                _move(directions::vectors[move_direction] * (slowed ? body().speed / 2 : body().speed));

                if(_on_slippery_ice())
                {
                    _slide = directions::vectors[move_direction] * slide_speed;
                }
            }

            if(outline_below && ! _form && bn::keypad::b_pressed())
            {
                start_transform(*outline_below, outline_shiny);
                used_outline = true;
            }
            else if(! _cooldown)
            {
                if(bn::keypad::a_held())
                {
                    _use_move(true, projectiles, messages);
                }
                else if(bn::keypad::b_pressed())
                {
                    _use_move(false, projectiles, messages);
                }
            }
        }
    }

    _update_sprite(moving);
    return used_outline;
}

bool player::vulnerable() const
{
    if(_invulnerable_frames || _transform_frames || _digging)
    {
        return false;
    }

    return ! _dodge_frames || _dodge_frames <= dodge_frames - dodge_invulnerable_frames;
}

const species_data& player::body() const
{
    const form* current = active_form();
    return current ? species::get(current->species) : ditto();
}

const form* player::active_form() const
{
    return _form ? &*_form : nullptr;
}

hit_result player::take_hit(const attack& hit, message_box& messages)
{
    const species_data& current = body();
    hit_result result = combat::resolve(hit, current.type_1, current.type_2);
    _invulnerable_frames = hurt_invulnerable_frames;

    if(! result.effectiveness)
    {
        messages.show("It doesn't affect DITTO...");
        return result;
    }

    if(! result.damage)
    {
        return result;
    }

    messages.show(combat::effectiveness_message(result.effectiveness));

    if(! _form)
    {
        _hp = bn::max(_hp - result.damage, 0);
        return result;
    }

    _form->hp -= result.damage;

    if(_form->hp <= 0)
    {
        _faint_form(messages);
    }

    return result;
}

void player::apply_status(status_effect effect, message_box& messages)
{
    if(effect == status_effect::none || _status != status_effect::none || _transform_frames)
    {
        return;
    }

    _status = effect;
    _poison_timer = 0;

    switch(effect)
    {

    case status_effect::paralysis:
        _status_frames = paralysis_frames;
        messages.show("DITTO is paralyzed!");
        break;

    case status_effect::poison:
        _status_frames = poison_frames;
        messages.show("DITTO was poisoned!");
        break;

    case status_effect::confusion:
        _status_frames = confusion_frames;
        messages.show("DITTO became confused!");
        break;

    case status_effect::freeze:
    {
        const species_data& current = body();

        if(current.type_1 == pokemon_type::fire || current.type_2 == pokemon_type::fire ||
           current.type_1 == pokemon_type::ice || current.type_2 == pokemon_type::ice)
        {
            _status = status_effect::none;
            return;
        }

        _status_frames = freeze_frames;
        _charge_frames = 0;
        _slide = bn::fixed_point();
        messages.show("DITTO was frozen solid!");
        break;
    }

    default:
        _status_frames = sleep_frames;
        _charge_frames = 0;
        messages.show("DITTO fell asleep!");
        break;
    }
}

player_state player::state() const
{
    bn::optional<form> saved_form = _form;

    if(_transform_frames)
    {
        const species_data& target = species::get(_transform_target);
        saved_form = form{ _transform_target, target.hp * form_hp_scale, moves::get(target.move_b).pp,
                           _transform_shiny };
    }

    return player_state{ _hp, _bonus_hp, saved_form.has_value(), saved_form.value_or(form{ species_id::ditto, 0, 0 }),
                         _held.has_value(), _held.value_or(item_id::ether), _shiny_ditto,
                         _bag.has_value(), _bag.value_or(item_id::potion) };
}

void player::restore(const player_state& state)
{
    _hp = state.hp;
    _bonus_hp = state.bonus_hp;
    _shiny_ditto = state.shiny_ditto;
    _form.reset();
    _held.reset();
    _bag.reset();

    if(state.has_bag)
    {
        _bag = state.bag;
    }

    if(state.has_form)
    {
        _form = state.form_value;
    }

    if(state.has_held)
    {
        _held = state.held;
    }

    _update_sprite(false);
}

int player::max_hp() const
{
    return ditto().hp + _bonus_hp;
}

bool player::give_item(item_id id, message_box& messages)
{
    const item_data& item = items::get(id);
    message_box::text message;

    if(items::held(item.kind))
    {
        message.append("DITTO is now holding ");
        message.append(item.name);
        message.append("!");
        _held = id;
        messages.show(message);
        return true;
    }

    if(! _bag)
    {
        message.append("Put the ");
        message.append(item.name);
        message.append(" in the BAG.");
        _bag = id;
        messages.show(message);
        return true;
    }

    return use_item(id, messages);
}

void player::use_bag(message_box& messages)
{
    if(_bag && use_item(*_bag, messages))
    {
        _bag.reset();
    }
}

bool player::use_item(item_id id, message_box& messages)
{
    const item_data& item = items::get(id);

    switch(item.kind)
    {

    case item_kind::ether:
        if(! _form)
        {
            messages.show("DITTO has no PP to restore.");
            return false;
        }

        if(_form->pp_b >= moves::get(species::get(_form->species).move_b).pp)
        {
            messages.show("The PP is already full!");
            return false;
        }

        _form->pp_b = moves::get(species::get(_form->species).move_b).pp;
        messages.show("DITTO used the ETHER!");
        messages.show("The PP of move B was restored!");
        return true;

    case item_kind::potion:
    {
        int maximum = _form ? species::get(_form->species).hp * form_hp_scale : max_hp();

        if((_form ? _form->hp : _hp) >= maximum)
        {
            messages.show("The HP is already full!");
            return false;
        }

        heal(maximum / 2);
        messages.show("DITTO used the POTION!");
        return true;
    }

    case item_kind::rare_candy:
        _bonus_hp += rare_candy_hp;
        _hp += rare_candy_hp;
        messages.show("DITTO ate the RARE CANDY!");
        messages.show("DITTO's max HP rose!");
        return true;

    default:
        return false;
    }
}

void player::set_forced_struggle(bool forced, message_box& messages)
{
    forced = forced && _form;

    if(forced && ! _forced_struggle)
    {
        message_box::text message(species::get(_form->species).name);
        message.append(" can't hurt the foe!");
        messages.show(message);
        messages.show("It will STRUGGLE!");
    }

    _forced_struggle = forced;
}

void player::heal(int amount)
{
    if(_form)
    {
        _form->hp = bn::min(_form->hp + amount, species::get(_form->species).hp * form_hp_scale);
    }
    else
    {
        _hp = bn::min(_hp + amount, max_hp());
    }
}

void player::recoil(message_box& messages)
{
    messages.show("DITTO is hit with recoil!");

    if(_form)
    {
        _form->hp = bn::max(_form->hp - 1, 1);
    }
    else
    {
        _hp = bn::max(_hp - 1, 0);
    }
}

bool player::_update_status(message_box& messages)
{
    if(_status == status_effect::none)
    {
        return true;
    }

    if(_status == status_effect::poison && ++_poison_timer % poison_tick_frames == 0)
    {
        _lose_hp(1);
    }

    bool asleep = _status == status_effect::sleep || _status == status_effect::freeze;

    if(--_status_frames <= 0)
    {
        if(_status == status_effect::freeze)
        {
            messages.show("DITTO thawed out!");
        }
        else if(asleep)
        {
            messages.show("DITTO woke up!");
        }
        else if(_status == status_effect::confusion)
        {
            messages.show("DITTO snapped out of confusion!");
        }

        _status = status_effect::none;
    }

    return ! asleep;
}

void player::_faint_form(message_box& messages)
{
    if(! _form)
    {
        return;
    }

    message_box::text message(species::get(_form->species).name);
    message.append(" fainted!");
    messages.show(message);
    messages.show("DITTO lost its shape!");
    _drop_form();
}

bool player::release_form(message_box& messages)
{
    if(! _form || _transform_frames || _dodge_frames || _dash_frames || _digging || _charge_frames ||
       _self_destructing)
    {
        return false;
    }

    messages.show("DITTO let go of its shape!");
    _drop_form();
    return true;
}

void player::_drop_form()
{
    audio::play(bn::sound_items::sfx_faint);
    _form.reset();
    _charge_frames = 0;
    _dash_frames = 0;
    _area_frames = 0;
    _digging = false;
    _self_destructing = false;
    _switch_flash_frames = switch_flash_frames;
}

void player::_lose_hp(int amount)
{
    if(_form)
    {
        _form->hp = bn::max(_form->hp - amount, 1);
    }
    else
    {
        _hp = bn::max(_hp - amount, 1);
    }
}

void player::start_transform(species_id target, bool shiny)
{
    audio::play(bn::sound_items::sfx_transform);
    _transform_target = target;
    _transform_shiny = shiny;
    _transform_frames = transform_frames;
    _dash_frames = 0;
    _digging = false;
    _area_frames = 0;
    _charge_frames = 0;
    _status = status_effect::none;
    _dodge_frames = 0;
}

void player::evolve(species_id target)
{
    if(_form)
    {
        _evolving_from = _form->species;
        start_transform(target, _form->shiny);
    }
}

void player::set_position(const bn::fixed_point& position)
{
    _position = position;
    _dash_frames = 0;
    _digging = false;
    _slide = bn::fixed_point();
    _dodge_frames = 0;
    _area_frames = 0;
    _self_destructing = false;
    _update_sprite(false);
}

bool player::over_pit() const
{
    return ! flying() && ! _transform_frames && room::feet_over_pit(_position);
}

bool player::flying() const
{
    const species_data& current = body();
    return current.type_1 == pokemon_type::flying || current.type_2 == pokemon_type::flying;
}

void player::push(const bn::fixed_point& delta)
{
    if(! flying())
    {
        _move(delta);
    }
}

void player::take_fall_damage(int amount)
{
    _lose_hp(amount);
    _invulnerable_frames = hurt_invulnerable_frames;
}

void player::set_visible(bool visible)
{
    _sprite.set_visible(visible);
    _aim_arrow.set_visible(false);
}

void player::_use_move(bool move_a, player_projectiles& projectiles, message_box& messages)
{
    const species_data& current = body();
    move_id move = _forced_struggle ? move_id::struggle : move_a ? current.move_a : current.move_b;

    if(_form && ! move_a && ! _forced_struggle)
    {
        int& pp = _form->pp_b;

        if(pp)
        {
            --pp;
        }
        else
        {
            message_box::text message(moves::get(move).name);
            message.append(" has no PP left!");
            messages.show(message);
            move = move_id::struggle;
        }
    }

    const move_data& data = moves::get(move);
    attack hit = combat::make_attack(move, current.type_1, current.type_2);
    audio::play_quiet(move == move_id::struggle ? bn::sound_items::sfx_struggle : bn::sound_items::sfx_shot);
    const bn::fixed_point& aim = directions::vectors[_aim];
    _cooldown = data.cooldown;

    if(_held && _items_allowed)
    {
        const item_data& item = items::get(*_held);

        if(item.kind == item_kind::type_boost && item.boosted_type == hit.type)
        {
            hit.power = hit.power * 6 / 5;
        }
        else if(item.kind == item_kind::quick_claw)
        {
            _cooldown = _cooldown * 4 / 5;
        }
    }

    switch(data.pattern)
    {

    case move_pattern::shot:
        attacks::shoot(projectiles, hit, _position + aim * 6, aim, 1);
        break;

    case move_pattern::melee:
        attacks::slash(projectiles, hit, _position, aim);
        break;

    case move_pattern::dash:
        _dash_frames = data.life;
        _dash_velocity = aim * data.speed;
        _start_area(hit, data.life, dash_half_size);
        break;

    case move_pattern::cloud:
        attacks::cloud(projectiles, hit, _position + aim * 8, aim, 1);
        break;

    case move_pattern::teleport:
    {
        bn::fixed_point goal = _position + aim * teleport_distance;
        bool can_swim = species::can_swim(current);

        if(room::feet_are_blocked(goal, can_swim))
        {
            bn::optional<bn::fixed_point> spot = room::nearest_standable(goal, can_swim);
            goal = spot ? *spot : _position;
        }

        _position = goal;
        _invulnerable_frames = teleport_invulnerable_frames;
        _switch_flash_frames = switch_flash_frames;
        messages.show("DITTO used TELEPORT!");
        break;
    }

    case move_pattern::explode:
        _start_area(hit, data.life, explode_half_size);
        _self_destructing = true;
        messages.show("DITTO used SELFDESTRUCT!");
        break;

    case move_pattern::dig:
        _dash_frames = data.life;
        _dash_velocity = aim * data.speed;
        _digging = true;
        _start_area(hit, data.life, dash_half_size);
        messages.show("DITTO dug underground!");
        break;

    case move_pattern::beam:
        _charge_frames = beam_charge_frames;
        _charge_attack = hit;
        messages.show(move == move_id::solar_beam ? "DITTO is taking in sunlight!" : "DITTO is charging up!");
        break;

    case move_pattern::wave:
        _start_area(hit, data.life, wave_half_size);
        _wave_sprite = bn::sprite_items::wave.create_sprite(_position, 0);
        _wave_sprite->set_camera(_camera);
        _wave_sprite->set_z_order(-999);
        break;

    default:
        if(move == move_id::splash)
        {
            messages.show("DITTO used SPLASH!");
            messages.show("But nothing happened!");
        }
        else
        {
            messages.show("DITTO used TRANSFORM!");
            messages.show("But it failed!");
        }
        break;
    }
}

void player::_finish_transform(message_box& messages)
{
    const species_data& target = species::get(_transform_target);
    _form = form{ _transform_target, target.hp * form_hp_scale,
                  moves::get(target.move_b).pp, _transform_shiny };
    _hp = max_hp();

    message_box::text message;

    if(_evolving_from)
    {
        message.append(species::get(*_evolving_from).name);
        message.append(" evolved into ");
        _evolving_from.reset();
    }
    else
    {
        message.append("DITTO transformed into ");
    }

    message.append(target.name);
    message.append("!");
    messages.show(message);
}

bool player::_move(const bn::fixed_point& delta)
{
    bool can_swim = species::can_swim(body());
    bool moved = true;
    constexpr bool over_pits = true;
    bn::fixed_point next(_position.x() + delta.x(), _position.y());

    if(! room::feet_are_blocked(next, can_swim, over_pits))
    {
        _position = next;
    }
    else if(delta.x() != 0)
    {
        moved = false;
    }

    next = bn::fixed_point(_position.x(), _position.y() + delta.y());

    if(! room::feet_are_blocked(next, can_swim, over_pits))
    {
        _position = next;
    }
    else if(delta.y() != 0)
    {
        moved = false;
    }

    return moved;
}

bool player::_on_slippery_ice() const
{
    const species_data& current = body();

    if(current.type_1 == pokemon_type::ice || current.type_2 == pokemon_type::ice)
    {
        return false;
    }

    return room::at(_position.x(), _position.y() + 4) == room::cells::ice;
}

void player::_update_water(message_box& messages)
{
    bool can_swim = species::can_swim(body());

    if(room::feet_are_blocked(_position, can_swim, true))
    {
        if(bn::optional<bn::fixed_point> shore = room::nearest_standable(_position, can_swim))
        {
            _position = *shore;
            messages.show("DITTO washed ashore!");
        }

        return;
    }

    bn::fixed_point flow = room::flow_at(_position);

    if(flow != bn::fixed_point())
    {
        _move(flow * current_speed);
    }

    bn::fixed_point wind = room::wind_at(_position);

    if(wind != bn::fixed_point() && ! flying())
    {
        _move(wind * wind_speed);
    }
}

void player::_start_area(const attack& hit, int frames, int half_size)
{
    _area_attack = hit;
    _area_frames = frames;
    _area_half_size = half_size;
    ++_area_serial;
}

void player::_update_wave()
{
    if(! _wave_sprite)
    {
        return;
    }

    if(! _area_frames)
    {
        _wave_sprite.reset();
        return;
    }

    _wave_sprite->set_tiles(bn::sprite_items::wave.tiles_item(), (_area_frames / 3) % 2);
    _wave_sprite->set_position(_position);
}

void player::_update_sprite(bool moving)
{
    _update_sprite_item(moving);
    _update_aim_arrow();

    if(_form && _form->shiny && ! _transform_frames)
    {
        _sprite.set_palette(*shiny::palette(_form->species));
    }
    else if(! _form && _shiny_ditto && ! _transform_frames)
    {
        _sprite.set_palette(shiny::ditto_palette());
    }
}

void player::_update_aim_arrow()
{
    constexpr int frames[8] = { 0, 2, 1, 2, 0, 2, 1, 2 };

    _aim_arrow.set_visible(bn::keypad::r_held() && ! _transform_frames);
    _aim_arrow.set_tiles(bn::sprite_items::aim_arrow.tiles_item(), frames[_aim]);
    _aim_arrow.set_horizontal_flip(_aim >= 3 && _aim <= 5);
    _aim_arrow.set_vertical_flip(_aim >= 5);
    _aim_arrow.set_position(_position + directions::vectors[_aim] * aim_arrow_distance);
}

void player::set_shiny_ditto(bool shiny)
{
    _shiny_ditto = shiny;
    _update_sprite(false);
}

void player::_update_sprite_item(bool moving)
{
    _sprite.set_position(_position);
    _sprite.set_z_order(-_position.y().round_integer());

    if(_charge_frames && (_charge_frames / 3) % 2)
    {
        _sprite.set_item(*body().sprite, species_frames::white);
        _sprite.set_visible(true);
        return;
    }

    if(_transform_frames)
    {
        bool show_target = (_transform_frames / 3) % 2 == 0 && _transform_frames < transform_frames * 2 / 3;
        const bn::sprite_item& item = show_target ? *species::get(_transform_target).sprite : bn::sprite_items::ditto;
        _sprite.set_item(item, species_frames::white);
        _sprite.set_visible(true);
        return;
    }

    bool squashed = _dodge_frames && _form;

    if(_digging)
    {
        const bn::sprite_item& item = *body().sprite;
        bool has_mound = item.tiles_item().graphics_count() > species_frames::mound;
        _sprite.set_item(item, has_mound ? species_frames::mound : species_frames::own_walk);
        _sprite.set_visible(true);
        return;
    }

    if(_dodge_frames && ! _form)
    {
        _sprite.set_item(bn::sprite_items::ditto, species_frames::ditto_flat);
    }
    else if(squashed)
    {
        _sprite.set_item(*body().sprite, species_frames::own_walk);
    }
    else if(_switch_flash_frames)
    {
        _sprite.set_item(*body().sprite, species_frames::white);
    }
    else
    {
        if(moving)
        {
            ++_walk_frames;
        }
        else
        {
            _walk_frames = 0;
        }

        bool flailing = _wave_sprite.has_value();
        int walk_frame = flailing ? 1 : (_walk_frames / 8) % 2;
        _sprite.set_item(*body().sprite, species_frames::own_walk + walk_frame);
    }

    if(squashed)
    {
        _sprite.set_vertical_scale(dodge_squash);
        _sprite.set_y(_position.y() + _sprite.shape_size().height() * (1 - dodge_squash) / 2);
    }
    else if(_sprite.affine_mat())
    {
        _sprite.remove_affine_mat();
    }

    _sprite.set_horizontal_flip(directions::vectors[_aim].x() < 0);

    bool blink = _invulnerable_frames && (_invulnerable_frames / 4) % 2;
    _sprite.set_visible(! blink);
}
