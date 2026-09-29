#!/usr/bin/env python3
"""Generates the placeholder sprites, tilesets and palettes."""

import json
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
GRAPHICS = ROOT / "graphics"

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
    "q": (232, 120, 152),
    "T": (56, 104, 128),
    "t": (32, 64, 88),
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

PORYGON_1 = [
    "",
    "",
    "       kkk",
    "      kqqBk",
    "  kk  kqwkBk",
    " kBBkkqqqqBk",
    " kBBBqqqqqk",
    "  kkqqqqqBBk",
    "   kqqqqBBBk",
    "  kqqkkqBBBk",
    "  kqk  kBBk",
    "  kk    kk",
]

PORYGON_2 = [""] + PORYGON_1[:-1]

TRI = [
    "",
    "   kk",
    "  kyyk",
    "  kyyk",
    " keeBBk",
    " keeBBk",
    " kkkkkk",
]

PSYBEAM = [
    "",
    "  kkkk",
    " kqhhqk",
    " khkkhk",
    " khkkhk",
    " kqhhqk",
    "  kkkk",
]

POKE_FLUTE = [
    "",
    "",
    "",
    "",
    "",
    "  kkkkkkkkkkkk",
    " kBBBBBBBBBBBBk",
    " kBwBkBkBkBBBBk",
    " kBBBBBBBBBBBBk",
    "  kkkkkkkkkkkk",
]


def ellipse(grid, cx, cy, rx, ry, color):
    for y in range(len(grid)):
        for x in range(len(grid[0])):
            if ((x - cx) / rx) ** 2 + ((y - cy) / ry) ** 2 <= 1:
                grid[y][x] = color


def outline(grid):
    size = len(grid)
    result = [row[:] for row in grid]
    for y in range(size):
        for x in range(size):
            if grid[y][x] != ".":
                continue
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nx, ny = x + dx, y + dy
                if 0 <= nx < size and 0 <= ny < size and grid[ny][nx] != ".":
                    result[y][x] = "k"
                    break
    return result


def snorlax_frame(step, asleep):
    grid = [["."] * 32 for _ in range(32)]
    foot = 1 if step else 0
    ellipse(grid, 8.5, 27.5 - foot, 5, 3.5, "c")
    ellipse(grid, 23.5, 27.5 - (1 - foot), 5, 3.5, "c")
    ellipse(grid, 16, 18.5, 13.5, 10.5, "T")
    ellipse(grid, 16, 20.5, 9.5, 8, "c")
    ellipse(grid, 16, 9, 9.5, 6.5, "T")
    ellipse(grid, 16, 10, 6.5, 4.5, "c")
    for ear_x in (8, 23):
        for y in range(2, 6):
            for x in range(ear_x - (y - 2) // 2, ear_x + 2 + (y - 2) // 2):
                grid[y][x] = "T"
    for x in range(11, 14):
        grid[9][x] = "t"
    for x in range(18, 21):
        grid[9][x] = "t"
    if asleep:
        for x in range(14, 18):
            grid[12][x] = "t"
        grid[11][15] = grid[11][16] = "w"
    else:
        for x in range(13, 19):
            grid[12][x] = "t"
        grid[11][13] = grid[11][18] = "w"
    for x in (6, 8, 10):
        grid[29 - foot][x] = "b"
    for x in (21, 23, 25):
        grid[29 - (1 - foot)][x] = "b"
    for x in (2, 29):
        for y in range(16, 21):
            grid[y][x] = "T"
    result = outline(grid)
    if asleep:
        for x, y in ((26, 1), (27, 1), (28, 1), (28, 2), (27, 3), (26, 4), (27, 4), (28, 4)):
            result[y][x] = "w"
    return ["".join(row) for row in result]


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


def save_species(name, walk_1, walk_2, size=16, extra_frames=()):
    own_outline = {"k": "m"}
    save_sprite_sheet(name, [walk_1, walk_2, whiten(walk_1),
                             recolor(walk_1, own_outline), recolor(walk_2, own_outline), *extra_frames], size)


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
    (208, 64, 72),
    (240, 240, 232),
    (232, 192, 48),
    (40, 40, 56),
]


def blank(value):
    return [[value] * 8 for _ in range(8)]


def floor_tile(light):
    tile = blank(1)
    for i in range(8):
        tile[i][7] = 2
        tile[7][i] = 2
    if light:
        tile[0][0] = tile[0][1] = tile[1][0] = 3
    return tile


