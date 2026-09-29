# Ditto's Climb — design

A top-down action roguelite for the Game Boy Advance. You play Ditto. Ditto knows one move, Transform, and
climbs a dungeon of 13 type-themed floors by becoming the Pokémon it defeats.

Private fan project. Not for distribution.

## Story

**The failure.** Long ago, scientists in the lab on Cinnabar Island tried to clone Mew. One attempt went wrong:
a small pink blob with a silly face. The scientists wrote "failure" in the log and flushed it down the waste
pipe. The pipe runs deep under the ground, to the bottom of the Unknown Dungeon.

**The voice.** Ditto wakes up in the dark. It knows only Transform. Then it hears a soft voice in its head. It
is Mew, at the very top of the dungeon: *"Come up to me, little one. I can tell you who you really are."*

**The rival.** The scientists made one clone that worked: Mewtwo. Mewtwo hears the voice too. It does not want a
second clone of Mew in the world, so it waits at the top.

**The end.** Ditto defeats Mewtwo and meets Mew. Mew tells Ditto the truth: Ditto is made from Mew, the same as
Mewtwo. Ditto has no true shape, and that is its power. Final gag: Ditto transforms into Mewtwo and waves.

The story leans on the fan theory that Ditto is a failed Mew clone. In Red and Blue, Ditto and Mewtwo both live in
the Unknown Dungeon.

## Core rules

### Transform

- A defeated wild Pokémon faints and leaves its outline on the floor for 5 seconds.
- Base Ditto stands on the outline and presses **B** to Transform. Ditto takes the species' types, stats and its
  two moves.
- A transformed Ditto keeps its pink outline, so it never looks like an enemy.
- The first Transform into a species registers it in the Pokédex.

### Forms and HP

- Only base Ditto can Transform: stand on a fainted Pokémon's outline and press **B**.
- Ditto holds one form at a time and cannot undo it. The form stays until it faints.
- Transform restores Ditto's own HP to full.
- The form has its own HP bar. When it faints, Ditto pops back to base form.
- Only a hit on base Ditto removes Ditto's own HP. Ditto at 0 HP is game over.

### Base Ditto

- Base Ditto only knows Transform, so it attacks with **Struggle**: a short shockwave around Ditto. It is weak,
  and each Struggle that hits costs Ditto 1 HP of recoil.
- This pushes the player to Transform quickly.

### Moves and PP

- Every move has a type, power, PP and an attack pattern: shot, spread, melee, dash, beam, cloud or status.
- A move with 0 PP becomes Struggle.
- **STAB**: a move of the same type as the user does ×1.5.

### Type chart

- The Gen 1 type chart, with the modern fixes (Ghost hits Psychic ×2, Bug vs Poison ×0.5).
- Text box messages: *"It's super effective!"*, *"It's not very effective…"*, *"It doesn't affect GASTLY…"*.
- A floor's forms are super effective against a later floor. Old forms stay useful because of Bill's PC.

### Pokémon Center and Bill's PC

- One Center room per floor. Chansey restores HP and PP.
- Bill's PC stores every form in the Pokédex. Base Ditto can take out any registered form at full HP.

### Field abilities

Some types open paths, like HMs:

| Type | Ability |
|---|---|
| Grass | Cut: clear bushes |
| Rock | Strength: push boulders |
| Ground | Dig: the dodge goes under ground and passes under attacks |
| Water | Surf: swim in deep water |
| Electric | Charge: power dead switches and doors |
| Fire | Melt ice blocks |
| Ice | Freeze water into a bridge |
| Flying | Fly over pits |
| Fighting | Rock Smash: break cracked walls |
| Ghost | Pass through ghost walls |
| Poison | Immune to gas clouds |
| Psychic | Use warp pads without the scramble |

### Pokémon jokes with a purpose

- Magikarp's Splash: *"But nothing happened!"* Stay a Magikarp through a full room and it evolves.
- Snorlax sleeps on the stairs of floor 1. You need the Poké Flute to wake it.
- Rare shiny enemies (1 in 64). A shiny form has other colors and does ×1.25 damage.
- Meowth's Pay Day drops coins. Coins buy items at the Center.
- Items: Potion, Oran Berry, Rare Candy, Escape Rope, Poké Flute, Silph Scope.

