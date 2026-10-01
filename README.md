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

`make release VERSION=1.2.0` builds the ROM and publishes it as a GitHub release (the repo is private, so only
people with access can download it). The latest ROM is always on the repo's Releases page.

`make assets` regenerates the placeholder art from `tools/gen_assets.py`. `make artbook` rebuilds the
[art book](docs/artbook/README.md), a page with every sprite and tileset per floor. `make audio` regenerates
the original chiptune music (ProTracker MODs, one per floor plus title, boss, final boss and ending) and the
sound effects (WAVs) in `audio/` from `tools/gen_audio.py`.

Test builds take these flags in `USERFLAGS` (use a separate `BUILD` folder, because `make` does not rebuild on a flag change):

| Flag | Effect |
|---|---|
| `-DDITTO_TEST_START_KIND=1` | Start in a combat room, which is also the key item room (Poké Flute, Silph Scope) |
| `-DDITTO_TEST_START_KIND=2` | Start in the stairs room, with the Poké Flute |
| `-DDITTO_TEST_FORM=<species>` | Start transformed (1 Rattata, 2 Meowth, 3 Porygon, 4 Snorlax, 5 Oddish, 6 Caterpie, 7 Paras, 8 Beedrill, 9 Venusaur, 10 Geodude, 11 Diglett, 12 Zubat, 13 Onix, 14 Magikarp, 15 Poliwag, 16 Staryu, 17 Horsea, 18 Gyarados, 19 Pikachu, 20 Voltorb, 21 Magnemite, 22 Zapdos, 23 Machop, 24 Charmander, 25 Bulbasaur, 26 Sandshrew, 27 Vulpix, 28 Ponyta, 29 Growlithe, 30 Magmar, 31 Squirtle, 32 Moltres, 33 Seel, 34 Jynx, 35 Shellder, 36 Omanyte, 37 Articuno, 38 Pidgey, 39 Spearow, 40 Aerodactyl, 41 Kabuto, 42 Pidgeot, 43 Koffing, 44 Ekans, 45 Grimer, 46 Slowpoke, 47 Arbok, 48 Weezing, 49 Mankey, 50 Machoke, 51 Farfetch'd, 52 Hitmonlee, 53 Hitmonchan, 54 Gastly, 55 Haunter, 56 Cubone, 57 Exeggcute, 58 Gengar, 59 Dratini, 60 Dragonair, 61 Seadra, 62 Lapras, 63 Dragonite, 64 Abra, 65 Kadabra, 66 Drowzee, 67 Venomoth, 68 Mewtwo, 69 Mew) |
| `-DDITTO_TEST_NO_ENEMIES` | Combat rooms spawn nothing |
| `-DDITTO_TEST_FLOOR=<n>` | Start on floor n (2 Viridian Forest, 3 Rock Tunnel, 4 Underground Lake, 5 Power Plant, 6 Volcano, 7 Seafoam Cave, 8 The Chasm, 9 Rocket Hideout, 10 Fighting Dojo, 11 Pokémon Tower, 12 Dragon's Den, 13 Cerulean Cave) |
| `-DDITTO_TEST_OVERGROWN` | Every door of the start room is overgrown with bushes |
| `-DDITTO_TEST_REWARD=<n>` | The start room holds a reward (1 journal page, 2 rare Pokémon, 3 item) |
| `-DDITTO_TEST_ITEM=<n>` | The item reward (0 Charcoal … 8 Leftovers, 9 Quick Claw, 10 Ether, 11 Rare Candy, 12 Potion) |
| `-DDITTO_TEST_FALL` | Start on a pit (floors with pits only) |
| `-DDITTO_TEST_WARP` | Start on a warp pad (floor 13) |
| `-DDITTO_TEST_ENDING=<pages>` | Go straight to the ending with this many journal pages (13 shows the secret) |
| `-DDITTO_TEST_LAYOUT=<n>` | The start combat room uses layout n (0–4; 3 has a centre block) |
| `-DDITTO_TEST_SCOPE` | Start with the Silph Scope |
| `-DDITTO_TEST_SHINY` | Every wild Pokémon and the test form are shiny |
| `-DDITTO_TEST_BOSS_HP=<n>` | The boss has n HP (its maximum too, so Mewtwo does not Recover) |
| `-DDITTO_TEST_DEX` | The Pokédex starts partly filled (seen, used and shiny entries) and the wallet is set to 500 coins at boot |
| `-DDITTO_TEST_MART` | Every floor with items has a Poké Mart, the run starts in it, and the wallet is set to 500 coins at boot |
| `-DDITTO_TEST_PAGES=<n>` | Start with n journal pages (13 unlocks the secret ending) |
| `-DDITTO_TEST_SPECIES=<species>` | Every wild Pokémon is this species |

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
| Select | Use the item in the bag |
| Start | Floor map / continue |
| R (on the title screen) | Options: music and sound volume, saved with the Pokédex |
| Select (on the title screen) | Pokédex of the species seen and the forms used, across runs; D-pad moves, L/R turn pages, A shows a shiny |
| A (on the floor map) | Read the collected journal pages; Left/Right turn, B goes back |
| Select (on the floor map) | Save and quit; A confirms, B goes back |

A saved run shows **A: CONTINUE** on the title screen. Continuing deletes the save, so a run resumes once.
