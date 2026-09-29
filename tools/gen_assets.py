#!/usr/bin/env python3
"""Generates the placeholder art, the room background and include/room_data.h."""

import json
import random
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
GRAPHICS = ROOT / "graphics"
INCLUDE = ROOT / "include"

TRANSPARENT = (255, 0, 255)

COLORS = {
    "k": (24, 16, 32),
    "w": (240, 240, 232),
    "m": (104, 48, 128),
    "p": (200, 144, 216),
    "h": (236, 204, 244),
    "d": (160, 104, 184),
    "r": (160, 104, 176),
    "R": (104, 64, 128),
    "c": (240, 224, 168),
    "C": (200, 176, 120),
    "e": (208, 48, 56),
    "g": (232, 192, 48),
    "G": (176, 128, 24),
    "b": (128, 80, 40),
    "y": (248, 232, 96),
    "B": (80, 120, 200),
}

DITTO = [
    "",
    "",
    "",
    "",
    "    mmm  mmm",
    "   mhhpmmpppm",
    "  mhpppppppppm",
    "  mppkppppkppm",
    " mpppkppppkpppm",
    " mppppppppppppm",
    " mppkppppppkppm",
    "mppppkkkkkkpppdm",
    "mdpppppppppppddm",
    " mddpppppppdddm",
    "  mmmddddddmmm",
    "     mmmmmm",
]

DITTO_SQUISH = [
    "",
    "",
    "",
    "",
    "",
    "",
    "    mmm  mmm",
    "  mmhhpmmppppmm",
    " mhpppkppppkpppm",
    "mppppkppppkppppm",
    "mpppkppppppkpppm",
    "mppppkkkkkkppppm",
    "mdpppppppppppddm",
    "mddpppppppppdddm",
    " mmmdddddddddmm",
    "    mmmmmmmmm",
]

DITTO_FLAT = [
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "   mmmmmmmmmm",
    " mmhhppkppkppmm",
    "mhppppkkkkppppdm",
    "mdppppppppppdddm",
    " mmddddddddddmm",
    "   mmmmmmmmmm",
]

RATTATA_BODY = [
    "",
    "",
    "",
    "          k k",
    "         krkrk",
    "    kkkkkrrrrk",
    "   krrrrrrrrerk",
    "  krrrrrrrrrrcck",
    " kRrrrrrrrrrrcwk",
    "kRkRRrrrrrrcccck",
    " kk kRRRrrrcccck",
    "     kRRRcccck",
]

RATTATA_1 = RATTATA_BODY + [
    "     kkcckkcck",
    "      kk  kk",
]

RATTATA_2 = RATTATA_BODY + [
    "    kcckk kcck",
    "    kk     kk",
]

MEOWTH_BODY = [
    "",
    "  kk        kk",
    " kbck      kcbk",
    " kbcckkkkkkccbk",
    "  kccccggcccck",
    "  kcccgwggccck",
    "  kccccggcccck",
    " kcckekcckekcck",
    "kwkcccccccccckwk",
    " kcccckbbkcccck",
    "  kkcccccccckk",
    "   kCcccccCk",
    "   kCccccCk",
]

MEOWTH_1 = MEOWTH_BODY + [
    "   kcckkcck",
    "   kbbk kbbk",
    "   kkk  kkk",
]

MEOWTH_2 = MEOWTH_BODY + [
    "   kcck kcck",
    "  kbbk   kbbk",
    "  kkk     kkk",
]

SPIT = [
    "",
    "   mm",
    "  mhpm",
    " mhppdm",
    " mpppdm",
    "  mddm",
    "   mm",
]

ENEMY_SHOT = [
    "",
    "  kkkk",
    " keeeek",
    " kewwek",
    " keweek",
    " keeeek",
    "  kkkk",
]

IMPACT = [
    "",
    " y    y",
    "  y  y",
    "   ww",
    "   ww",
    "  y  y",
    " y    y",
]

COIN = [
    "",
    "  kkkk",
    " kgggGk",
    " kgwggk",
    " kggggk",
    " kGgggk",
    "  kkkk",
]

