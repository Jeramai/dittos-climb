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

Test builds take these flags in `USERFLAGS` (use a separate `BUILD` folder, because `make` does not rebuild on a flag change):

| Flag | Effect |
|---|---|
| `-DDITTO_TEST_START_KIND=1` | Start in a combat room, which is also the Poké Flute room |
| `-DDITTO_TEST_START_KIND=2` | Start in the stairs room, with the Poké Flute |
| `-DDITTO_TEST_FORM=<species>` | Start transformed (1 Rattata, 2 Meowth, 3 Porygon, 4 Snorlax, 5 Oddish, 6 Caterpie, 7 Paras, 8 Beedrill, 9 Venusaur, 10 Geodude, 11 Diglett, 12 Zubat, 13 Onix) |
| `-DDITTO_TEST_NO_ENEMIES` | Combat rooms spawn nothing |
| `-DDITTO_TEST_FLOOR=<n>` | Start on floor n (2 Viridian Forest, 3 Rock Tunnel) |
| `-DDITTO_TEST_OVERGROWN` | Every door of the start room is overgrown with bushes |

```
make TARGET=test-boss BUILD=build-test-boss USERFLAGS="-DDITTO_TEST_START_KIND=2 -DDITTO_TEST_FORM=1"
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
