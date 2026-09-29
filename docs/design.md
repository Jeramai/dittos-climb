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
- Field gates never block the way to the stairs: bushes only grow on doors off the start-to-stairs path, and
  every river has a land bridge. Gated rooms are optional.

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

- Every move has a type, power and an attack pattern: shot, spread, melee, dash, beam, cloud or status.
- Move A (weak) has no PP limit. Move B (strong) has PP; at 0 PP it becomes Struggle.
- **STAB**: a move of the same type as the user does ×1.5.
- If none of a form's moves can affect any Pokémon left in the room (for example a Normal form against
  ghosts), both buttons become Struggle until a foe it can hit appears. Struggle has no type, so it hits everything.

### Type chart

- The Gen 1 type chart, with the modern fixes (Ghost hits Psychic ×2, Bug vs Poison ×0.5).
- Text box messages: *"It's super effective!"*, *"It's not very effective…"*, *"It doesn't affect GASTLY…"*.
- A floor's forms are super effective against a later floor.

### Side rooms

Side rooms are combat rooms off the start-to-stairs path (rooms behind bushes are side rooms too). Each side room
holds a reward that shows once the room is clear:

- **Journal page** (one per floor): a page of the Cinnabar lab journal. Ditto was sample 132 — its Pokédex number.
  If a floor has no side room, its page goes into a room on the path. Thirteen pages; all of them unlock a secret
  ending (later).
- **Rare Pokémon** (at most one per floor): one extra wild Pokémon that is not in the floor's pool and counters its boss: Machop (Snorlax),
  Charmander (Venusaur), Bulbasaur (Onix), Pikachu (Gyarados), Sandshrew (Zapdos).
