#!/usr/bin/env python3
"""Builds docs/artbook from the game's graphics and data tables."""

import re
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
GRAPHICS = ROOT / "graphics"
OUTPUT = ROOT / "docs" / "artbook"
IMAGES = OUTPUT / "images"
SCALE = 4
TRANSPARENT = (255, 0, 255)

FLOOR_MECHANICS = [
    "Tutorial lab. Doors lock until the room is clear. Find the Poké Flute to wake Snorlax.",
    "Tall grass hides wild Pokémon. Bushes block side doors: a Grass form uses Cut.",
    "Darkness: only a circle of light around Ditto. Diglett travel underground.",
    "Rivers with currents and ponds. Only Water and Flying forms swim; every river has a bridge.",
    "Floor plates charge and shock; the lights flicker. Ground types are immune.",
    "Lava cycles crust, glow and molten; embers fall from the ceiling. Fire types walk on lava.",
    "Slippery ice patches. Frozen side doors: a Fire form melts them. Freeze status.",
    "Wind bands push walkers; pits drop Ditto one room back. Flying forms ignore both.",
    "Spinner arrow lanes and poison gas vents. One room drops the Silph Scope.",
    "Three waves per room, no held items. Cracked rocks: a Fighting form uses Rock Smash.",
    "Dark tower. Ghosts are invisible without the Silph Scope. Spirit barriers need a Ghost form.",
    "Large rooms with waterfalls and whirlpools.",
    "Warp pads send Ditto to their partner. Abra and Kadabra teleport.",
]

BOSS_SPRITES = {
    "snorlax": ["snorlax"],
    "venusaur": ["venusaur"],
    "onix": ["onix", "onix_segment"],
    "gyarados": ["magikarp", "gyarados"],
    "zapdos": ["zapdos"],
    "moltres": ["moltres"],
    "articuno": ["articuno"],
    "pidgeot": ["pidgeot"],
    "team_rocket": ["arbok", "weezing", "balloon"],
    "hitmon": ["hitmonlee", "hitmonchan"],
    "gengar": ["gengar"],
    "dragonite": ["dragonite"],
    "mewtwo": ["mewtwo"],
}

SHARED_SHEETS = [
    ("ditto", 16, "Ditto: walk, squish, white (Transform), own walk, own squish, flat (dodge)"),
    ("mew", 16, "Mew (ending)"),
    ("projectiles", 8, "Projectiles"),
    ("water_projectiles", 8, "Water, dragon, ice and ghost projectiles"),
    ("electric_projectiles", 8, "Electric, fire and flying projectiles"),
    ("clouds", 16, "Clouds: Stun Spore, Sleep Powder, Fire Spin, Smog, Blizzard, Whirlwind"),
    ("slash", 16, "Melee slash"),
    ("wave", 32, "Struggle effort lines"),
    ("pickups", 16, "Item ball, journal page, Silph Scope"),
    ("poke_flute", 16, "Poké Flute"),
    ("light", 64, "Light circle mask (dark floors)"),
]


def read(path):
    return (ROOT / path).read_text()


def enum_names(header, enum):
    body = re.search(r"enum class " + enum + r"\s*\{(.*?)\};", read(header), re.S).group(1)
    return [name.strip() for name in body.split(",") if name.strip()]


def parse_moves():
    names = enum_names("include/moves.h", "move_id")
    rows = re.findall(r'\{ "([^"]+)", "[^"]+", pokemon_type::(\w+), (\d+), (\d+), (\w+)', read("src/moves.cpp"))
    return {name: {"name": row[0], "type": row[1], "power": int(row[2]), "pp": int(row[3])}
            for name, row in zip(names, rows)}


def parse_species():
    rows = re.findall(r'\{ "([^"]+)", pokemon_type::(\w+), pokemon_type::(\w+), (\d+), [\d.]+, '
                      r'move_id::(\w+),\s*move_id::(\w+),\s*&bn::sprite_items::(\w+)', read("src/species.cpp"))
    return {row[6]: {"name": row[0], "types": [t for t in row[1:3] if t != "none"], "hp": int(row[3]),
                     "move_a": row[4], "move_b": row[5]} for row in rows}


def parse_themes():
    text = read("src/floor_theme.cpp")
    pattern = (r'\{ "([^"]+)", &bn::regular_bg_tiles_items::(\w+),.*?\{ ((?:\{ species_id::\w+, \d+ \},?\s*)+)\}, '
               r'\d+,.*?boss_kind::(\w+),\s*species_id::(\w+)')
    themes = []
    for name, tiles, spawns, boss, rare in re.findall(pattern, text, re.S):
        themes.append({"name": name, "tiles": tiles.replace("_tiles", ""),
                       "spawns": re.findall(r"species_id::(\w+)", spawns), "boss": boss, "rare": rare})
    return themes


def to_rgba(image):
    image = image.convert("RGBA")
    source = image.get_flattened_data() if hasattr(image, "get_flattened_data") else image.getdata()
    pixels = [(0, 0, 0, 0) if pixel[:3] == TRANSPARENT else pixel for pixel in source]
    image.putdata(pixels)
    return image


