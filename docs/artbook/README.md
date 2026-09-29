# Ditto's Climb — art book

Every sprite and tileset in the game, per floor, with a sample room of each floor's tiles. All art is generated placeholder art from
`tools/gen_assets.py`; a hand-drawn BMP of the same size and palette limit (16 colours) replaces it.

Regenerate this page with `make artbook`.

Sprite strips show: walk 1, walk 2, player form (pink outline), and the special frame (asleep, charging,
mound) when the Pokémon has one.

## 1F · Cinnabar Lab

Tutorial lab. Doors lock until the room is clear. Find the Poké Flute to wake Snorlax.

![CINNABAR LAB room](images/lab_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![RATTATA](images/rattata.png) | ![shiny rattata](images/rattata_shiny.png) | **RATTATA** | Normal | TACKLE (Normal, 40) | HYPER FANG (Normal, 80, 15 PP) | Wild |
| ![MEOWTH](images/meowth.png) | ![shiny meowth](images/meowth_shiny.png) | **MEOWTH** | Normal | SCRATCH (Normal, 40) | PAY DAY (Normal, 40, 20 PP) | Wild |
| ![PORYGON](images/porygon.png) | ![shiny porygon](images/porygon_shiny.png) | **PORYGON** | Normal | TRI ATTACK (Normal, 30) | PSYBEAM (Psychic, 65, 15 PP) | Wild |
| ![MACHOP](images/machop.png) | ![shiny machop](images/machop_shiny.png) | **MACHOP** | Fighting | KARATE CHOP (Fighting, 50) | LOW KICK (Fighting, 60, 15 PP) | Rare (side room) |
| ![SNORLAX](images/snorlax.png) | ![shiny snorlax](images/snorlax_shiny.png) | **SNORLAX** | Normal | HEADBUTT (Normal, 70) | BODY SLAM (Normal, 85, 10 PP) | Boss |

## 2F · Viridian Forest

Tall grass hides wild Pokémon. Bushes block side doors: a Grass form uses Cut.

![VIRIDIAN FOREST room](images/forest_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![ODDISH](images/oddish.png) | ![shiny oddish](images/oddish_shiny.png) | **ODDISH** | Grass / Poison | ABSORB (Grass, 30) | STUN SPORE (Grass, —, 10 PP) | Wild |
| ![CATERPIE](images/caterpie.png) | ![shiny caterpie](images/caterpie_shiny.png) | **CATERPIE** | Bug | TACKLE (Normal, 40) | STRING SHOT (Bug, 10, 20 PP) | Wild |
| ![PARAS](images/paras.png) | ![shiny paras](images/paras_shiny.png) | **PARAS** | Bug / Grass | SCRATCH (Normal, 40) | LEECH LIFE (Bug, 50, 15 PP) | Wild |
| ![BEEDRILL](images/beedrill.png) | ![shiny beedrill](images/beedrill_shiny.png) | **BEEDRILL** | Bug / Poison | TWINEEDLE (Bug, 25) | FURY ATTACK (Normal, 60, 15 PP) | Wild |
| ![CHARMANDER](images/charmander.png) | ![shiny charmander](images/charmander_shiny.png) | **CHARMANDER** | Fire | EMBER (Fire, 40) | FLAMETHROWER (Fire, 55, 10 PP) | Rare (side room) |
| ![VENUSAUR](images/venusaur.png) | ![shiny venusaur](images/venusaur_shiny.png) | **VENUSAUR** | Grass / Poison | RAZOR LEAF (Grass, 35) | SOLAR BEAM (Grass, 120, 5 PP) | Boss |

## 3F · Rock Tunnel

Darkness: only a circle of light around Ditto. Diglett travel underground.

![ROCK TUNNEL room](images/cave_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![GEODUDE](images/geodude.png) | ![shiny geodude](images/geodude_shiny.png) | **GEODUDE** | Rock / Ground | ROCK THROW (Rock, 50) | ROLLOUT (Rock, 60, 15 PP) | Wild |
| ![ZUBAT](images/zubat.png) | ![shiny zubat](images/zubat_shiny.png) | **ZUBAT** | Poison / Flying | LEECH LIFE (Bug, 50) | SUPERSONIC (Normal, —, 15 PP) | Wild |
| ![DIGLETT](images/diglett.png) | ![shiny diglett](images/diglett_shiny.png) | **DIGLETT** | Ground | SCRATCH (Normal, 40) | DIG (Ground, 80, 10 PP) | Wild |
| ![BULBASAUR](images/bulbasaur.png) | ![shiny bulbasaur](images/bulbasaur_shiny.png) | **BULBASAUR** | Grass / Poison | VINE WHIP (Grass, 45) | LEECH SEED (Grass, 30, 10 PP) | Rare (side room) |
| ![ONIX](images/onix.png) | ![shiny onix](images/onix_shiny.png) | **ONIX** | Rock / Ground | ROCK THROW (Rock, 50) | SLAM (Normal, 80, 10 PP) | Boss |
| ![onix_segment](images/onix_segment.png) |  | Onix Segment | | | | Boss |

## 4F · Underground Lake

Rivers with currents and ponds. Only Water and Flying forms swim; every river has a bridge.

![UNDERGROUND LAKE room](images/lake_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![MAGIKARP](images/magikarp.png) | ![shiny magikarp](images/magikarp_shiny.png) | **MAGIKARP** | Water | SPLASH (Water, —) | TACKLE (Normal, 40, 35 PP) | Wild |
| ![POLIWAG](images/poliwag.png) | ![shiny poliwag](images/poliwag_shiny.png) | **POLIWAG** | Water | BUBBLE (Water, 25) | HYPNOSIS (Psychic, —, 15 PP) | Wild |
| ![STARYU](images/staryu.png) | ![shiny staryu](images/staryu_shiny.png) | **STARYU** | Water | WATER GUN (Water, 40) | SWIFT (Normal, 25, 15 PP) | Wild |
| ![HORSEA](images/horsea.png) | ![shiny horsea](images/horsea_shiny.png) | **HORSEA** | Water | WATER GUN (Water, 40) | BUBBLEBEAM (Water, 45, 15 PP) | Wild |
| ![PIKACHU](images/pikachu.png) | ![shiny pikachu](images/pikachu_shiny.png) | **PIKACHU** | Electric | THUNDERSHOCK (Electric, 40) | QUICK ATTACK (Normal, 40, 30 PP) | Rare (side room) |
| ![MAGIKARP](images/magikarp.png) | ![shiny magikarp](images/magikarp_shiny.png) | **MAGIKARP** | Water | SPLASH (Water, —) | TACKLE (Normal, 40, 35 PP) | Boss |
| ![GYARADOS](images/gyarados.png) | ![shiny gyarados](images/gyarados_shiny.png) | **GYARADOS** | Water / Flying | BITE (Normal, 60) | HYDRO PUMP (Water, 110, 5 PP) | Boss |

## 5F · Power Plant

Floor plates charge and shock; the lights flicker. Ground types are immune.

![POWER PLANT room](images/plant_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![PIKACHU](images/pikachu.png) | ![shiny pikachu](images/pikachu_shiny.png) | **PIKACHU** | Electric | THUNDERSHOCK (Electric, 40) | QUICK ATTACK (Normal, 40, 30 PP) | Wild |
| ![VOLTORB](images/voltorb.png) | ![shiny voltorb](images/voltorb_shiny.png) | **VOLTORB** | Electric | SONICBOOM (Normal, 40) | SELFDESTRUCT (Normal, 130, 1 PP) | Wild |
| ![MAGNEMITE](images/magnemite.png) | ![shiny magnemite](images/magnemite_shiny.png) | **MAGNEMITE** | Electric | THUNDERSHOCK (Electric, 40) | THUNDER WAVE (Electric, —, 20 PP) | Wild |
| ![SANDSHREW](images/sandshrew.png) | ![shiny sandshrew](images/sandshrew_shiny.png) | **SANDSHREW** | Ground | SCRATCH (Normal, 40) | DIG (Ground, 80, 10 PP) | Rare (side room) |
| ![ZAPDOS](images/zapdos.png) | ![shiny zapdos](images/zapdos_shiny.png) | **ZAPDOS** | Electric / Flying | THUNDERSHOCK (Electric, 40) | THUNDER (Electric, 110, 5 PP) | Boss |

## 6F · Volcano

Lava cycles crust, glow and molten; embers fall from the ceiling. Fire types walk on lava.

![VOLCANO room](images/volcano_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![VULPIX](images/vulpix.png) | ![shiny vulpix](images/vulpix_shiny.png) | **VULPIX** | Fire | EMBER (Fire, 40) | CONFUSE RAY (Ghost, —, 15 PP) | Wild |
| ![PONYTA](images/ponyta.png) | ![shiny ponyta](images/ponyta_shiny.png) | **PONYTA** | Fire | TACKLE (Normal, 40) | FIRE SPIN (Fire, 35, 15 PP) | Wild |
| ![GROWLITHE](images/growlithe.png) | ![shiny growlithe](images/growlithe_shiny.png) | **GROWLITHE** | Fire | BITE (Normal, 60) | FLAMETHROWER (Fire, 55, 10 PP) | Wild |
| ![MAGMAR](images/magmar.png) | ![shiny magmar](images/magmar_shiny.png) | **MAGMAR** | Fire | FIRE PUNCH (Fire, 55) | SMOG (Poison, 20, 20 PP) | Wild |
| ![SQUIRTLE](images/squirtle.png) | ![shiny squirtle](images/squirtle_shiny.png) | **SQUIRTLE** | Water | WATER GUN (Water, 40) | BUBBLEBEAM (Water, 45, 15 PP) | Rare (side room) |
| ![MOLTRES](images/moltres.png) | ![shiny moltres](images/moltres_shiny.png) | **MOLTRES** | Fire / Flying | EMBER (Fire, 40) | SKY ATTACK (Flying, 140, 5 PP) | Boss |

## 7F · Seafoam Cave

Slippery ice patches. Frozen side doors: a Fire form melts them. Freeze status.

![SEAFOAM CAVE room](images/ice_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![SEEL](images/seel.png) | ![shiny seel](images/seel_shiny.png) | **SEEL** | Water | HEADBUTT (Normal, 70) | AURORA BEAM (Ice, 50, 15 PP) | Wild |
| ![JYNX](images/jynx.png) | ![shiny jynx](images/jynx_shiny.png) | **JYNX** | Ice / Psychic | ICE PUNCH (Ice, 55) | LOVELY KISS (Normal, —, 10 PP) | Wild |
| ![SHELLDER](images/shellder.png) | ![shiny shellder](images/shellder_shiny.png) | **SHELLDER** | Water | CLAMP (Water, 35) | ICE BEAM (Ice, 90, 10 PP) | Wild |
| ![OMANYTE](images/omanyte.png) | ![shiny omanyte](images/omanyte_shiny.png) | **OMANYTE** | Rock / Water | WATER GUN (Water, 40) | ROCK SLIDE (Rock, 60, 10 PP) | Rare (side room) |
| ![ARTICUNO](images/articuno.png) | ![shiny articuno](images/articuno_shiny.png) | **ARTICUNO** | Ice / Flying | ICE SHARD (Ice, 40) | BLIZZARD (Ice, 60, 5 PP) | Boss |

## 8F · The Chasm

Wind bands push walkers; pits drop Ditto one room back. Flying forms ignore both.

![THE CHASM room](images/chasm_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![PIDGEY](images/pidgey.png) | ![shiny pidgey](images/pidgey_shiny.png) | **PIDGEY** | Normal / Flying | GUST (Flying, 35) | QUICK ATTACK (Normal, 40, 30 PP) | Wild |
| ![SPEAROW](images/spearow.png) | ![shiny spearow](images/spearow_shiny.png) | **SPEAROW** | Normal / Flying | PECK (Flying, 35) | FURY ATTACK (Normal, 60, 15 PP) | Wild |
| ![AERODACTYL](images/aerodactyl.png) | ![shiny aerodactyl](images/aerodactyl_shiny.png) | **AERODACTYL** | Rock / Flying | BITE (Normal, 60) | ROCK SLIDE (Rock, 60, 10 PP) | Wild |
| ![KABUTO](images/kabuto.png) | ![shiny kabuto](images/kabuto_shiny.png) | **KABUTO** | Rock / Water | SCRATCH (Normal, 40) | ROCK SLIDE (Rock, 60, 10 PP) | Rare (side room) |
| ![PIDGEOT](images/pidgeot.png) | ![shiny pidgeot](images/pidgeot_shiny.png) | **PIDGEOT** | Normal / Flying | GUST (Flying, 35) | WING ATTACK (Flying, 60, 15 PP) | Boss |

## 9F · Rocket Hideout

Spinner arrow lanes and poison gas vents. One room drops the Silph Scope.

![ROCKET HIDEOUT room](images/hideout_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![KOFFING](images/koffing.png) | ![shiny koffing](images/koffing_shiny.png) | **KOFFING** | Poison | TACKLE (Normal, 40) | SELFDESTRUCT (Normal, 130, 1 PP) | Wild |
| ![EKANS](images/ekans.png) | ![shiny ekans](images/ekans_shiny.png) | **EKANS** | Poison | POISON STING (Poison, 30) | GLARE (Normal, —, 15 PP) | Wild |
| ![GRIMER](images/grimer.png) | ![shiny grimer](images/grimer_shiny.png) | **GRIMER** | Poison | SLUDGE (Poison, 45) | POISON GAS (Poison, —, 20 PP) | Wild |
| ![SLOWPOKE](images/slowpoke.png) | ![shiny slowpoke](images/slowpoke_shiny.png) | **SLOWPOKE** | Water / Psychic | WATER GUN (Water, 40) | CONFUSION (Psychic, 50, 15 PP) | Rare (side room) |
| ![ARBOK](images/arbok.png) | ![shiny arbok](images/arbok_shiny.png) | **ARBOK** | Poison | POISON STING (Poison, 30) | WRAP (Normal, 60, 15 PP) | Boss |
| ![WEEZING](images/weezing.png) | ![shiny weezing](images/weezing_shiny.png) | **WEEZING** | Poison | SLUDGE (Poison, 45) | SELFDESTRUCT (Normal, 130, 1 PP) | Boss |
| ![balloon](images/balloon.png) |  | Balloon | | | | Boss |

## 10F · Fighting Dojo

Three waves per room, no held items. Cracked rocks: a Fighting form uses Rock Smash.

![FIGHTING DOJO room](images/dojo_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![MANKEY](images/mankey.png) | ![shiny mankey](images/mankey_shiny.png) | **MANKEY** | Fighting | KARATE CHOP (Fighting, 50) | THRASH (Normal, 90, 10 PP) | Wild |
| ![MACHOP](images/machop.png) | ![shiny machop](images/machop_shiny.png) | **MACHOP** | Fighting | KARATE CHOP (Fighting, 50) | LOW KICK (Fighting, 60, 15 PP) | Wild |
| ![MACHOKE](images/machoke.png) | ![shiny machoke](images/machoke_shiny.png) | **MACHOKE** | Fighting | KARATE CHOP (Fighting, 50) | SUBMISSION (Fighting, 80, 15 PP) | Wild |
| ![FARFETCH'D](images/farfetchd.png) | ![shiny farfetchd](images/farfetchd_shiny.png) | **FARFETCH'D** | Normal / Flying | PECK (Flying, 35) | WING ATTACK (Flying, 60, 15 PP) | Rare (side room) |
| ![HITMONLEE](images/hitmonlee.png) | ![shiny hitmonlee](images/hitmonlee_shiny.png) | **HITMONLEE** | Fighting | ROLLING KICK (Fighting, 60) | HI JUMP KICK (Fighting, 110, 5 PP) | Boss |
| ![HITMONCHAN](images/hitmonchan.png) | ![shiny hitmonchan](images/hitmonchan_shiny.png) | **HITMONCHAN** | Fighting | FIRE PUNCH (Fire, 55) | MEGA PUNCH (Normal, 80, 10 PP) | Boss |

## 11F · Pokemon Tower

Dark tower. Ghosts are invisible without the Silph Scope. Spirit barriers need a Ghost form.

![POKEMON TOWER room](images/tower_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![GASTLY](images/gastly.png) | ![shiny gastly](images/gastly_shiny.png) | **GASTLY** | Ghost / Poison | LICK (Ghost, 30) | CONFUSE RAY (Ghost, —, 15 PP) | Wild |
| ![HAUNTER](images/haunter.png) | ![shiny haunter](images/haunter_shiny.png) | **HAUNTER** | Ghost / Poison | LICK (Ghost, 30) | NIGHT SHADE (Ghost, 45, 15 PP) | Wild |
| ![CUBONE](images/cubone.png) | ![shiny cubone](images/cubone_shiny.png) | **CUBONE** | Ground | BONE CLUB (Ground, 50) | BONEMERANG (Ground, 45, 10 PP) | Wild |
| ![EXEGGCUTE](images/exeggcute.png) | ![shiny exeggcute](images/exeggcute_shiny.png) | **EXEGGCUTE** | Grass / Psychic | PSYWAVE (Psychic, 40) | CONFUSION (Psychic, 50, 15 PP) | Rare (side room) |
| ![GENGAR](images/gengar.png) | ![shiny gengar](images/gengar_shiny.png) | **GENGAR** | Ghost / Poison | LICK (Ghost, 30) | SHADOW BALL (Ghost, 70, 10 PP) | Boss |

## 12F · Dragon'S Den

Large rooms with waterfalls and whirlpools.

![DRAGON'S DEN room](images/den_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![DRATINI](images/dratini.png) | ![shiny dratini](images/dratini_shiny.png) | **DRATINI** | Dragon | TWISTER (Dragon, 40) | THUNDER WAVE (Electric, —, 20 PP) | Wild |
| ![DRAGONAIR](images/dragonair.png) | ![shiny dragonair](images/dragonair_shiny.png) | **DRAGONAIR** | Dragon | TWISTER (Dragon, 40) | DRAGON RAGE (Dragon, 40, 10 PP) | Wild |
| ![SEADRA](images/seadra.png) | ![shiny seadra](images/seadra_shiny.png) | **SEADRA** | Water | WATER GUN (Water, 40) | BUBBLEBEAM (Water, 45, 15 PP) | Wild |
| ![LAPRAS](images/lapras.png) | ![shiny lapras](images/lapras_shiny.png) | **LAPRAS** | Water / Ice | ICE SHARD (Ice, 40) | ICE BEAM (Ice, 90, 10 PP) | Rare (side room) |
| ![DRAGONITE](images/dragonite.png) | ![shiny dragonite](images/dragonite_shiny.png) | **DRAGONITE** | Dragon / Flying | TWISTER (Dragon, 40) | HYPER BEAM (Normal, 150, 5 PP) | Boss |

## 13F · Cerulean Cave

Warp pads send Ditto to their partner. Abra and Kadabra teleport.

![CERULEAN CAVE room](images/peak_room.png)

| Sprite | Shiny | Pokémon | Type | Move A | Move B | Role |
|---|---|---|---|---|---|---|
| ![ABRA](images/abra.png) | ![shiny abra](images/abra_shiny.png) | **ABRA** | Psychic | PSYWAVE (Psychic, 40) | TELEPORT (Psychic, —, 20 PP) | Wild |
| ![KADABRA](images/kadabra.png) | ![shiny kadabra](images/kadabra_shiny.png) | **KADABRA** | Psychic | PSYWAVE (Psychic, 40) | PSYBEAM (Psychic, 65, 15 PP) | Wild |
| ![DROWZEE](images/drowzee.png) | ![shiny drowzee](images/drowzee_shiny.png) | **DROWZEE** | Psychic | PSYWAVE (Psychic, 40) | HYPNOSIS (Psychic, —, 15 PP) | Wild |
| ![VENOMOTH](images/venomoth.png) | ![shiny venomoth](images/venomoth_shiny.png) | **VENOMOTH** | Bug / Poison | LEECH LIFE (Bug, 50) | PSYBEAM (Psychic, 65, 15 PP) | Rare (side room) |
| ![MEWTWO](images/mewtwo.png) | ![shiny mewtwo](images/mewtwo_shiny.png) | **MEWTWO** | Psychic | PSYWAVE (Psychic, 40) | PSYCHIC (Psychic, 90, 10 PP) | Boss |

## Shared art

**Ditto: walk, squish, white (Transform), own walk, own squish, flat (dodge)**

![ditto](images/shared_ditto.png)

**Mew (ending)**

![mew](images/shared_mew.png)

**Projectiles**

![projectiles](images/shared_projectiles.png)

**Water, dragon, ice and ghost projectiles**

![water_projectiles](images/shared_water_projectiles.png)

**Electric, fire and flying projectiles**

![electric_projectiles](images/shared_electric_projectiles.png)

**Clouds: Stun Spore, Sleep Powder, Fire Spin, Smog, Blizzard, Whirlwind**

![clouds](images/shared_clouds.png)

**Melee slash**

![slash](images/shared_slash.png)

**Struggle effort lines**

![wave](images/shared_wave.png)

**Item ball, journal page, Silph Scope**

![pickups](images/shared_pickups.png)

**Poké Flute**

![poke_flute](images/shared_poke_flute.png)

**Light circle mask (dark floors)**

![light](images/shared_light.png)

**HP bar** (green, yellow, red at full)

![hp bar](images/shared_hp_bar.png)

**Floor map tiles**

![map](images/shared_map_tiles.png)