- **Item** in a Poké Ball. Ditto has one held-item slot; a new held item replaces the old one:
  type boosters (Charcoal, Miracle Seed, Mystic Water, Magnet, Hard Stone, Soft Sand, SilverPowder, Black Belt:
  +20% to that type), Leftovers (slow healing), Quick Claw (shorter cooldowns). Instant items: Ether (refills move
  B's PP; it stays on the floor for base Ditto) and Rare Candy (+5 max HP for base Ditto).

The pause screen shows the held item and the journal count.

### Save and quit

No save slots: a run is meant to be short. Select on the pause screen, then A, stores the run in SRAM and returns
to the title: the floor, the current room, the state of every room (visited, cleared, bushes, rewards), the key
items, the journal pages, and Ditto's HP, form, PP and held item. The title then offers **Continue**, which
resumes in the saved room; an uncleared room fills with Pokémon again and a living boss starts at full HP.
Starting any run deletes the save, so a save cannot be loaded twice.

### Poké Mart (later)

- No Pokémon Center: Transform already heals Ditto.
- A Poké Mart later sells boost items. It needs a currency first, for example Pay Day coins.

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
- Meowth's Pay Day drops coins. Coins buy items at the Poké Mart (later).
- Items: Potion, Oran Berry, Rare Candy, Escape Rope, Poké Flute, Silph Scope.

## Floors

| # | Place | Type | Floor mechanic | Wild Pokémon | Boss | Counter forms |
|---|---|---|---|---|---|---|
| 1 | Cinnabar Lab basement | Normal | Tutorial. Lab doors lock until the room is clear. | Rattata, Meowth, Porygon | Snorlax (wake it with the Poké Flute) | — |
| 2 | Viridian Forest | Grass / Bug | Tall grass hides wild Pokémon until they jump out. Bushes block paths. | Oddish, Caterpie, Beedrill, Paras | Venusaur (Solar Beam charge, Sleep Powder clouds) | — |
| 3 | Rock Tunnel | Rock / Ground | Darkness: only a light circle around Ditto. Diglett pop out of holes. | Geodude, Diglett, Zubat, Onix | Onix (a long body of segments that chases you) | Grass |
| 4 | Underground Lake | Water | Deep water that only Water forms can cross. Currents push you. | Magikarp, Poliwag, Staryu, Horsea | Gyarados (a Magikarp evolves in the fight) | Grass |
| 5 | Power Plant | Electric | Floor plates charge and shock on a timer. Lights flicker. | Pikachu, Voltorb, Magnemite | Zapdos | Ground |
| 6 | Volcano | Fire | Lava rises and falls. Ember rain from the ceiling. | Vulpix, Ponyta, Growlithe, Magmar | Moltres | Water, Rock, Ground |
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
| A | Move 1: weak, no PP limit |
| B | Move 2: strong, uses PP |
| L | Dodge (per form: a roll, a Dig, a Teleport) |
| R (hold) | Lock the aim direction |
| Start | Menu / pause |
| Select (paused) | Save and quit |

## Art

- Own pixel art, not ripped sprites. 16×16 for wild Pokémon, 32×32 or larger for bosses.
- One 16-colour palette per sprite sheet.
- The placeholder art comes from `tools/gen_assets.py`. A hand-made BMP of the same size replaces it.
- The music and sound effects come from `tools/gen_audio.py` and play through Maxmod. Every floor has its own
  theme; bosses switch to a boss theme (Mewtwo has its own) and the floor theme returns when the boss faints.
  A hand-made MOD or WAV with the same name replaces a generated one.

## Milestones

1. **Ditto and floor 1.** (done) Base Ditto with Struggle. Transform. Rattata and Meowth with 2 moves each and PP.
   HP bars, the text box, the type chart, the intro story.
2. **Floor structure.** (done) Random floors of 7–11 rooms, doors that lock until the room is clear, the floor
   map on Start, stairs.
3. **Floor 1 complete.** (done) Porygon (Tri Attack, Psybeam). One combat room drops the Poké Flute. Snorlax
   sleeps on the stairs; the flute wakes it. Body Slam jumps with a shockwave ring; Rest heals once at low HP.
   Its outline gives the Snorlax form (Headbutt, Body Slam).
4. **Floor 2.** (done) Per-floor themes (tileset, door messages, wild Pokémon, boss). Viridian Forest: wild
   Pokémon hide in tall grass until Ditto is near; some doors are overgrown and a Grass form CUTs them. Oddish,
   Caterpie, Paras and Beedrill bring paralysis, poison, sleep and drain moves. Venusaur uses Razor Leaf, Sleep
   Powder clouds and a charged Solar Beam, and speeds up below half HP. Floors 3+ reuse the forest for now.
5. **Floor 3.** (done) Rock Tunnel is dark: a hardware sprite window shows a light circle around Ditto, and wild
   Pokémon outside it are hidden. Geodude (Rock Throw, Rollout), Diglett (travels underground as a mound and pops
   up next to Ditto; Dig), Zubat (zigzag flight, Supersonic confuses: reversed controls). Onix is a head with six
   trailing segments: only the head takes damage, the body blocks shots and hurts on contact; it Slams in a
   straight line (crashing into walls stuns it) and drops Rock Slide on marked spots.
6. **Floor 4.** (done) Underground Lake: combat rooms get a river with a current and a pond. Only Water or Flying
   forms cross water; shots fly over it; outlines of Pokémon that faint in water move to the shore; Ditto washes
   ashore when its form faints in water. Magikarp (Splash: "But nothing happened!"; a Magikarp form evolves into
   Gyarados when a room is cleared), Poliwag (Bubble, Hypnosis), Staryu (Water Gun, Swift), Horsea (Water Gun,
   Bubblebeam). The boss starts as a splashing Magikarp and evolves into Gyarados: Bite lunges, Dragon Rage and a
   telegraphed Hydro Pump; it thrashes (faster bites) below 40% HP.
7. **Floor 5.** (done) Power Plant: patches of floor plates cycle idle, warning glow and shock; the shock hits
   everything standing on them (Ground types are immune), and the lights flicker now and then. Pikachu
   (Thundershock, Quick Attack), Voltorb (Sonic Boom, Self-Destruct: a long flashing warning, then an explosion
   that faints it; a Voltorb form can do the same and loses its shape), Magnemite (Thundershock, Thunder Wave).
   Zapdos orbits Ditto, fires Thundershock bursts, Drill Pecks, and calls Thunder onto marked spots; the plates
   cycle faster during the fight.
8. **Floor 6.** (done) Volcano: lava patches cycle crust, glow and molten, and molten lava burns everything
   except Fire types; embers fall on marked spots near Ditto. Vulpix (Ember, Confuse Ray), Ponyta (Tackle, Fire
   Spin), Growlithe (Bite, Flamethrower), Magmar (Fire Punch, Smog) — Magmar replaces Slugma, which is Gen 2.
   Rare: Squirtle. Moltres orbits Ditto with Flamethrower bursts and a Fire Spin ring, and glows before a Sky
   Attack sweep; the ember rain falls faster in its room.
9. **Floor 7.** (done) Seafoam Cave: ice patches make Ditto slide in a straight line until it hits a wall or
   leaves the ice (Ice forms keep their grip). Side doors can be frozen shut; a Fire form melts the ice (the same
   gate system as the forest bushes). New status Freeze: no action for a moment; Fire and Ice types are immune.
   Seel (Headbutt, Aurora Beam), Jynx (Ice Punch, Lovely Kiss), Shellder (Clamp, Ice Beam). Rare: Omanyte.
   Articuno: Ice Shard bursts, a Blizzard ring of snow clouds, a telegraphed Ice Beam and a diving attack.
10. **Floor 8.** (done) The Chasm: bands of wind push walkers, and pits drop Ditto back into the room it came
    from (a little HP, never a black-out). Flying forms fly over both; wild Pokémon walk around pits. Pidgey (Gust,
    Quick Attack), Spearow (Peck, Fury Attack), Aerodactyl (Bite, Rock Slide). Rare: Kabuto. Pidgeot fires feather
    volleys, summons Whirlwind tornadoes that confuse, dives with Wing Attack, and whips up a Gust that pushes Ditto.
11. **Floor 9.** (done) Rocket Hideout: spinner arrow lanes push Ditto fast in their direction, and poison gas
    vents cycle like the plates (the gas poisons; Poison types are immune). One room drops the Silph Scope, which
    Ditto keeps for the rest of the run (floor 11 needs it) — key items (Poké Flute, Silph Scope) now share one
    system. Koffing (Tackle, Self-Destruct), Ekans (Poison Sting, Glare), Grimer (Sludge, Poison Gas). Rare:
    Slowpoke. Team Rocket: Arbok (Poison Sting fan, Wrap lunge) and Weezing (Sludge, Smog) share one HP bar, while
    Meowth drifts overhead in the balloon throwing Pay Day coins; defeat: "TEAM ROCKET is blasting off again!" and
    outlines of both Arbok and Weezing.
12. **Floor 10.** (done) Fighting Dojo: combat rooms send three waves of three wild Pokémon; held items have no
    effect and side rooms give no items; cracked rocks block side doors and a Fighting form uses Rock Smash.
    Mankey (Karate Chop, Thrash), Machop, Machoke (Karate Chop, Submission). Rare: Farfetch'd. The Dojo Master sends
    out Hitmonlee (Rolling Kick ring, Hi Jump Kick that crashes into walls and hurts itself) or Hitmonchan (Fire,
    Ice and Thunder Punch combos, Mega Punch, a Counter stance that strikes back when hit); its outline is the
    other one — the dojo prize.
13. **Floor 11.** (done) Pokémon Tower is dark; without the Silph Scope Ghost types stay invisible except while
    they wind up an attack ("GHOST: Get out... Get out..."), and Gengar is hard to see. Spirit barriers block side
    doors and a Ghost form phases through. Gastly (Lick, Confuse Ray), Haunter (Lick, Night Shade), Cubone (Bone
    Club, Bonemerang). Rare: Exeggcute. Gengar fires Shadow Ball fans, vanishes and reappears next to Ditto with a
    Lick, and follows Hypnosis with Dream Eater. Base Ditto (Normal) is immune to its Ghost moves.
14. **Floor 12.** (done) Dragon's Den uses the large combat layouts; its rivers are waterfalls (too strong to swim
    up; a land bridge crosses) and its ponds are whirlpools that pull swimmers gently. Dratini (Twister, Thunder
    Wave), Dragonair (Twister, Dragon Rage), Seadra (Water Gun, Bubblebeam). Rare: Lapras. Dragonite fires Twister
    fans and Dragon Rage, rampages with three Outrage lunges and is then tired, and must recharge after a
    telegraphed Hyper Beam.
15. **Floor 13.** (done) Cerulean Cave: paired warp pads send Ditto to their partner. Abra and Kadabra teleport
    around Ditto (an Abra form's B is Teleport), Drowzee uses Hypnosis. Rare: Venomoth. Mewtwo fires Confusion,
    rings of Psychic shots and (below half HP) Swift, raises Barrier, teleports, and uses Recover once.
16. **Ending.** (done) Mew appears, tells Ditto the truth, and Ditto transforms into Mewtwo. With all 13 journal
    pages, a secret scene follows and Ditto transforms into Mew. The run ends and the title screen returns.
17. Music and sound effects: 17 original tracks and 20 effects.
18. Save and quit.
19. Next: bonus floors (Steel, Dark, Fairy), shiny Pokémon, the Poké Mart, real art.