def sheet_frames(name, size):
    image = to_rgba(Image.open(GRAPHICS / f"{name}.bmp"))
    count = image.size[1] // size
    return [image.crop((0, index * size, image.size[0], index * size + size)) for index in range(count)]


def save_strip(frames, file_name, gap=2):
    width = sum(frame.size[0] for frame in frames) * SCALE + gap * SCALE * (len(frames) - 1)
    height = max(frame.size[1] for frame in frames) * SCALE
    strip = Image.new("RGBA", (width, height), (0, 0, 0, 0))
    x = 0
    for frame in frames:
        strip.paste(frame.resize((frame.size[0] * SCALE, frame.size[1] * SCALE), Image.NEAREST), (x, 0))
        x += frame.size[0] * SCALE + gap * SCALE
    strip.save(IMAGES / file_name)
    return file_name


def species_image(sprite):
    size = 32 if Image.open(GRAPHICS / f"{sprite}.bmp").size[0] == 32 else 16
    frames = sheet_frames(sprite, size)
    chosen = [frames[index] for index in (0, 1, 3) if index < len(frames)]
    if len(frames) > 5:
        chosen.append(frames[5])
    return save_strip(chosen, f"{sprite}.png")


def tileset_image(name):
    image = to_rgba(Image.open(GRAPHICS / f"{name}_tiles.bmp"))
    tiles = [image.crop((x, 0, x + 8, 8)) for x in range(0, image.size[0], 8)]
    return save_strip(tiles, f"{name}_tiles.png", gap=1)


def move_label(moves, move_id, show_pp):
    move = moves[move_id]
    power = move["power"] if move["power"] else "—"
    pp = f", {move['pp']} PP" if move["pp"] and show_pp else ""
    return f"{move['name']} ({move['type'].title()}, {power}{pp})"


def species_row(species, moves, sprite, role):
    data = species.get(sprite)
    image = species_image(sprite)
    if not data:
        return f"| ![{sprite}](images/{image}) | {sprite.replace('_', ' ').title()} | | | | {role} |"
    types = " / ".join(t.title() for t in data["types"])
    return (f"| ![{data['name']}](images/{image}) | **{data['name']}** | {types} | {move_label(moves, data['move_a'], False)} "
            f"| {move_label(moves, data['move_b'], True)} | {role} |")


def main():
    IMAGES.mkdir(parents=True, exist_ok=True)
    for old in IMAGES.glob("*.png"):
        old.unlink()

    moves = parse_moves()
    species = parse_species()
    themes = parse_themes()
    sprite_of = {data["name"]: sprite for sprite, data in species.items()}
    sprite_by_id = {sprite.replace("_", ""): sprite for sprite in species}

    lines = [
        "# Ditto's Climb — art book",
        "",
        "Every sprite and tileset in the game, per floor. All art is generated placeholder art from",
        "`tools/gen_assets.py`; a hand-drawn BMP of the same size and palette limit (16 colours) replaces it.",
        "",
        "Regenerate this page with `make artbook`.",
        "",
        "Sprite strips show: walk 1, walk 2, player form (pink outline), and the special frame (asleep, charging,",
        "mound) when the Pokémon has one.",
        "",
    ]

    header = "| Sprite | Pokémon | Type | Move A | Move B | Role |\n|---|---|---|---|---|---|"

    for number, theme in enumerate(themes, start=1):
        lines += [f"## {number}F · {theme['name'].title()}", "", FLOOR_MECHANICS[number - 1], "",
                  f"![{theme['name']} tiles](images/{tileset_image(theme['tiles'])})", "", header]
        for spawn in theme["spawns"]:
            lines.append(species_row(species, moves, sprite_by_id.get(spawn.replace("_", ""), spawn), "Wild"))
        lines.append(species_row(species, moves, sprite_by_id.get(theme["rare"].replace("_", ""), theme["rare"]),
                                 "Rare (side room)"))
        for sprite in BOSS_SPRITES.get(theme["boss"], []):
            lines.append(species_row(species, moves, sprite, "Boss"))
        lines.append("")

    lines += ["## Shared art", ""]
    for name, size, caption in SHARED_SHEETS:
        image = save_strip(sheet_frames(name, size), f"shared_{name}.png")
        lines += [f"**{caption}**", "", f"![{name}](images/{image})", ""]

    hp_frames = sheet_frames("hp_bar", 8)
    lines += ["**HP bar** (green, yellow, red at full)", "",
              f"![hp bar](images/{save_strip([hp_frames[28], hp_frames[57], hp_frames[86]], 'shared_hp_bar.png')})",
              ""]

    overlay = to_rgba(Image.open(GRAPHICS / "overlay_tiles.bmp"))
    tiles = [overlay.crop((x, 0, x + 8, 8)) for x in range(0, overlay.size[0], 8)]
    lines += ["**Floor map tiles**", "", f"![map](images/{save_strip(tiles, 'shared_map_tiles.png', gap=1)})", ""]

    (OUTPUT / "README.md").write_text("\n".join(lines) + "\n")
    print(f"{len(themes)} floors, {len(list(IMAGES.glob('*.png')))} images")


if __name__ == "__main__":
    main()