## Floors

| # | Place | Type | Floor mechanic | Wild Pokémon | Boss | Counter forms |
|---|---|---|---|---|---|---|
| 1 | Cinnabar Lab basement | Normal | Tutorial. Lab doors lock until the room is clear. | Rattata, Meowth, Porygon | Snorlax (wake it with the Poké Flute) | — |
| 2 | Viridian Forest | Grass / Bug | Tall grass hides wild Pokémon until they jump out. Bushes block paths. | Oddish, Caterpie, Beedrill, Paras | Venusaur (Solar Beam charge, Sleep Powder clouds) | — |
| 3 | Rock Tunnel | Rock / Ground | Darkness: only a light circle around Ditto. Diglett pop out of holes. | Geodude, Diglett, Zubat, Onix | Onix (a long body of segments that chases you) | Grass |
| 4 | Underground Lake | Water | Deep water that only Water forms can cross. Currents push you. | Magikarp, Poliwag, Staryu, Horsea | Gyarados (a Magikarp evolves in the fight) | Grass |
| 5 | Power Plant | Electric | Floor plates charge and shock on a timer. Lights flicker. | Pikachu, Voltorb, Magnemite | Zapdos | Ground |
| 6 | Volcano | Fire | Lava rises and falls. Ember rain from the ceiling. | Vulpix, Ponyta, Growlithe, Slugma | Moltres | Water, Rock, Ground |
| 7 | Seafoam Ice Cave | Ice | Slippery ice floors and sliding puzzles. | Seel, Jynx, Shellder | Articuno | Fire, Rock |
| 8 | The Chasm | Flying | Wind vents push you. A fall into a pit drops you one room back. | Pidgey, Spearow, Aerodactyl | Pidgeot (Gust pushes you around) | Electric, Ice, Rock |
| 9 | Rocket Hideout | Poison | Spinner arrow tiles. Poison gas clouds. Holds the Silph Scope. | Koffing, Ekans, Grimer | Jessie and James (Arbok, Weezing and a Meowth balloon) | Ground |
| 10 | Fighting Dojo | Fighting | Arena rooms in waves. No items allowed. | Machop, Mankey | Hitmonlee or Hitmonchan: you fight one, and you can Transform into the other | Flying |
| 11 | Pokémon Tower | Ghost | Ghosts are invisible without the Silph Scope. Normal forms cannot hit ghosts, and ghosts cannot hurt Normal forms. | Gastly, Haunter, Cubone | Gengar | Ghost |
| 12 | Dragon's Den | Dragon | Waterfalls and whirlpools. Large open rooms. | Dratini, Dragonair | Dragonite | Ice, Dragon |
| 13 | Cerulean Cave peak | Psychic | Warp pads scramble where you go. Hypnosis makes the controls slow. | Abra, Drowzee, Kadabra | Mewtwo | Bug, Ghost |

Bonus floors after the end: Steel, Dark and Fairy.

## Controls

| Button | Action |
|---|---|
| D-pad | Move |
| A | Move 1: weak, a lot of PP |
| B | Move 2: strong, little PP |
| L | Dodge (per form: a roll, a Dig, a Teleport) |
| R (hold) | Lock the aim direction |
| Start | Menu / pause |

## Art

- Own pixel art, not ripped sprites. 16×16 for wild Pokémon, 32×32 or larger for bosses.
- One 16-colour palette per sprite sheet.
- The placeholder art comes from `tools/gen_assets.py`. A hand-made BMP of the same size replaces it.

## Milestones

1. **Ditto and floor 1.** (done) Base Ditto with Struggle. Transform. Rattata and Meowth with 2 moves each and PP.
   HP bars, the text box, the type chart, the intro story.
2. **Floor structure.** (done) Random floors of 7–11 rooms, doors that lock until the room is clear, the floor
   map on Start, stairs, the Pokémon Center (Chansey heals once per floor; Bill's PC gives one form per floor).
3. **Floor 1 complete.** Porygon, the Poké Flute, the Snorlax boss.
4. **Floor 2.** Viridian Forest, tall grass, Cut, Venusaur.
5. Then one floor per milestone.
