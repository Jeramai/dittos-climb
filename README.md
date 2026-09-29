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

`make assets` regenerates the placeholder art from `tools/gen_assets.py`.

A test build can start in a room of one kind (1 combat, 2 Pokémon Center, 3 stairs), with every form registered:

```
make TARGET=test-kind-2 BUILD=build-test-2 USERFLAGS=-DDITTO_TEST_START_KIND=2
```

## Controls

| Button | Action |
|---|---|
| D-pad | Move |
| A (hold) | Move 1 |
| B | Move 2 |
| L | Dodge |
| R (hold) | Lock aim while moving |
| Start | Floor map / restart after a black-out |
