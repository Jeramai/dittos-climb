# Ditto's Climb

A top-down action roguelite for the Game Boy Advance, built with [Butano](https://github.com/GValiente/butano).
You play Ditto and climb a dungeon by transforming into the Pokémon you defeat. See [docs/design.md](docs/design.md).

## Build

Requires devkitPro with `gba-dev` (`sudo dkp-pacman -S gba-dev`) and Python 3 with Pillow.

```
git submodule update --init
make -j8
make run
```

`make assets` regenerates the placeholder art and `include/room_data.h` from `tools/gen_assets.py`.

## Controls

| Button | Action |
|---|---|
| D-pad | Move |
| A (hold) | Move 1 |
| B | Move 2 |
| L | Dodge |
| R (hold) | Lock aim while moving |
| Start | Pause / restart after a black-out |