SLASH = [
    "",
    "",
    "        www",
    "          ww",
    "           ww",
    "            ww",
    "             w",
    "             w",
    "             w",
    "             w",
    "            ww",
    "           ww",
    "          ww",
    "        www",
]


def pad(frame, size):
    assert len(frame) <= size, frame
    rows = [row.ljust(size) for row in frame] + [" " * size] * (size - len(frame))
    for row in rows:
        assert len(row) == size, row
    return [row.replace(" ", ".") for row in rows]


def recolor(frame, mapping):
    return ["".join(mapping.get(c, c) for c in row) for row in frame]


def whiten(frame):
    return ["".join(c if c in ". " else "w" for c in row) for row in frame]


def save_indexed(name, pixels, palette, json_data):
    image = Image.new("P", (len(pixels[0]), len(pixels)))
    flat = []
    for color in palette:
        flat.extend(color)
    flat.extend([0] * (768 - len(flat)))
    image.putpalette(flat)
    image.putdata([index for row in pixels for index in row])
    image.save(GRAPHICS / f"{name}.bmp")
    (GRAPHICS / f"{name}.json").write_text(json.dumps(json_data, indent=4) + "\n")


def save_sprite_sheet(name, frames, size):
    frames = [pad(frame, size) for frame in frames]
    keys = ["."] + sorted({c for frame in frames for row in frame for c in row} - {"."})
    assert len(keys) <= 16, (name, keys)
    palette = [TRANSPARENT] + [COLORS[c] for c in keys[1:]]
    pixels = [[keys.index(c) for c in row] for frame in frames for row in frame]
    save_indexed(name, pixels, palette, {"type": "sprite", "height": size})


def save_species(name, walk_1, walk_2):
    own_outline = {"k": "m"}
    save_sprite_sheet(name, [walk_1, walk_2, whiten(walk_1),
                             recolor(walk_1, own_outline), recolor(walk_2, own_outline)], 16)


def save_wave():
    palette = [TRANSPARENT, COLORS["h"], COLORS["p"], COLORS["w"]]
    pixels = []
    for radius in (7, 11, 15):
        for y in range(32):
            row = []
            for x in range(32):
                distance = ((x - 15.5) ** 2 + (y - 15.5) ** 2) ** 0.5
                offset = distance - radius
                row.append(3 if -0.5 <= offset < 0.5 else 1 if -1.5 <= offset < -0.5 else 2 if -2.5 <= offset < -1.5 else 0)
            pixels.append(row)
    save_indexed("wave", pixels, palette, {"type": "sprite", "height": 32})


def save_hp_bar():
    fill_colors = [(72, 200, 96), (232, 192, 48), (224, 64, 56)]
    palette = [TRANSPARENT, (24, 16, 32), (72, 72, 80), (240, 240, 232)] + fill_colors
    inner = 28
    pixels = []
    for color_index in range(3):
        for fill in range(inner + 1):
            for y in range(8):
                row = []
                for x in range(32):
                    if y in (1, 6) and 1 <= x <= 30 or x in (1, 30) and 1 <= y <= 6:
                        row.append(3)
                    elif 2 <= y <= 5 and 2 <= x <= 29:
                        row.append(4 + color_index if x - 2 < fill else 2)
                    else:
                        row.append(0)
                pixels.append(row)
    save_indexed("hp_bar", pixels, palette, {"type": "sprite", "height": 8})


def save_text_box():
    palette = [TRANSPARENT, (240, 240, 232), (40, 48, 88), (120, 144, 200)]
    pixels = [[0] * 256 for _ in range(256)]
    top, bottom, left, right = 182, 207, 9, 246
    for y in range(top, bottom + 1):
        for x in range(left, right + 1):
            edge = y in (top, bottom) or x in (left, right)
            inner_edge = y in (top + 1, bottom - 1) or x in (left + 1, right - 1)
            pixels[y][x] = 1 if edge else 3 if inner_edge else 2
    save_indexed("text_box", pixels, palette, {"type": "regular_bg"})


# Room: 64x32 tiles = 512x256 pixels, the map centre is world (0, 0).
ROOM_W = 64
ROOM_H = 32