def shadow_tile():
    tile = blank(4)
    for i in range(8):
        tile[i][7] = 10
        tile[7][i] = 10
    return tile


def crack_tile():
    tile = floor_tile(False)
    for i in range(2, 6):
        tile[4][i] = 11
    return tile


def wall_top_tile():
    tile = blank(5)
    tile[0] = [6] * 8
    return tile


def wall_face_tile():
    tile = blank(7)
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    for row in range(1, 7):
        tile[row][0] = 8
    return tile


def door_tile():
    tile = blank(8)
    for row in (1, 3, 5):
        tile[row] = [15] * 8
    tile[7] = [14, 14, 15, 15, 14, 14, 15, 15]
    return tile


def stairs_tiles():
    big = [[15] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 13
            big[top + 1][x] = 3
            big[top + 2][x] = 9
    for y in range(16):
        big[y][0] = big[y][15] = 5
    return split_quad(big)


def split_quad(big):
    return [[row[x0:x0 + 8] for row in big[y0:y0 + 8]] for y0 in (0, 8) for x0 in (0, 8)]










LAB_TILES = [
    blank(0),
    floor_tile(False),
    floor_tile(True),
    shadow_tile(),
    wall_top_tile(),
    wall_face_tile(),
    door_tile(),
    *stairs_tiles(),
    crack_tile(),
]

OVERLAY_PALETTE = [
    TRANSPARENT,
    (16, 16, 24),
    (56, 56, 72),
    (96, 96, 112),
    (152, 160, 176),
    (240, 240, 232),
    (200, 144, 216),
    (104, 48, 128),
    (232, 192, 48),
    (208, 64, 72),
]


def room_marker(fill, border, icon=None, icon_color=None):
    big = [[0] * 16 for _ in range(16)]
    for y in range(1, 15):
        for x in range(1, 15):
            edge = y in (1, 14) or x in (1, 14)
            big[y][x] = border if edge else fill
    if icon == "stairs":
        for step in range(3):
            for x in range(4 + step * 2, 12):
                big[10 - step * 2][x] = icon_color
                big[11 - step * 2][x] = icon_color
    return split_quad(big)


def connector(horizontal):
    tile = blank(0)
    for i in range(8):
        if horizontal:
            tile[3][i] = tile[4][i] = 4
        else:
            tile[i][3] = tile[i][4] = 4
    return tile


OVERLAY_TILES = [
    blank(0),
    blank(1),
    *room_marker(2, 3),
    *room_marker(4, 5),
    *room_marker(6, 7),
    *room_marker(4, 5, "stairs", 8),
    connector(True),
    connector(False),
]


def save_tiles(name, tiles, palette):
    pixels = [[0] * (8 * len(tiles)) for _ in range(8)]
    for index, tile in enumerate(tiles):
        for y in range(8):
            for x in range(8):
                pixels[y][index * 8 + x] = tile[y][x]
    save_indexed(f"{name}_tiles", pixels, palette, {"type": "regular_bg_tiles", "bpp_mode": "bpp_4"})
    save_indexed(f"{name}_palette", [[0] * 8 for _ in range(8)], palette,
                 {"type": "bg_palette", "bpp_mode": "bpp_4", "colors_count": 16})


def main():
    GRAPHICS.mkdir(exist_ok=True)
    for old in ["room", "chansey"]:
        for suffix in [".bmp", ".json"]:
            (GRAPHICS / f"{old}{suffix}").unlink(missing_ok=True)

    save_sprite_sheet("ditto", [DITTO, DITTO_SQUISH, whiten(DITTO), DITTO, DITTO_SQUISH, DITTO_FLAT], 16)
    save_species("rattata", RATTATA_1, RATTATA_2)
    save_species("meowth", MEOWTH_1, MEOWTH_2)
    save_species("porygon", PORYGON_1, PORYGON_2)
    save_species("snorlax", snorlax_frame(0, False), snorlax_frame(1, False), 32, [snorlax_frame(0, True)])
    save_sprite_sheet("poke_flute", [POKE_FLUTE], 16)
    save_sprite_sheet("projectiles", [SPIT, ENEMY_SHOT, IMPACT, COIN, TRI, PSYBEAM], 8)
    save_sprite_sheet("slash", [SLASH], 16)
    save_wave()
    save_hp_bar()
    save_text_box()
    save_tiles("lab", LAB_TILES, LAB_PALETTE)
    save_tiles("overlay", OVERLAY_TILES, OVERLAY_PALETTE)


if __name__ == "__main__":
    main()