def build_room():
    solid = [[True] * ROOM_W for _ in range(ROOM_H)]
    for y in range(3, ROOM_H - 3):
        for x in range(3, ROOM_W - 3):
            solid[y][x] = False

    for px, py in [(14, 8), (48, 8), (14, 22), (48, 22), (30, 14), (32, 14), (30, 16), (32, 16)]:
        for dy in range(2):
            for dx in range(2):
                solid[py + dy][px + dx] = True

    return solid


LAB_PALETTE = [
    (16, 16, 24),
    (184, 192, 200),
    (144, 152, 168),
    (216, 224, 232),
    (120, 128, 144),
    (48, 56, 72),
    (88, 104, 128),
    (112, 128, 152),
    (72, 84, 104),
    (152, 168, 192),
    (96, 104, 120),
    (72, 176, 168),
]


def floor_tile(x, y, rng, shadow):
    base, grout, light = (4, 10, 2) if shadow else (1, 2, 3)
    tile = [[base] * 8 for _ in range(8)]
    for i in range(8):
        tile[i][7] = grout
        tile[7][i] = grout
    if not shadow:
        tile[0][0] = light
        tile[0][1] = light
        tile[1][0] = light
    if rng.random() < 0.04:
        for i in range(2, 6):
            tile[4][i] = 11
    return tile


def wall_top_tile():
    tile = [[5] * 8 for _ in range(8)]
    tile[0] = [6] * 8
    return tile


def wall_face_tile(x):
    tile = [[7] * 8 for _ in range(8)]
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    if x % 4 == 0:
        for row in range(1, 7):
            tile[row][0] = 8
    if x % 8 == 2:
        tile[3][3] = 11
        tile[3][4] = 11
    return tile


def save_room(solid):
    rng = random.Random(7)
    pixels = [[0] * (ROOM_W * 8) for _ in range(ROOM_H * 8)]
    for ty in range(ROOM_H):
        for tx in range(ROOM_W):
            if solid[ty][tx]:
                below_is_floor = ty + 1 < ROOM_H and not solid[ty + 1][tx]
                tile = wall_face_tile(tx) if below_is_floor else wall_top_tile()
            else:
                shadow = ty > 0 and solid[ty - 1][tx]
                tile = floor_tile(tx, ty, rng, shadow)
            for py in range(8):
                for px in range(8):
                    pixels[ty * 8 + py][tx * 8 + px] = tile[py][px]
    save_indexed("room", pixels, LAB_PALETTE, {"type": "regular_bg"})


def save_room_header(solid):
    rows = ['        "' + "".join("#" if cell else "." for cell in row) + '"' for row in solid]
    header = f"""#ifndef ROOM_DATA_H
#define ROOM_DATA_H

// Generated by tools/gen_assets.py. Do not edit.

namespace room_data
{{
    constexpr int width = {ROOM_W};
    constexpr int height = {ROOM_H};

    constexpr const char* rows[height] = {{
{("," + chr(10)).join(rows)}
    }};
}}

#endif
"""
    (INCLUDE / "room_data.h").write_text(header)


def main():
    GRAPHICS.mkdir(exist_ok=True)
    INCLUDE.mkdir(exist_ok=True)
    for old in ["player", "enemy", "gun", "hud", "bullets"]:
        for suffix in [".bmp", ".json"]:
            (GRAPHICS / f"{old}{suffix}").unlink(missing_ok=True)

    save_sprite_sheet("ditto", [DITTO, DITTO_SQUISH, whiten(DITTO), DITTO, DITTO_SQUISH, DITTO_FLAT], 16)
    save_species("rattata", RATTATA_1, RATTATA_2)
    save_species("meowth", MEOWTH_1, MEOWTH_2)
    save_sprite_sheet("projectiles", [SPIT, ENEMY_SHOT, IMPACT, COIN], 8)
    save_sprite_sheet("slash", [SLASH], 16)
    save_wave()
    save_hp_bar()
    save_text_box()

    solid = build_room()
    save_room(solid)
    save_room_header(solid)


if __name__ == "__main__":
    main()
