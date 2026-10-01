#!/usr/bin/env python3
"""Generates the placeholder sprites, tilesets and palettes."""

import json
import math
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
    "v": (88, 176, 72),
    "V": (40, 112, 48),
    "o": (232, 136, 56),
    "u": (72, 104, 184),
    "U": (40, 56, 120),
    "n": (208, 56, 48),
    "j": (96, 176, 152),
    "J": (48, 112, 104),
    "s": (168, 168, 176),
    "a": (136, 128, 112),
    "A": (88, 80, 72),
    "z": (160, 120, 200),
    "Z": (96, 64, 144),
    "x": (152, 104, 64),
    "i": (136, 184, 232),
    "l": (168, 184, 200),
    "L": (112, 128, 152),
    "O": (160, 152, 192),
    "P": (104, 96, 136),
    "F": (200, 96, 56),
    "f": (240, 160, 96),
    "4": (244, 176, 192),
    "5": (248, 192, 216),
    "6": (216, 136, 176),
    "7": (255, 204, 204),
    "8": (255, 150, 150),
    "9": (192, 80, 80),
    "0": (176, 216, 230),
    "1": (72, 168, 200),
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
    "         kk  kk",
    "        kqqkkqqk",
    "        kqrrrrqk",
    "       krrrrrrrk",
    "       krrrwerrk",
    "      krrrrrrcck",
    " kk  krrrrrrccck",
    "k  kkRrrrrrrckkk",
    "k k kRRrrrrrkwwk",
    " kk kRRrrrrcckwk",
    "    kRRRrrcccck",
    "     kRRRcccck",
]

RATTATA_1 = RATTATA_BODY + [
    "    kkcck kcck",
    "     kk   kk",
]

RATTATA_2 = RATTATA_BODY + [
    "   kcck  kcck",
    "   kk     kk",
]

def _grid_b(rows):
    return [row.replace(".", " ").rstrip() for row in rows]


MEOWTH_BODY = _grid_b([
    "................",
    "..kk........kk..",
    ".kbck......kcbk.",
    ".kbcckkkkkkccbk.",
    "..kcccckkcccck..",
    "..kccckggkccck..",
    ".kcccckGGkcccck.",
    ".kccwkccccwkcck.",
    "kwkccccqqcccckwk",
    ".kkcccckkcccckk.",
    "..kkcccccccckk..",
    "...kCccccccCk...",
    "...kCccccccCkbk.",
    "...kCCccccCCkkbk",
])


MEOWTH_1 = MEOWTH_BODY + _grid_b([
    "...kbbk..kbbk...",
    "...kkk....kkk...",
])


MEOWTH_2 = MEOWTH_BODY + _grid_b([
    "..kbbk....kbbk..",
    "..kkk......kkk..",
])


PORYGON_1 = _grid_b([
    "................",
    "................",
    ".......kkkk.....",
    "......kqqqqk....",
    "......kqwkqkkk..",
    "......kqqqqkBBk.",
    ".....kkqqqqkkkk.",
    "..kkkqqqqqqk....",
    ".kBBkqqqqqqqk...",
    ".kBBBkqqqqqqqk..",
    "..kkkqqqqqqBBk..",
    "....kqqkkkBBBk..",
    "....kBBk..kBBk..",
    "....kkk....kk...",
])


PORYGON_2 = [""] + PORYGON_1[:-1]


ODDISH_TOP = _grid_b([
    "....k......k....",
    "...kvk.kk.kvk...",
    "..kvVvkvvkvVvk..",
    "..kvVVkvVkVVvk..",
    "...kvVVVVVVvk...",
    "....kkVVVVkk....",
    "....kuuuuuuk....",
    "...kuuuuuuuuk...",
    "..kuekuuuuekuk..",
    "..kuuuuuuuuuuk..",
    "..kUuuuuuuuuUk..",
    "...kUuuuuuuUk...",
    "....kUUUUUUk....",
])


ODDISH_1 = ODDISH_TOP + _grid_b(["....kuk..kuk....", "....kkk..kkk...."])


ODDISH_2 = ODDISH_TOP + _grid_b(["...kuk....kuk...", "...kkk....kkk..."])


CATERPIE_1 = _grid_b([
    "................",
    "................",
    "................",
    "............nn..",
    "...........kn...",
    "........kkkkk...",
    ".......kvvvvvk..",
    ".......kvvvykvk.",
    "..kkk.kvvvvyyvk.",
    ".kvvvkkvvvvvvkk.",
    "kvvvvvkvvvvvvk..",
    "kvvvvvkvvvvvvk..",
    "kcvcvckcvcvcvk..",
    ".kkkkkkkkkkkkk..",
    "..c..c..c..c....",
])


CATERPIE_2 = CATERPIE_1[:14] + _grid_b(["...c..c..c..c..."])


PARAS_TOP = _grid_b([
    "................",
    "..kkkk....kkkk..",
    ".knnynk..knynnk.",
    "kncnnnnkknnnncnk",
    "knnnnnnkknnnnnnk",
    ".kkkyykkkkyykkk.",
    "..kooooooooook..",
    ".kooooooooooook.",
    ".kowkoooooowkok.",
    ".kooooccccooook.",
    "..kccookkoocck..",
    "...kkk....kkk...",
])


PARAS_1 = PARAS_TOP + _grid_b(["..kok.kok.kok...", "..kk..kk..kk...."])


PARAS_2 = PARAS_TOP + _grid_b(["...kok.kok.kok..", "...kk..kk..kk..."])


BEEDRILL_1 = _grid_b([
    ".kk..........kk.",
    "kiik........kiik",
    "kiiik.kkkk.kiiik",
    ".kiik.kyyk.kiik.",
    "..kiikyeeykiik..",
    "...kkkyyyykkk...",
    ".kwk..kkkk..kwk.",
    "kwsk.kyyyyk.kswk",
    "kssk.kkkkkk.kssk",
    ".ksk.kyyyyk.ksk.",
    "..kk.kkkkkk.kk..",
    ".....kyyyyk.....",
    "......kkkk......",
    ".......kwk......",
    "........k.......",
])


BEEDRILL_2 = _grid_b([
    "................",
    "kkk..........kkk",
    "kiikk.kkkk.kkiik",
    ".kiiikkyykkiiik.",
    "..kkikyeeykikk..",
    "...kkkyyyykkk...",
]) + BEEDRILL_1[6:]


DIGLETT_GROUND = _grid_b([
    ".kkAAkkkkkkAAkk.",
    "kAaAAaAAaAAaAAak",
    ".kkkkkkkkkkkkkk.",
])


DIGLETT_1 = _grid_b([
    "................",
    "................",
    "................",
    "......kkkk......",
    ".....kxxxxk.....",
    "....kxxxxxxk....",
    "....kxkxxkxk....",
    "....kxxxxxxk....",
    "....kxqqqqxk....",
    "....kxq4qqxk....",
    "....kbxqqxbk....",
    "...kkbxxxxbkk...",
]) + DIGLETT_GROUND


DIGLETT_2 = _grid_b([""] * 5) + DIGLETT_1[3:10] + DIGLETT_GROUND


DIGLETT_MOUND = _grid_b([""] * 10 + [
    "......kkkk......",
    "....kkxbbxkk....",
    "..kkAxbAAbxAkk..",
]) + DIGLETT_GROUND


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


LEAF = [
    "",
    "     kk",
    "   kkvk",
    "  kvvVk",
    " kvvVk",
    " kvVkk",
    "  kk",
]

NEEDLE = [
    "",
    "",
    "",
    " kkkkk",
    "kwwwwsk",
    " kkkkk",
]

STRING = [
    "",
    " w   w",
    "  w w",
    "   w",
    "  w w",
    " w   w",
]

BEAM = [
    "",
    " yyyyyy",
    "ywwwwwwy",
    "ywwwwwwy",
    "ywwwwwwy",
    "ywwwwwwy",
    " yyyyyy",
]


GEODUDE_1 = [
    "",
    "     kkkkkk",
    "   kkaaaaaakk",
    "  kaaaaaaaaaak",
    " kaaAaaaaaaaaak",
    " kaakkaaaakkaak",
    " kaaawkaakwaaak",
    " kAaaaaaaaaaaAk",
    " kAaaakkkkaaaAk",
    "kkkAaaaaaaaaAkkk",
    "kaakkAaaaaAkkaak",
    "kakakkAAAAkkakak",
    "kaaak kkkk kaaak",
    " kkk        kkk",
]

GEODUDE_2 = [""] + GEODUDE_1[:-1]


def _clean_c(rows):
    assert len(rows) <= 16, len(rows)
    for row in rows:
        assert len(row) <= 16, (len(row), row)
    return [row.replace(".", " ").rstrip() for row in rows]


ZUBAT_1 = _clean_c([
    "................",
    "k..............k",
    "kZk..k....k..kZk",
    "kZzk.kukkuk.kzZk",
    "kZqzkuuuuuukzqZk",
    "kZqqkuuuuuukqqZk",
    "kZqqkUuuuuUkqqZk",
    ".kZqkukkkkukqZk.",
    "..kZkwqqqqwkZk..",
    "...kkUkkkkUkk...",
    "......kUUk......",
    ".....kUkkUk.....",
    "....kUk..kUk....",
    "....kk....kk....",
])


ZUBAT_2 = [""] + ZUBAT_1[:-1]


POLIWAG_1 = _clean_c([
    "................",
    "................",
    "......kkkk......",
    "....kkBBBBkk....",
    "...kBiiBBBBBk...",
    "..kBwwBBBBwwBk..",
    "..kBwkBqqBkwBk..",
    ".kBBBBBBBBBBBBk.",
    ".kBBBwwwwwwBBBk.",
    ".kBBwwkkkkwwBBk.",
    ".kBBwkwwwwkwBBk.",
    ".kBBwkwkkwkwBBk.",
    ".kBBwwkwwkwwBBk.",
    "..kBBwwkkwwBBk..",
    "...kkBBkkBBkk...",
    "....kk....kk....",
])


POLIWAG_2 = [""] + POLIWAG_1[:-1]


STARYU_1 = _clean_c([
    "................",
    ".......kk.......",
    "......kgxk......",
    "......kgxk......",
    ".....kgxxbk.....",
    "kkkkkkgxxbkkkkkk",
    "kggggxxxxxxbbbbk",
    ".kgxxxkkkkxxxbk.",
    "..kgxkynnykxbk..",
    "...kxkynwykxk...",
    "...kgxkyykxbk...",
    "..kgxxxkkxxxbk..",
    "..kgxbk..kgxbk..",
    ".kgxbk....kgxbk.",
    ".kgbk......kgbk.",
    "kkk..........kkk",
])


STARYU_2 = [""] + STARYU_1[:-1]


HORSEA_1 = _clean_c([
    "................",
    "....k.kk........",
    "...kikiik.......",
    "...kiiiiik......",
    "..kBiiikwikkkk..",
    "..kBBiiiiiiiiik.",
    ".kkBBiiiiikkkk..",
    "kikBBiiick......",
    "kiikBBiicck.....",
    "..kkBBiicck.....",
    "...kBBicck......",
    "...kBiickk......",
    "...kBBikk.......",
    ".kk.kBik........",
    ".kikkiik........",
    "..kiikk.........",
])


HORSEA_2 = [""] + HORSEA_1[:-1]


PIKACHU_1 = _clean_c([
    "..k.......k.....",
    "..kk.....kk.....",
    "..kyk...kyk.....",
    "...kyk.kyk......",
    "...kyykyyk....kk",
    "..kyyyyyyyk..kyk",
    ".kyykyyykyyk.kyk",
    ".kyykyyykyykkyk.",
    ".knnyykyynnkkyyk",
    ".kyyyyyyyyykyyk.",
    "..kyyyyyyyyk.kbk",
    ".kykyyyyyykykbk.",
    "..kyyyyyyyykbk..",
    "...kyyyyyyk.k...",
    "...kyk..kyk.....",
    "...kk....kk.....",
])


PIKACHU_2 = [""] + PIKACHU_1[:-1]


VOLTORB_1 = _clean_c([
    "................",
    ".....kkkkkk.....",
    "...kknnnnnnkk...",
    "..knwwnnnnnnnk..",
    ".knwnnnnnnnnnFk.",
    ".knkkknnnnkkkFk.",
    "knnkwwknnkwwknFk",
    "knnkwkknnkkwknFk",
    "knnnkknnnnkknnFk",
    "kkkkkkkkkkkkkkkk",
    "kwwwwwwwwwwwwwsk",
    ".kwwwwwwwwwwwsk.",
    ".kwwwwwwwwwwssk.",
    "..kwwwwwwwwssk..",
    "...kkwwwwsskk...",
    ".....kkkkkk.....",
])


VOLTORB_2 = [""] + VOLTORB_1[:-1]


MAGNEMITE_1 = _clean_c([
    "................",
    ".......kk.......",
    ".......kLk......",
    "......kkkk......",
    "....kkllllkk....",
    "kk.kllllllllk.kk",
    "nLkllwwwwllLkkLu",
    "kLkllwwkkwllLkLk",
    "kLkllwwkkwllLkLk",
    "nLklllwwwlllLkLu",
    "kk.kllllllllk.kk",
    "....kLlllllLk...",
    ".....kkkkkk.....",
    "....kak..kak....",
    "....kk....kk....",
])


MAGNEMITE_2 = [""] + MAGNEMITE_1[:-1]


ROCK = [
    "",
    "  kkkk",
    " ksssskk",
    "ksswsssk",
    "kswssksk",
    "ksssskk",
    " kkkkk",
]

SUPERSONIC = [
    "",
    "  wwww",
    " w    w",
    "w  ww  w",
    "w  ww  w",
    " w    w",
    "  wwww",
]

def _grid_a():
    return [["."] * 32 for _ in range(32)]


def _put_a(grid, x, y, color):
    x, y = round(x), round(y)
    if 0 <= x < len(grid[0]) and 0 <= y < len(grid):
        grid[y][x] = color


def _curve_a(grid, points, radius_from, radius_to, color, samples=48):
    (x0, y0), (x1, y1), (x2, y2) = points
    for t in range(samples + 1):
        u = t / samples
        x = (1 - u) ** 2 * x0 + 2 * (1 - u) * u * x1 + u * u * x2
        y = (1 - u) ** 2 * y0 + 2 * (1 - u) * u * y1 + u * u * y2
        r = radius_from + (radius_to - radius_from) * u
        ellipse(grid, x, y, r, r, color)


def _shade_a(grid, base, shade, dx=1, dy=1):
    size_y, size_x = len(grid), len(grid[0])
    marks = []
    for y in range(size_y):
        for x in range(size_x):
            if grid[y][x] != base:
                continue
            for ox, oy in ((dx, 0), (0, dy)):
                nx, ny = x + ox, y + oy
                if not (0 <= nx < size_x and 0 <= ny < size_y) or grid[ny][nx] not in (base, shade):
                    marks.append((x, y))
                    break
    for x, y in marks:
        grid[y][x] = shade


def _rows_a(grid):
    return ["".join(row) for row in grid]


def mewtwo_frame(step, charging):
    grid = _grid_a()
    lift = step
    _curve_a(grid, ((20, 24), (31, 24), (28, 13)), 2.3, 1.1, "z")
    ellipse(grid, 27.5, 12.5, 1.6, 1.6, "z")
    ellipse(grid, 12, 28.5 - lift, 2.6, 1.8, "h")
    ellipse(grid, 20, 28.5 - (1 - lift), 2.6, 1.8, "h")
    ellipse(grid, 12.3, 24, 3, 3.6, "h")
    ellipse(grid, 19.7, 24, 3, 3.6, "h")
    ellipse(grid, 16, 19.5, 4.6, 4.2, "z")
    ellipse(grid, 16, 13.5, 5.8, 4.2, "h")
    ellipse(grid, 10.2, 12, 2.2, 2, "h")
    ellipse(grid, 21.8, 12, 2.2, 2, "h")
    for side in (-1, 1):
        _curve_a(grid, ((16 + side * 7, 13), (16 + side * 10, 16), (16 + side * 9.5, 20)), 1.3, 1, "h", 20)
        hand_x = 16 + side * 9.5
        ellipse(grid, hand_x, 20.5, 1.4, 1.3, "h")
        for finger in (-1, 0, 1):
            _put_a(grid, hand_x + finger, 22.2, "h")
    _curve_a(grid, ((18.5, 6), (22, 7.5), (21, 11.5)), 0.9, 0.9, "p", 20)
    ellipse(grid, 16, 6, 4.3, 3.8, "h")
    polygon(grid, [(12, 5), (12.4, 0.8), (14.6, 3.6)], "h")
    polygon(grid, [(17.4, 3.6), (19.6, 0.8), (20, 5)], "h")
    _shade_a(grid, "h", "p")
    _shade_a(grid, "z", "Z")
    eye = "w" if charging else "Z"
    for x0, x1 in ((13, 14), (18, 19)):
        grid[6][x0] = grid[6][x1] = eye
        grid[7][x0] = grid[7][x1] = "k"
    grid[5][13] = grid[5][19] = "k"
    grid[5][14] = grid[5][18] = "p"
    grid[9][15] = grid[9][16] = grid[9][17] = "p"
    result = outline(grid)
    if charging:
        glow = [row[:] for row in result]
        for y in range(32):
            for x in range(32):
                if result[y][x] != ".":
                    continue
                if any(0 <= x + ox < 32 and 0 <= y + oy < 32 and result[y + oy][x + ox] == "k"
                       for ox, oy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    glow[y][x] = "d"
        result = glow
    return _rows_a(result)


def onix_frame(step, charging):
    grid = _grid_a()
    jaw = step
    polygon(grid, [(13.5, 9), (16, 0.5), (19, 9)], "a")
    polygon(grid, [(16, 1.5), (17.2, 4), (19, 9), (16.4, 9)], "A")
    ellipse(grid, 16, 25 + jaw, 8.5, 4.2, "a")
    ellipse(grid, 16, 13.5, 10.5, 7.5, "a")
    ellipse(grid, 7.5, 16, 3.5, 3.2, "a")
    ellipse(grid, 24.5, 16, 3.5, 3.2, "a")
    ellipse(grid, 16, 21 + jaw * 0.5, 7, 3 + jaw * 0.5, "k")
    for x in range(10, 23, 3):
        polygon(grid, [(x - 1, 18.2), (x + 1, 18.2), (x, 20.2)], "w")
        polygon(grid, [(x + 0.5, 24 + jaw), (x + 2.5, 24 + jaw), (x + 1.5, 22 + jaw)], "w")
    _shade_a(grid, "a", "A")
    for cx, cy, rx, ry in ((16, 13.5, 10.5, 7.5), (7.5, 16, 3.5, 3.2), (24.5, 16, 3.5, 3.2), (16, 25 + jaw, 8.5, 4.2)):
        for y in range(32):
            for x in range(32):
                if grid[y][x] == "a" and ((x - cx + rx * 0.45) / rx) ** 2 + ((y - cy + ry * 0.45) / ry) ** 2 <= 0.12:
                    grid[y][x] = "s"
    for x in range(8, 25):
        if grid[9][x] == "a" and x % 5 == 1:
            grid[9][x] = grid[10][x] = "A"
    for y, xs in ((12, (6, 26)), (14, (11, 21))):
        for x in xs:
            _put_a(grid, x, y, "A")
    eye = "w" if charging else "k"
    for side in (-1, 1):
        cx = 16 + side * 5
        for i in range(3):
            _put_a(grid, cx + side * (i - 1), 13 + (i if side < 0 else 2 - i) * 0.34, "k")
            _put_a(grid, cx + side * (i - 1), 14, eye if i == 1 else "k")
    return _rows_a(outline(grid))


def _onix_segment_grid_a():
    grid = [["."] * 16 for _ in range(16)]
    ellipse(grid, 7.5, 8, 6.8, 6.3, "a")
    _shade_a(grid, "a", "A")
    for y in range(16):
        for x in range(16):
            if grid[y][x] == "a" and ((x - 5.5) / 3) ** 2 + ((y - 5.5) / 2.6) ** 2 <= 1:
                grid[y][x] = "s"
    for x, y in ((9, 4), (10, 5), (10, 6), (11, 7), (4, 10), (5, 10), (6, 11), (8, 9)):
        grid[y][x] = "A"
    return [row[:] for row in outline(grid)]




def snorlax_frame(step, asleep):
    grid = _grid_a()
    foot = 1 if step else 0
    for side in (-1, 1):
        ellipse(grid, 16 + side * 13, 18, 2.6, 4, "T")
    ellipse(grid, 16, 19, 13.5, 10.5, "T")
    ellipse(grid, 16, 21, 9.5, 8, "c")
    ellipse(grid, 16, 9.5, 9.5, 6.8, "T")
    ellipse(grid, 16, 11, 6.8, 4.6, "c")
    for ear_x in (8, 24):
        polygon(grid, [(ear_x - 2.5, 5.5), (ear_x, 1.2), (ear_x + 2.5, 5.5)], "T")
    ellipse(grid, 8, 28 - foot, 5, 3.4, "c")
    ellipse(grid, 24, 28 - (1 - foot), 5, 3.4, "c")
    _shade_a(grid, "T", "t")
    _shade_a(grid, "c", "C")
    if asleep:
        for x in (11, 12, 13, 19, 20, 21):
            grid[10][x] = "t"
        grid[9][11] = grid[9][13] = grid[9][19] = grid[9][21] = "c"
        grid[11][11] = grid[11][13] = grid[11][19] = grid[11][21] = "c"
        grid[9][10] = grid[9][14] = grid[9][18] = grid[9][22] = "t"
        ellipse(grid, 16, 13.5, 1.6, 1.1, "t")
        grid[13][17] = "w"
    else:
        for x in (11, 12, 13, 19, 20, 21):
            grid[8][x] = "t"
        for x in (12, 13, 19, 20):
            grid[10][x] = "k"
        grid[9][12] = grid[9][19] = "k"
        grid[9][13] = grid[9][20] = "w"
        for x in range(13, 20):
            grid[13][x] = "t"
        grid[12][13] = grid[12][19] = "w"
    for side, offset in ((-1, foot), (1, 1 - foot)):
        cx = 16 + side * 8
        ellipse(grid, cx, 28 - offset, 2, 1.4, "b")
        for dx in (-3, 0, 3):
            _put_a(grid, cx + dx, 25.5 - offset, "b")
    for y in (20, 22):
        _put_a(grid, 2 if y == 20 else 3, y, "w")
        _put_a(grid, 30 if y == 20 else 29, y, "w")
    result = outline(grid)
    if asleep:
        for x, y in ((26, 1), (27, 1), (28, 1), (28, 2), (27, 3), (26, 4), (27, 4), (28, 4)):
            result[y][x] = "w"
        for x, y in ((29, 6), (30, 6), (30, 7), (29, 8), (30, 8)):
            result[y][x] = "w"
    return _rows_a(result)


def venusaur_frame(step, charging):
    grid = _grid_a()
    for side, offset in ((-1, step), (1, 1 - step)):
        ellipse(grid, 16 + side * 8.5, 28 - offset, 3.6, 2.6, "j")
    ellipse(grid, 16, 21.5, 12.5, 7.5, "j")
    for side in (-1, 1):
        polygon(grid, [(16, 13), (16 + side * 15, 10), (16 + side * 12, 16)], "v")
        polygon(grid, [(16, 14), (16 + side * 13, 17), (16 + side * 8, 19)], "v")
    ellipse(grid, 16, 17, 2.4, 3, "b")
    for angle in range(0, 360, 60):
        rad = math.radians(angle + 30)
        ellipse(grid, 16 + math.cos(rad) * 6.5, 9 + math.sin(rad) * 3.6, 4.2, 2.6, "q")
    ellipse(grid, 16, 9, 3.4, 2.2, "w" if charging else "y")
    ellipse(grid, 16, 24, 8.5, 4.5, "j")
    _shade_a(grid, "j", "J")
    _shade_a(grid, "v", "V")
    _shade_a(grid, "q", "n")
    for x, y in ((9, 22), (23, 22), (12, 25), (20, 25), (6, 19), (26, 19)):
        grid[y][x] = grid[y][x + 1] = "J"
    for side in (-1, 1):
        cx = 16 + side * 5
        grid[21][cx] = "e"
        grid[20][cx] = "w" if charging else "e"
        grid[20][cx - side] = "k"
    for x in range(13, 20):
        grid[25][x] = "J"
    for side, offset in ((-1, step), (1, 1 - step)):
        for dx in (-2, 0, 2):
            _put_a(grid, 16 + side * 8.5 + dx, 30 - offset, "w")
    return _rows_a(outline(grid))


def gyarados_frame(step, charging):
    grid = _grid_a()
    sway = step
    _curve_a(grid, ((16, 18), (1, 26 + sway), (16, 28.5)), 3.2, 3, "u")
    _curve_a(grid, ((16, 28.5), (29, 30), (28, 20 - sway)), 3, 1.6, "u")
    polygon(grid, [(26, 21 - sway), (31, 15 - sway), (31, 23 - sway)], "i")
    _curve_a(grid, ((13, 20), (4, 26 + sway), (15, 27)), 1.1, 1.1, "c")
    _curve_a(grid, ((17, 27.5), (26, 28.5), (26, 22 - sway)), 1, 0.8, "c")
    ellipse(grid, 16, 11, 9, 7, "u")
    for x, top in ((9, 2), (12, 0), (16, 0), (20, 0), (23, 2)):
        polygon(grid, [(x - 1.4, 6), (x, top), (x + 1.4, 6)], "i")
    _shade_a(grid, "u", "U")
    ellipse(grid, 16, 14.5, 5.8, 3.4, "B" if charging else "k")
    for x in (12, 14, 18, 20):
        polygon(grid, [(x - 0.8, 11.6), (x + 0.8, 11.6), (x, 13.4)], "w")
    for x in (13, 16, 19):
        polygon(grid, [(x - 0.8, 17.6), (x + 0.8, 17.6), (x, 15.8)], "w")
    for side in (-1, 1):
        cx = 16 + side * 4.5
        _put_a(grid, cx, 8, "e")
        _put_a(grid, cx + side, 8, "e")
        _put_a(grid, cx - side, 7, "k")
        _put_a(grid, cx, 7, "k")
        for i in range(5):
            _put_a(grid, 16 + side * (7 + i), 13 + i * 0.6, "w")
    return _rows_a(outline(grid))


def weezing_frame(step, charging):
    grid = _grid_a()
    bob = step
    ellipse(grid, 23, 7 + bob, 3.6, 3.2, "z")
    ellipse(grid, 22, 20 - bob, 7.2, 6.8, "z")
    ellipse(grid, 11, 13 + bob, 9.2, 8.8, "z")
    for cx, cy in ((6, 5 + bob), (13, 4 + bob), (18, 13 - bob), (26, 14 - bob), (25, 3 + bob)):
        ellipse(grid, cx, cy, 1.8, 1.4, "z")
    _shade_a(grid, "z", "Z")
    for cx, cy in ((6, 5 + bob), (13, 4 + bob), (26, 14 - bob), (25, 3 + bob)):
        _put_a(grid, cx, cy - 0.4, "Z")
    ellipse(grid, 11, 17 + bob, 3.2, 2.4, "c")
    for dx, dy in ((-3, -2), (3, -2), (-3, 2), (3, 2)):
        _put_a(grid, 11 + dx, 17 + bob + dy, "c")
    _put_a(grid, 10, 16.6 + bob, "k")
    _put_a(grid, 12, 16.6 + bob, "k")
    eye = "w" if charging else "k"
    for cx, cy, spread in ((11, 10 + bob, 3), (22, 17 - bob, 2.5)):
        _put_a(grid, cx - spread, cy, eye)
        _put_a(grid, cx + spread, cy, eye)
        _put_a(grid, cx - spread - 1, cy - 1, "k")
        _put_a(grid, cx + spread + 1, cy - 1, "k")
    for x in range(19, 26):
        _put_a(grid, x, 21.5 - bob, "k")
    for x in (20, 22, 24):
        _put_a(grid, x, 22.5 - bob, "w")
    for x in range(7, 16):
        _put_a(grid, x, 21 + bob if x in (7, 15) else 21.5 + bob, "k")
    result = outline(grid)
    for x, y in ((5, 1), (6, 0), (14, 0), (26, 0), (29, 10), (30, 11)):
        if 0 <= y + bob < 32:
            result[y + bob if y + bob < 32 else y][x] = "s"
    return _rows_a(result)


MAGIKARP_1 = [
    "",
    "......k.k.k.....",
    "kk...kckckck....",
    "kck.kcccccckk...",
    "kccknnnnnnnnnk..",
    "kcknnonnonnwwnk.",
    "kknnonnonnwkwnk.",
    " knonnonnonwwnck",
    " knnonnonnnnccck",
    "kknnnnnnnnnnkck.",
    "kcknnnnnnnnnkcky",
    "kccknnnnnncck..y",
    "kck.kcckkcck....",
    "kk...kk..kk.....",
]
MAGIKARP_1 = [row.replace(".", " ") for row in MAGIKARP_1]

MAGIKARP_2 = [""] + MAGIKARP_1[:-1]


BUBBLE = [
    "",
    "  kkkk",
    " kiiiik",
    "kiwwiiik",
    "kiwiiiik",
    "kiiiiiik",
    " kiiiik",
    "  kkkk",
]

WATER_DROP = [
    "",
    "   kk",
    "  kBBk",
    " kBwBBk",
    " kBwBBk",
    " kBBBBk",
    "  kBBk",
    "   kk",
]

STAR = [
    "",
    "   kk",
    "   ky",
    "kkkyykkk",
    " kyywyk",
    "  kyyk",
    " ky  yk",
    " k    k",
]

DRAGON_FIRE = [
    "",
    "  kkk",
    " kqqqk",
    "kqpwpqk",
    "kqppmqk",
    " kqmmqk",
    "  kkkk",
]


SPARK = [
    "",
    "    kk",
    "   kyk",
    "  kyk",
    " kyyyyk",
    "   kyk",
    "  kyk",
    "  kk",
]

BOLT = [
    "  kyyk",
    "  kywk",
    " kywk",
    " kyyyyk",
    "  kwyk",
    "  kyk",
    " kyk",
    " kk",
]

THUNDER_WAVE = [
    "",
    "  yyyy",
    " y    y",
    "y  yy  y",
    "y  yy  y",
    " y    y",
    "  yyyy",
]


MACHOP_TOP = [
    "",
    "     kLkLkLk",
    "     kLLLLLk",
    "    kllllllk",
    "    klekkelk",
    "kk  kllllllk  kk",
    "klk  kllkkk  klk",
    "kllkkllllllkkllk",
    " klLkklllllkkLlk",
    "  kLLlllllllLLk",
    "     kllLLllk",
    "     kLkkkkLk",
    "     kLLkkLLk",
]

MACHOP_1 = MACHOP_TOP + ["    kLLk  kLLk", "    kkkk  kkkk"]
MACHOP_2 = MACHOP_TOP + ["   kLLk    kLLk", "   kkkk    kkkk"]

CHARMANDER_1 = [
    "",
    "        kkk",
    " kk   kkoookk",
    "knnk koooooook",
    "nynk koooookwk",
    "nynkkooooookwok",
    "kynkkoFoooooook",
    " kok kFFooookkk",
    " kokkkoFoooook",
    " koookoooookk",
    "  koooooccoook",
    "   kooFccccokok",
    "    kFFccccokk",
    "    kFFccook",
    "    kFFooook",
    "     kkkkkk",
]


CHARMANDER_2 = [
    "",
    "        kkk",
    " kk   kkoookk",
    "knnk koooooook",
    "nynk koooookwk",
    "nynkkooooookwok",
    "kynkkoFoooooook",
    " kok kFFooookkk",
    " kokkkoFoooook",
    " koookoooookk",
    "  koooooccoook",
    "   kooFccccokok",
    "    kFFccccokk",
    "   kFFocccook",
    "   kFFkoooook",
    "    kk kkkkk",
]


BULBASAUR_1 = [
    "",
    "   kkkkkk",
    "  kvvvVvvk",
    " kvVvVvVvvk",
    " kvvVvvVVvk kk",
    "kvvVvVvvjvvkjjk",
    " kvvvvvvjjjjkk",
    " kvvvvjjjjjjjk",
    "  kvjjjjjjjewjk",
    "  kjjjjjjjjewjk",
    " kjjJjJjjjjjjjk",
    "  kJjJjjjJjkkkk",
    "   kjJJjjjjjjk",
    "    kJJjjjkjjk",
    "    kJwkkkkjwk",
    "     kk    kk",
]


BULBASAUR_2 = [
    "",
    "   kkkkkk",
    "  kvvvVvvk",
    " kvVvVvVvvk",
    " kvvVvvVVvk kk",
    "kvvVvVvvjvvkjjk",
    " kvvvvvvjjjjkk",
    " kvvvvjjjjjjjk",
    "  kvjjjjjjjewjk",
    "  kjjjjjjjjewjk",
    " kjjJjJjjjjjjjk",
    "  kJjJjjjJjkkkk",
    "   kJJjjjjjjjjk",
    "   kJJjjjjkkjjk",
    "   kJwkkkk kjwk",
    "    kk      kk",
]


PONYTA_1 = [
    "         kyk",
    "        kynkk",
    "       kynncck",
    "      kynokkk",
    "     kynokccckk",
    " kk  knokcccwcck",
    "kynkknokkcccUckk",
    "knnoccccccccckk",
    "nnoccccccccck",
    "noccCccccccck",
    "kkkccCccCcck",
    "  kCccccCcck",
    "  kCkckkCkck",
    "  kCkckkCkck",
    "  kAkAkkAkAk",
    "   k k  k k",
]


PONYTA_2 = [
    "         kyk",
    "        kynkk",
    "       kynncck",
    "      kynokkk",
    "     kynokccckk",
    " kk  knokcccwcck",
    "kynkknokkcccUckk",
    "knnoccccccccckk",
    "nnoccccccccck",
    "noccCccccccck",
    "kkkccCccCcck",
    "   kCccccCk",
    "   kCkkkkCk",
    "   kCk  kCk",
    "   kAk  kAk",
    "    k    k",
]


MAGMAR_1 = [
    "     k kk k",
    "    knkyyknk",
    "    knkooonk",
    "     koooook",
    "    kokoookok",
    "    kowoyoook",
    "    kkynknykk",
    "   knoooyooknk",
    "  kokoooooookok",
    "  kokooyyyookok",
    " kokkoyyyyyokkok",
    " kkkkoyyyyykkkkk",
    "  k kkoyyyokk k",
    "    kooooook",
    "    kyokkoyk",
    "     kk  kk",
]


MAGMAR_2 = [
    "     k kk k",
    "    knkyyknk",
    "    knkooonk",
    "     koooook",
    "    kokoookok",
    "    kowoyoook",
    "    kkynknykk",
    "   knoooyooknk",
    "  kokoooooookok",
    "  kokooyyyookok",
    " kokkoyyyyyokkok",
    " kkkkoyyyyykkkkk",
    "  k kkoyyyokk k",
    "   kookoooook",
    "   kyokkkkoyk",
    "    kk    kk",
]


SQUIRTLE_1 = [
    "",
    "       kkkkk",
    "      kiiiiik",
    "     ki1iiiiik",
    "     k1iiiikwk",
    "    ki1iiiikwik",
    "    kkiiiiiiik",
    "   kbbi1iiikkkk",
    "kkkbxxxiiiiikk",
    "11kbxbcccckkk",
    "kiixxccCCCckik",
    "kiibbccccccik",
    " kkbxxcCCCkk",
    "   k11bcciik",
    "   k11kkkiik",
    "    kk   kk",
]


SQUIRTLE_2 = [
    "",
    "       kkkkk",
    "      kiiiiik",
    "     ki1iiiiik",
    "     k1iiiikwk",
    "    ki1iiiikwik",
    "    kkiiiiiiik",
    "   kbbi1iiikkkk",
    "kkkbxxxiiiiikk",
    "11kbxbcccckkk",
    "kiixxccCCCckik",
    "kiibbccccccik",
    " kkbxxcCCCkk",
    "  k11bbcckiik",
    "  k11kkkkkiik",
    "   kk     kk",
]


JYNX_1 = [
    "     kkkkkkk",
    "    kyyyyyyyk",
    "   kyyyyyygyyk",
    "   kygyyyyyyyk",
    "  kyykZZZZZkyyk",
    "  kyyZZwZwZZyyk",
    "  kyyZZZZZZZyyk",
    "  kyyZZZZZZZyyk",
    "   kynZq4qZnyk",
    "  kynnnnZnnnyk",
    "  kyyenyyyennyk",
    "  knnnnnnnnnnyk",
    "  knnnennnnenk",
    " knnnnnnennnnnk",
    " knnnnnnnnnnnnk",
    "  kkkkZZkZZkkk",
]


JYNX_2 = [
    "     kkkkkkk",
    "    kyyyyyyyk",
    "   kyyyyyygyyk",
    "   kygyyyyyyyk",
    "  kyykZZZZZkyyk",
    "  kyyZZwZwZZyyk",
    "  kyyZZZZZZZyyk",
    "  kyyZZZZZZZyyk",
    "   kynZq4qZnyk",
    "  kynnnnZnnnyk",
    "  kyyenyyyennyk",
    "  knnnnnnnnnnyk",
    "  knnnennnnenk",
    " knnnnnnennnnnk",
    " knnnnnnnnnnnnk",
    "  kkkkkZZZkkkk",
]


SHELLDER_1 = [
    "     kkkkkk",
    "   kkzzzzzzk",
    "  kZhzzZzzzZk",
    "  khzzzZzzzZzk",
    " kzZzzzZzzzZzzk",
    " kzzZzzzZzzzZzk",
    "kzzzZzzzZzzzZzk",
    "kzzzZzzzZzzzZzzk",
    "kzzzzzzzkzzzzzzk",
    " kkkkkwssswkkkk",
    "kzzzkkksqskkkzzk",
    " kzzzzkkkqqzzzk",
    " kzzZzzzZzzzZzk",
    "  kzZzzzZzzzZk",
    "   kkkkkkkkkk",
    "",
]


SHELLDER_2 = [
    "   kkzzzzzzk",
    "  kZhzzZzzzZk",
    "  khzzzZzzzZzk",
    " kzZzzzZzzzZzzk",
    " kzzZzzzZzzzZzk",
    "kzzzZzzzZzzzZzk",
    "kzzzZzzzZzzzZzzk",
    "kzzzzzzzzzzzzzzk",
    " kkkkkwssswkkkk",
    " kkkkkkssskkkkk",
    "kzzzzkksqskkzzzk",
    " kzzzzzzzqqzzzk",
    " kzzZzzzZzzzZzk",
    "  kzZzzzZzzzZk",
    "   kkkkkkkkkk",
    "",
]


OMANYTE_1 = [
    "",
    "     kkkkkkk",
    "    kwi11111kk",
    "   kwi111i111ik",
    "   ki11iiiii11k",
    "  kii1ii111ii11k",
    "  kii1ii1i11i11k",
    "  kii1ii111ii11k",
    "  kii1ciiiii11ik",
    "   kwccwci111ik",
    "  kckcckcc11iik",
    "  kccccccciikk",
    "   kccccCCkk",
    "  kckccckck",
    "  kckkcckkck",
    " kck kck  k",
]


OMANYTE_2 = [
    "",
    "     kkkkkkk",
    "    kwi11111kk",
    "   kwi111i111ik",
    "   ki11iiiii11k",
    "  kii1ii111ii11k",
    "  kii1ii1i11i11k",
    "  kii1ii111ii11k",
    "  kii1ciiiii11ik",
    "   kwccwci111ik",
    "  kckcckcc11iik",
    "  kccccccciikk",
    "   kccccCCkk",
    "  kckccckck",
    " kckkcckkck",
    "  k  kck kck",
]


SANDSHREW_TOP = [
    "",
    "",
    "   kk      kk",
    "   kGk    kGk",
    "   kGGkkkkGGk",
    "  kgGGgGGgGGgk",
    "  kggggggggggk",
    "  kgkkggggkkgk",
    "  kggggkkggggk",
    " kGGccccccccGGk",
    "kwkGccccccccGkwk",
    "kwwkcccccccckwwk",
    " kk kGccccGk kk",
    "    kGGGGGGk",
]

SANDSHREW_1 = SANDSHREW_TOP + ["   kGgk  kgGk", "   kkkk  kkkk"]
SANDSHREW_2 = SANDSHREW_TOP + ["  kGgk    kgGk", "  kkkk    kkkk"]

ITEM_BALL = [
    "",
    "",
    "     kkkkkk",
    "   kknnnnnnkk",
    "  knnhnnnnnnnk",
    "  knnnnnnnnnnk",
    " knnnnnnnnnnnnk",
    " kkkkkkwwkkkkkk",
    " kwwwwkwwkwwwwk",
    "  kwwwwkkwwwwk",
    "  kwwwwwwwwwwk",
    "   kkwwwwwwkk",
    "     kkkkkk",
]

JOURNAL_PAGE = [
    "",
    "",
    "   kkkkkkkkk",
    "   kwwwwwwwkk",
    "   kwkkkkwwkwk",
    "   kwwwwwwwkkk",
    "   kwkkkkkkwwk",
    "   kwwwwwwwwwk",
    "   kwkkkkkwwwk",
    "   kwwwwwwwwwk",
    "   kwkkkkkkwwk",
    "   kwwwwwwwwwk",
    "   kkkkkkkkkkk",
]

EMBER = [
    "",
    "   kk",
    "  knyk",
    " knyyk",
    " kyywk",
    "  kyyk",
    "   kk",
]


VULPIX_TOP = [
    "",
    "         kk  kk",
    "        kbFkkFbk",
    " kk kk  kFffFFFk",
    "kffkffkkFfFFFFFk",
    "kfFkfFkkFFFwkFFk",
    " kFFFFkkFFFFFFck",
    "kffkFFkkcFFFFckk",
    "kfFkFFFFkcFFFk",
    " kkFFFFFFkccck",
    "  kFFFFFFFFcck",
    "   kFFFFFFFFk",
    "    kFkkkkFk",
]

VULPIX_1 = VULPIX_TOP + ["    kbk  kbk", "    kk   kk"]
VULPIX_2 = VULPIX_TOP + ["   kbk    kbk", "   kk     kk"]


GROWLITHE_TOP = [
    "",
    "         kk  kk",
    "        kockkcok",
    "  kkk   kccccook",
    " kccck kccoooook",
    " kcccck kooowkok",
    "  kccckkoooookok",
    "   kckoookooocck",
    "   kkoookoocccck",
    "  kookoookoccck",
    "  kookoooooccck",
    "  koooooooocck",
    "   koooooooook",
]

GROWLITHE_1 = GROWLITHE_TOP + ["   kook  kook", "   kkk   kkk"]
GROWLITHE_2 = GROWLITHE_TOP + ["  kook    kook", "  kkk     kkk"]


SEEL_TOP = [
    "",
    "",
    "",
    "",
    "            k",
    "           kwk",
    "         kkkwkk",
    "        kwwwwwwk",
    "        kwwwkwwk",
    "    kkkkwwwwwwwk",
    "  kkwwwwwwwwwwqk",
    "kkwwwwwwwwwwwwqk",
    "kiikwwwwwwwwwwk",
    " kkiiwwwwwwiiik",
]

SEEL_1 = SEEL_TOP + ["    kiik  kiik", "    kkk   kkk"]

SEEL_2 = SEEL_TOP + ["   kiik    kiik", "   kkk     kkk"]

AERODACTYL_1 = [
    "",
    "k      kk      k",
    "kk    kOOk    kk",
    "kPk  kOOOOk  kPk",
    "kPPk kkOOkk kPPk",
    "kPPPkOOOOOOkPPPk",
    " kPPkwkwkwkkPPk",
    " kPPPkOOOOkPPPk",
    "  kPkPkOOkPkPk",
    "  k k kOOk k k",
    "      kOOk",
    "     kOkkOk",
    "     kk  kk",
]

AERODACTYL_2 = [
    "",
    "",
    "",
    "       kk",
    "      kOOk",
    "kkk  kOOOOk  kkk",
    "kPPk kkOOkk kPPk",
    "kPPPkOOOOOOkPPPk",
    " kPPkwkwkwkkPPk",
    "  kPPkOOOOkPPk",
    "   kPkkOOkkPk",
    "    k kOOk k",
    "      kOOk",
    "     kOkkOk",
    "     kk  kk",
]

SLOWPOKE_TOP = [
    "",
    "         kkkk",
    "  kk   kk4444kk",
    " kcck k44444444k",
    " kcqk k44kkk444k",
    "  kqk k44kwk44ck",
    "  kqk k444k44cck",
    "  kqkk44444cccck",
    "   kq4444kkkkkk",
    "   kq444444qk",
    "    k444444qqk",
    "    k4444444qk",
    "     kqqqqqqqk",
]

SLOWPOKE_1 = SLOWPOKE_TOP + ["     kck  kck", "     kkk  kkk"]

SLOWPOKE_2 = SLOWPOKE_TOP + ["    kck    kck", "    kkk    kkk"]

BALLOON = [
    "  kk        kk",
    " kbck kkkk kcbk",
    " kbcckcggckccbk",
    "  kccckGGkccck",
    " kcckkcccckkcck",
    "kkcccwkcckwccckk",
    "k kcccccccccck k",
    "kkkcckkkkkkcckkk",
    "  kccckwwkccck",
    "   kkcccccckk",
    "    k kkkk k",
    "    k      k",
    "   kkkkkkkkkk",
    "   kbxbxbxbxk",
    "   kxbxbxbxbk",
    "    kkkkkkkk",
]

EXEGGCUTE_1 = [
    "      kkk",
    "     k4w4k",
    "    k44444k",
    "    k4k4k4k",
    "   kkk4q44kkk",
    "  k4w4k44k4w4k",
    " k4k444kk44444k",
    " k44k4kkk4k4k4k",
    " k44444kk44q44k",
    "  kkk4kkkk4kkk",
    " k4w4k4w4kk4w4k",
    "k444k44444kk444k",
    "k4k4k4k4k4k4k4kk",
    "k44qk44q44k4444k",
    " k444k444kk444k",
    "  kkk kkk  kkk",
]

EXEGGCUTE_2 = [""] + EXEGGCUTE_1[:-1]


ICE_SHARD = [
    "",
    "   kk",
    "  kwik",
    " kwiik",
    " kiiBk",
    "  kiBk",
    "   kk",
]

HEART = [
    "",
    " kk kk",
    "kqqkqqk",
    "kqwqqqk",
    " kqqqk",
    "  kqk",
    "   k",
]


PIDGEY_TOP = [
    "",
    "       kkk",
    "      kxxCk",
    "     kkxCCkk",
    "    kxxxxxcck",
    "   kxxxxxckwck",
    "   kxbxxxcckkAk",
    "  kxbbxxxccckAAk",
    "  kxbbbxccccckk",
    "  kxbbbcccccck",
    "  kbbbbxccccck",
    " kkkbbbxxccck",
    " kxxkkxxxkkk",
    "  kk kkkk",
]


PIDGEY_1 = PIDGEY_TOP + ["   kqk kqk", "    kk  kk"]


PIDGEY_2 = PIDGEY_TOP + ["  kqk   kqk", "   kk    kk"]


SPEAROW_TOP = [
    "       k k k",
    "      kbkbkbk",
    "     kxbxbxbk",
    "    kxxxxxxxxk",
    "    kxxxxxkkxk",
    "   kxxxxxxkwkCk",
    "   kxxcxxxxkCCCk",
    "   kxqqqqxcckkk",
    "  kqqqnqqxcck",
    "  kqqnnqqccck",
    "  kqnnqqcccck",
    " kkqnqqxccck",
    " kbbkkxxxkk",
    "  kk kkkk",
]


SPEAROW_1 = SPEAROW_TOP + ["   kCk kCk", "    kk  kk"]


SPEAROW_2 = SPEAROW_TOP + ["  kCk   kCk", "   kk    kk"]


KABUTO_1 = [
    "",
    "",
    "     kkkkkk",
    "   kkCCxxxxkk",
    "  kCCxxxxxxxxk",
    " kCxxxxbbxxxxxk",
    " kxxxxbxxbxxxxk",
    "kxxxxbxxxxbxxxxk",
    "kbxxxxxxxxxxxxbk",
    "kbbbbbbbbbbbbbbk",
    " kkkkkkkkkkkkkk",
    " kbkyGkkkkyGkbk",
    "  kkyykkkkyykk",
    "   kkkkkkkkkk",
    "   kbk kk kbk",
    "   kk      kk",
]


KABUTO_2 = [""] + KABUTO_1[:-1]


KOFFING_1 = [
    "",
    "  kk   kk   kk",
    " kzzk kzzk kzzk",
    "  kkkkzzzzkkkk",
    "  kzzzzzzzzzzk",
    " kzhkkzzzzkkzzk",
    " kzzzwkzzzkwzZk",
    "kzzzzkkzzzkkzZZk",
    "kzzzzzzzzzzzzZZk",
    "kzzzzzcccczzzZZk",
    "kzzzzckcckczzZZk",
    " kzzzzcccczzZZk",
    " kzzzczcczczZZk",
    "  kkZZZZZZZZkk",
    "    kkkkkkkk",
]


KOFFING_2 = [""] + KOFFING_1[:-1]


EKANS_TOP = [
    "",
    "     kkkk",
    "    kzzzzk",
    "   kzzwkzzk",
    "   kzzkkzzzk",
    "   kzzzzzzzk",
    "    kknnkzzk",
    "     kyyyykk",
    "      kzzk",
    "     kzzk    kk",
    "    kzZk   kkGk",
    "   kzzZk  kzZkk",
    "   kzyyzkkzzZk",
    "  kzzzzzzzzzZk",
]


EKANS_1 = EKANS_TOP + ["  kZzyyzzyyzZk", "   kkkkkkkkkk"]


EKANS_2 = EKANS_TOP + [" kZzyyzzyyzzZk", "  kkkkkkkkkkk"]


GRIMER_TOP = [
    "",
    "",
    "      kkkk",
    "     kddddk",
    "    kdhddddk",
    "   kdhddddddk",
    "  kddwkddwkddk",
    "  kddkkddkkddk",
    " kdddddddddddRk",
    " kddkkkkkkkkdRk",
    " kddkqqqqqqkdRk",
    "kdddkkkkkkkkdRRk",
    "kdddddddddddRRk",
]


GRIMER_1 = GRIMER_TOP + ["kddRddddddRdRRdk", "kRRRRRRRRRRRRRRk", " kkkkkkkkkkkkkk"]


GRIMER_2 = GRIMER_TOP + ["kdRddddRddddRRdk", "kRRRRRRRRRRRRRRk", " kkkkkkkkkkkkkk"]


FEATHER = [
    "",
    "     kk",
    "    kwk",
    "   kwsk",
    "  kwsk",
    " kwsk",
    " kkk",
]


SILPH_SCOPE = [
    "",
    "",
    "",
    "  kkkkkkkkkkkk",
    " kssssssssssssk",
    " kskkkssssskkksk",
    " kskBBksssskBBksk",
    " kskBwksssskBwksk",
    " kskkkssssskkksk",
    " kssssssssssssk",
    "  kkkkkkkkkkkk",
]

SILPH_SCOPE = [row[:16] for row in SILPH_SCOPE]

SLUDGE = [
    "",
    "  kkkk",
    " kmmmmk",
    "kmpmmmmk",
    "kmmmmqmk",
    "kmmmmmmk",
    " kmmmmk",
    "  kkkk",
]


MANKEY_TOP = [
    "",
    " kkk k kk k kkk",
    " kCckwkwwkwkcCk",
    "  kckwwwwwwwkck",
    "  kwwwwwwwwwwk",
    "  kwkkwwwwkkwk",
    "  kwwckwwkcwwk",
    "  kwwwcCCcwwwk",
    "  kwwwCkkCwwwk",
    "  kcwwcCCcwwck",
    " kbkcwwwwwwckbk",
    "kbbkccwwwwcckbbk",
    " kk kCccccCk kk",
    "    kkCCCCkk",
]

MANKEY_1 = [row[:16] for row in MANKEY_TOP + ["    kbbk  kbbk", "    kkk   kkk"]]
MANKEY_2 = [row[:16] for row in MANKEY_TOP + ["   kbbk    kbbk", "   kkk     kkk"]]

MACHOKE_TOP = [
    "",
    "       kk",
    "      kPPk",
    "     kOOOOk",
    "    kOekkeOk",
    "kkk kOOOOOOk kkk",
    "kOOk kOkkOk kOOk",
    "kOPOkkOOOOkkOPOk",
    " kPOOOOOOOOOOPk",
    "  kkPOOOOOOPkk",
    "    kOOPPOOk",
    "    kkkggkkk",
    "    kkkkkkkk",
    "    kOPkkPOk",
]

MACHOKE_1 = MACHOKE_TOP + ["   kOPk  kPOk", "   kkkk  kkkk"]
MACHOKE_2 = MACHOKE_TOP + ["  kOPk    kPOk", "  kkkk    kkkk"]

FARFETCHD_TOP = [
    "",
    " kk",
    "kvVk     kkkk",
    "kvVk    kbbbbk",
    " kvk   kbbbbbbk",
    " kvVk  kbkkbbbkk",
    " kvVk  kbwkbbkyk",
    "  kvk kbbbbbkkyk",
    "  kvVkkbccbbbkk",
    "   kwkbccccbbbk",
    "   kwkbcccccbbbk",
    "    kkbcccccbbbk",
    "     kbbcccbbbk",
    "      kkbbbbkk",
]


FARFETCHD_1 = FARFETCHD_TOP + ["      kgk kgk", "      kk  kk"]


FARFETCHD_2 = FARFETCHD_TOP + ["     kgk   kgk", "     kk    kk"]


GASTLY_1 = [
    "",
    "   z  zzz   z",
    "  zpz zpz  zpz",
    " zpzzzkkkkzzzpz",
    "  zzkkkkkkkkzz",
    " zpkkwwkkkwwkkz",
    " zpkwwkkkkkwwkpz",
    "zpzkkkkkkkkkkzpz",
    " zpkkwnnnnnwkkpz",
    " zpkkkwnnnwkkkpz",
    "  zzkkkkkkkkkzz",
    " zpzzkkkkkkkzzpz",
    "  zpz zzzzz zpz",
    "   z        z",
]


GASTLY_2 = [""] + GASTLY_1[:-1]


HAUNTER_1 = [
    "",
    "   k        k",
    "   kk kkkk kk",
    "   kzkzzzzkzk",
    "  kzzzzzzzzzzk",
    "  kzwwwzzwwwzk",
    "  kzwkwzzwkwzk",
    "  kzzzzzzzzzzk",
    "  kzkknnnnkkzk",
    "   kznwnnwnzk",
    "k k kznnnnzk k k",
    "kZk  kzzzzk  kZk",
    "kZZk  kzzk  kZZk",
    " kk    kk    kk",
]


HAUNTER_2 = [""] + HAUNTER_1[:-1]


CUBONE_TOP = [
    "",
    "  kk      kk",
    "  kwk    kwk",
    "  kwwkkkkwwk",
    " kwwwwwwwwwwk",
    " kwkkwwwwkkwk",
    " kwkkwwwwkkwk",
    " kswwwwwwwwsk kk",
    "  ksbbbbbbsk kwk",
    "   kbbccbbk kwk",
    "  kbbccccbbkwk",
    "  kbkccccbkwk",
    "  kbbccccbwk",
    "   kbbbbbwwk",
]


CUBONE_1 = CUBONE_TOP + ["   kbbk kbbk", "   kkk  kkk"]


CUBONE_2 = CUBONE_TOP + ["  kbbk   kbbk", "  kkk    kkk"]


ABRA_TOP = [
    "",
    "  kk        kk",
    "  kyk      kyk",
    "  kyykkkkkkyyk",
    "   kyyyyyyyyk",
    "  kyyyyyyyyyyk",
    "  kykkyyyykkyk",
    "  kyyyyGGyyyyk",
    "   kyyGyyGyyk",
    "  kbbkyyyykbbk",
    " kbxbbkyykbbxbk",
    " kbbbbkkkkbbbbk",
    "  kkkyyyyyykkk",
    "    kyyyyyyk",
]


ABRA_1 = ABRA_TOP + ["   kyyk  kyyk", "   kkkk  kkkk"]


ABRA_2 = ABRA_TOP + ["  kyyk    kyyk", "  kkkk    kkkk"]


VENOMOTH_1 = [
    "",
    "    k      k",
    "     k    k",
    " kk   kkkk   kk",
    "kzzk kzzzzk kzzk",
    "kzhzkkz11zkkzhzk",
    "kzzhzkzzzzkzhzzk",
    " kzzzzkzzkzzzzk",
    "  kzhzkzzkzhzk",
    " kzzzzkZZkzzzzk",
    "kzhzk kZk  kzhzk",
    "kzzzk  kZk  kzzk",
    " kkk   kZk   kk",
    "       kk",
]


VENOMOTH_2 = [
    "",
    "",
    "    k      k",
    "     k    k",
    "      kkkk",
    " kkk kzzzzk kkk",
    "kzzzkkz11zkkzzzk",
    "kzhzzkzzzzkzzhzk",
    "kzzhzzkzzkzzhzzk",
    " kzzzzkzzkzzzzk",
    "  kzhzkZZkzhzk",
    " kzzzk kZk kzzk",
    " kkk   kZk   kk",
    "       kk",
]


SHADOW_BALL = [
    "",
    "  kkkk",
    " kmmmmk",
    "kmppmmmk",
    "kmpmmmmk",
    "kmmmmmmk",
    " kmmmmk",
    "  kkkk",
]

BONE = [
    "",
    " kk  kk",
    "kcckkcck",
    " kccccck",
    "  kccck",
    " kcccck",
    "kcckkcck",
    " kk  kk",
]


DRATINI_1 = [
    "",
    "          kkkk",
    "    kkk  kiiiik",
    "   kwwwkkiiiiiik",
    "    kkwkiiwkiiik",
    "      kkiiiiiiik",
    "       kiiiiikk",
    "      kiiwwkk",
    "     kiiwwk",
    "    kiiwwk",
    "   kiiwwk     kk",
    "   kiiwwk   kkik",
    "   kiiiwwkkkiiik",
    "    kiiiiiiiiikk",
    "     kkkkkkkkk",
]

DRATINI_2 = [""] + DRATINI_1[:-1]

DRAGONAIR_1 = [
    "",
    "           k",
    "     kk   kwk",
    "    kwwk kuwuk",
    "     kwwkuuuuuk",
    "      kkuwkuuuk",
    "       kuuuuuuk",
    "      kiikuukk",
    "      kuuwwk",
    "     kuuwwk",
    "    kuuwwk    kk",
    "   kuuwwk   kkik",
    "   kuuuwwkkkuuik",
    "    kuuuuuuuuukk",
    "     kkkkkkkkk",
]

DRAGONAIR_2 = [""] + DRAGONAIR_1[:-1]

SEADRA_1 = [
    "",
    "    k k k",
    "   kBkBkBk",
    "   kBBBBBBkkkk",
    "   kBwkBBBBBBBk",
    "   kBBBBBBkkkk",
    " kk  kBBBck",
    "kBBk kBBcck",
    " kBBkkBBcck",
    "kBBBBkBBBck",
    " kBBkkBBcck",
    "  kk kBBBck",
    "      kBBck",
    "   kBk kBk",
    "   kBkkBk",
    "    kkkk",
]

SEADRA_2 = [""] + SEADRA_1[:-1]

LAPRAS_TOP = [
    "",
    "           k",
    "          kik",
    "         kkiikk",
    "        kiiiiiik",
    "        kiwkiiik",
    "         kiiicck",
    "     kk   kiikk",
    "   kkAAkk kiik",
    "  kssAssskkiik",
    " ksAsssAssiiik",
    " kssssssssiik",
    "kiiiiiiiiiiiik",
    "kiicccccccciik",
]

LAPRAS_1 = LAPRAS_TOP + [" kiik     kiik", "  kk       kk"]

LAPRAS_2 = LAPRAS_TOP + ["kiik       kiik", " kk         kk"]


KADABRA_TOP = [
    "",
    " kk      kk",
    " kyk    kyk   kk",
    " kyykkkkyyk  ksk",
    "  kyynnyyk   ksk",
    "  kykyykyk    kk",
    "  kyyyyyyk   ksk",
    " kbkyyyykbk  ksk",
    "kbkkbbbbkkbk kyk",
    "kk kbbyybbkkkyyk",
    "   kbyyyybkkkk",
    "   kyyyyyyk",
    "    kyyyyk",
]

KADABRA_1 = KADABRA_TOP + ["   kyk  kyk", "   kkk  kkk"]
KADABRA_2 = KADABRA_TOP + ["  kyk    kyk", "  kkk    kkk"]

DROWZEE_TOP = [
    "",
    "",
    "     kkkkkk",
    "   kkyyyyyykk",
    "  kyyyyyyyyyyk",
    "  kyyyyyyyyyyk",
    "  kykkkyykkkyk",
    "  kgyyyggyyygk",
    "  kxxxxggxxxxk",
    " kxkxxxyyxxxkxk",
    "kxxkxxxyyxxxkxxk",
    " kk kxxGGxxk kk",
    "    kbxxxxbk",
]

DROWZEE_1 = DROWZEE_TOP + ["   kbxk  kxbk", "   kkkk  kkkk"]
DROWZEE_2 = DROWZEE_TOP + ["  kbxk    kxbk", "  kkkk    kkkk"]


def moltres_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    lift = 2 * step
    for side in (1, -1):
        for index, (tx, ty) in enumerate(((31, 2 - lift), (31, 8 - lift), (28, 14 - lift), (23, 18))):
            base = [(16 + side * 3, 9 + index * 2), (16 + side * 3, 14 + index * 2)]
            polygon(grid, [base[0], (16 + side * (tx - 16), ty), base[1]], "o")
        for flame in range(4):
            x = 16 + side * (7 + flame * 2.6)
            y = 7 + flame * -0.9 - lift + (flame % 2)
            polygon(grid, [(x - 1.5, y + 4), (x + 1.5, y + 4), (x + side * 0.5, y - 2)], "n")
            polygon(grid, [(x - 0.7, y + 4), (x + 0.7, y + 4), (x + side * 0.3, y)], "y")
    ellipse(grid, 16, 17, 5, 6.5, "y")
    ellipse(grid, 16, 18, 3, 4.5, "g")
    ellipse(grid, 16, 9, 3.5, 3.5, "y")
    for tip, height in ((12, 2), (14, 0), (16, 1), (18, 0), (20, 2)):
        polygon(grid, [(tip - 1.5, 7), (tip + 1.5, 7), (tip, height)], "n")
        polygon(grid, [(tip - 0.6, 7), (tip + 0.6, 7), (tip, height + 3)], "o")
    polygon(grid, [(15, 10), (18, 10), (16.5, 14)], "o")
    for x in (14, 18):
        grid[8][x] = "w" if charging else "k"
    for x in (14, 18):
        for y in range(23, 27):
            grid[y][x] = "s"
    for tip in (12, 16, 20):
        polygon(grid, [(tip - 2, 23), (tip + 2, 23), (tip, 31)], "n")
        polygon(grid, [(tip - 1, 23), (tip + 1, 23), (tip, 28)], "y")
    return ["".join(row) for row in outline(grid)]

def gengar_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    for tip_x, tip_y in ((6, 1), (26, 1)):
        polygon(grid, [(tip_x - 3 if tip_x < 16 else tip_x - 5, 11), (tip_x + 5 if tip_x < 16 else tip_x + 3, 9),
                       (tip_x, tip_y + step)], "z")
    for index, x in enumerate((10, 13, 16, 19, 22)):
        polygon(grid, [(x - 2.5, 9), (x + 2.5, 9), (x + (index - 2) * 0.8, 3 + abs(index - 2) + step)], "z")
    ellipse(grid, 16, 17 + step, 12, 10, "z")
    ellipse(grid, 16, 21 + step, 10, 5, "Z")
    ellipse(grid, 16, 18 + step, 11, 6, "z")
    for side in (-1, 1):
        polygon(grid, [(16 + side * 11, 17 + step), (16 + side * 11, 22 + step), (16 + side * 15, 20 + step)], "z")
        polygon(grid, [(16 + side * 5, 25), (16 + side * 9, 25), (16 + side * 8, 30)], "Z")
        polygon(grid, [(16 + side * 3, 11 + step), (16 + side * 8, 10 + step), (16 + side * 7, 15 + step),
                       (16 + side * 4, 14 + step)], "w" if charging else "n")
        grid[12 + step][16 + side * 6] = "k"
    for x in range(8, 25):
        depth = round(3.5 * math.sin((x - 7.5) / 17 * math.pi))
        for y in range(18 + step, 18 + step + depth + 1):
            grid[y][x] = "w"
        grid[18 + step + depth + 1][x] = "k"
    for x in range(9, 24, 3):
        grid[19 + step][x] = "k"
    return ["".join(row) for row in outline(grid)]

def dragonite_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    lift = 2 * step
    for side in (-1, 1):
        polygon(grid, [(16 + side * 7, 13), (16 + side * 13, 8 - lift), (16 + side * 14, 13 - lift),
                       (16 + side * 12, 14), (16 + side * 13, 17), (16 + side * 8, 17)], "j")
        polygon(grid, [(16 + side * 8, 15), (16 + side * 13, 11 - lift), (16 + side * 14, 13 - lift),
                       (16 + side * 12, 14), (16 + side * 13, 17), (16 + side * 8, 17)], "J")
    polygon(grid, [(20, 25), (30, 28), (31, 31), (20, 30)], "o")
    ellipse(grid, 16, 20, 8.5, 9, "o")
    ellipse(grid, 16, 22, 5.5, 6.5, "c")
    for y in (19, 22, 25):
        for x in range(12, 21):
            if grid[y][x] == "c":
                grid[y][x] = "C"
    ellipse(grid, 16, 7, 6, 5, "o")
    ellipse(grid, 16, 9, 4, 2.5, "c")
    for side in (-1, 1):
        for i, (dx, dy) in enumerate(((1, 2), (2, 1), (3, 0), (4, 0), (5, 1))):
            grid[dy][16 + side * dx] = "o"
        polygon(grid, [(16 + side * 8, 17), (16 + side * 11, 18), (16 + side * 9, 21)], "o")
        for x in (16 + side * 4, 16 + side * 5):
            for y in range(28, 31):
                grid[y][x] = "o"
    for x in (13, 19):
        grid[6][x] = "w" if charging else "k"
        grid[5][x] = "w" if charging else "k"
    for x in range(14, 19):
        grid[10][x] = "k"
    grid[9][13] = grid[9][19] = "k"
    return ["".join(row) for row in outline(grid)]

def hitmon_frame(step, charging, kicker):
    grid = [["."] * 32 for _ in range(32)]
    if kicker:
        ellipse(grid, 16, 10, 7, 7.5, "b")
        ellipse(grid, 16, 17, 7.5, 5, "c")
        ellipse(grid, 16, 14.5, 7, 2.5, "b")
        for side in (-1, 1):
            for i in range(7):
                x = 16 + side * (7 + i // 2)
                grid[9 + i][x] = grid[9 + i][x + side] = "b"
            for x in range(16 + side * 9, 16 + side * 12, side):
                grid[15][x] = grid[16][x] = "c"
        kick = 1 if step else -1
        for i in range(10):
            grid[21 + i][15 - i // 3] = grid[21 + i][14 - i // 3] = "b"
        for i in range(10):
            if step:
                x, y = 17 + i, 21 + i // 3
            else:
                x, y = 17 + i // 3, 21 + i
            grid[y][x] = "b"
            grid[min(31, y + 1)][x] = "b"
        grid[30][10] = grid[30][11] = grid[30][12] = "c"
        if step:
            for y in (23, 24, 25):
                grid[y][27] = grid[y][28] = "c"
        else:
            for x in (19, 20, 21):
                grid[30][x] = "c"
        for x in (13, 19):
            grid[7][x] = "w" if charging else "k"
            grid[8][x] = "w" if charging else "k"
    else:
        ellipse(grid, 16, 16, 7, 7, "z")
        ellipse(grid, 16, 16, 4, 5, "Z")
        ellipse(grid, 16, 6, 5, 4.5, "x")
        polygon(grid, [(11, 5), (21, 5), (16, -1)], "b")
        for side in (-1, 1):
            polygon(grid, [(16 + side * 5, 11), (16 + side * 8, 12), (16 + side * 9, 16), (16 + side * 6, 15)], "x")
            ellipse(grid, 16 + side * (11 + step), 15, 3.8, 3.8, "n")
            grid[13][16 + side * (11 + step) - side] = "w"
            for y in range(22, 30):
                grid[y][16 + side * 3] = grid[y][16 + side * 4] = "x"
            for x in range(16 + side * 2, 16 + side * 6, side):
                grid[30][x] = "b"
        for x in range(10, 23):
            grid[22][x] = "Z"
        for x in (14, 18):
            grid[6][x] = "w" if charging else "k"
        for x in range(14, 19):
            grid[9][x] = "k"
    return ["".join(row) for row in outline(grid)]


def arbok_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    for i in range(10):
        cx = 16 + (6 if (i // 2 + step) % 2 else -6) * (1 if i > 4 else 0)
        ellipse(grid, cx, 30 - i * 1.2, 5, 2.5, "z")
    ellipse(grid, 16, 15, 12, 9, "Z")
    ellipse(grid, 16, 15, 11, 8, "z")
    for cx in (10, 22):
        ellipse(grid, cx, 13, 3.5, 3, "y")
        ellipse(grid, cx, 13, 2.2, 1.8, "n")
        grid[13][cx] = "k"
    for x in range(9, 24):
        y = 19 - round(2.5 * math.cos((x - 16) / 7 * math.pi / 2))
        grid[y][x] = "y"
        grid[y + 1][x] = "k"
    ellipse(grid, 16, 5, 5, 3.5, "z")
    grid[4][14] = grid[4][18] = "w" if charging else "e"
    for x in range(14, 19):
        grid[7][x] = "k"
    grid[8][16] = "n"
    return ["".join(row) for row in outline(grid)]


def pidgeot_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    lift = 2 * step
    wing = [(12, 11), (20, 9 - lift), (31, 5 - lift), (31, 9 - lift), (29, 13 - lift), (30, 15 - lift),
            (26, 16 - lift), (26, 18), (22, 18), (20, 20)]
    for side in (1, -1):
        pts = wing if side == 1 else mirrored(wing)
        polygon(grid, pts, "x")
        for i in range(12):
            x = 16 + side * (4 + i)
            column = [y for y in range(32) if grid[y][x] == "x"]
            if column:
                for y in column[:2]:
                    grid[y][x] = "b"
                if i > 7:
                    for y in column[2:]:
                        grid[y][x] = "b"
    for index, color in enumerate("nynyn"):
        angle = math.radians(50 + index * 20)
        for r in range(9):
            x = 16 + math.cos(angle) * r * 1.3
            y = 21 + math.sin(angle) * r * 1.1
            if 0 <= round(y) < 32:
                ellipse(grid, x, y, 1.2, 1.2, color)
    ellipse(grid, 16, 17, 5, 7, "c")
    ellipse(grid, 16, 9, 4, 3.5, "x")
    for i in range(10):
        x = 16 - i * 0.9
        y = 5 - i * 0.5 + (i * i) * 0.02
        grid[max(0, round(y))][round(x)] = "n"
        grid[max(0, round(y) + 1)][round(x) + 1] = "y"
    grid[9][14] = grid[9][18] = "w" if charging else "k"
    grid[9][13] = grid[9][19] = "k"
    grid[11][16] = grid[12][16] = "q"
    grid[11][15] = "q"
    return ["".join(row) for row in outline(grid)]


def articuno_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    lift = 2 * step
    wing = [(12, 10), (20, 9 - lift), (31, 3 - lift), (31, 7 - lift), (28, 11 - lift), (29, 13 - lift),
            (25, 14 - lift), (25, 16), (21, 16), (20, 18)]
    for side in (1, -1):
        pts = wing if side == 1 else mirrored(wing)
        polygon(grid, pts, "i")
        for i in range(12):
            x = 16 + side * (4 + i)
            top = next((y for y in range(32) if grid[y][x] == "i"), None)
            if top is not None:
                grid[top][x] = "w"
                if i > 8:
                    for y in range(top + 1, 32):
                        if grid[y][x] == "i":
                            grid[y][x] = "B"
    for t in range(40):
        y = 18 + t * 0.33
        x = 16 + 4 * math.sin(t / 6)
        width = 2.6 - t * 0.04
        ellipse(grid, x, y, width, 1.2, "B")
        if width > 1.4:
            grid[round(y)][round(x)] = "u"
    ellipse(grid, 16, 15, 4.5, 6, "i")
    ellipse(grid, 16, 15, 2.5, 4, "w")
    ellipse(grid, 16, 8, 3.5, 3.2, "i")
    for x, y in ((15, 3), (15, 2), (16, 1), (16, 0), (17, 2), (18, 3), (14, 4), (16, 4), (16, 3), (17, 4), (15, 5), (16, 5)):
        grid[y][x] = "B"
    for x in (14, 18):
        grid[8][x] = "w" if charging else "e"
    grid[10][16] = grid[11][16] = "s"
    return ["".join(row) for row in outline(grid)]


def zapdos_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    lift = 2 * step
    tips = [(31, 1 - lift), (31, 7 - lift), (29, 13 - lift), (25, 17 - lift), (21, 20)]
    for side in (1, -1):
        for index, (tx, ty) in enumerate(tips):
            base = [(16 + side * 3, 8 + index * 2), (16 + side * 3, 13 + index * 2)]
            polygon(grid, [base[0], (16 + side * (tx - 16), ty), base[1]], "y" if index % 2 == 0 else "g")
    ellipse(grid, 16, 17, 5, 6.5, "y")
    for tip in (13, 16, 19):
        polygon(grid, [(tip - 2, 17), (tip + 2, 17), (tip, 22)], "g")
    ellipse(grid, 16, 9, 4, 3.5, "y")
    for tip, height in ((12, 3), (14, 1), (16, 0), (18, 1), (20, 3)):
        polygon(grid, [(tip - 1, 8), (tip + 1.5, 8), (tip + 0.5, height)], "y")
    polygon(grid, [(14.5, 10), (18.5, 10), (16.5, 15)], "o")
    for x, y in ((14, 8), (18, 8)):
        grid[y][x] = "w" if charging else "k"
    grid[7][13] = grid[7][19] = "k"
    for tip in (13, 19):
        polygon(grid, [(tip - 2, 22), (tip + 3, 22), (tip + 0.5, 30)], "y")
    for x in (14, 18):
        for y in range(23, 28):
            grid[y][x] = "o"
        for dx in (-1, 0, 1):
            grid[28][x + dx] = "o"
    return ["".join(row) for row in outline(grid)]


def mart_frame():
    grid = [["."] * 32 for _ in range(32)]
    ellipse(grid, 16, 15, 6.5, 4, "u")
    for y in range(12, 19):
        for x in range(10, 23):
            if grid[y][x] == "u" and y % 2 == 0:
                grid[y][x] = "w"
    ellipse(grid, 16, 7.5, 4.2, 4.2, "c")
    for x in range(12, 21):
        for y in range(2, 6):
            if ((x - 16) / 4.6) ** 2 + ((y - 6) / 4.2) ** 2 <= 1:
                grid[y][x] = "u"
    for x in range(10, 16):
        grid[6][x] = "U"
    grid[8][14] = grid[8][18] = "k"
    for y in range(17, 30):
        for x in range(1, 31):
            grid[y][x] = "w" if y < 19 else "U" if y > 27 else "B"
    ellipse(grid, 16, 23.5, 3.5, 3.5, "k")
    ellipse(grid, 16, 23.5, 2.6, 2.6, "w")
    for y in range(20, 24):
        for x in range(13, 20):
            if grid[y][x] == "w" and ((x - 16) / 2.6) ** 2 + ((y - 23.5) / 2.6) ** 2 <= 1:
                grid[y][x] = "n"
    for x in range(13, 20):
        grid[23][x] = "k"
    grid[23][16] = "w"
    return ["".join(row) for row in outline(grid)]


ITEM_ICONS = [
    ["..kkkk..", ".kAAAAk.", "kAAoAAAk", "kAoooAAk", "kAAoAAAk", ".kAAAAk.", "..kkkk..", "........"],
    ["...kk...", "..kvvk..", ".kvVvvk.", ".kvvvVk.", ".kVvvvk.", "..kvVk..", "...kk...", "........"],
    ["...k....", "..kwk...", ".kwBwk..", "kwBBBwk.", "kBBwBBk.", "kBBBBBk.", ".kBBBk..", "..kkk..."],
    ["ss....ss", "nn....uu", "nn....uu", "nn....uu", "nnn..uuu", ".nnnuuu.", "..nnuu..", "........"],
    ["........", "..kkkk..", ".kssAAk.", "kssAAAAk", "ksAAAAAk", "kAAAAAAk", ".kkkkkk.", "........"],
    ["........", "........", "...kk...", "..kCCk..", ".kCcCCk.", "kCcCCcCk", "kkkkkkkk", "........"],
    ["...kk...", "..kwwk..", "..kssk..", ".kswwsk.", "kswwwwsk", "kswwwssk", ".kssssk.", "..kkkk.."],
    ["........", "kkkkkkkk", "kAAgAAAk", "kkkkkkkk", "..kAk...", "..kAk...", "..kkk...", "........"],
    ["...Vk...", "..knnk..", ".knnnnk.", ".kcccck.", "..kcck..", ".knnnnk.", "..kkkk..", "........"],
    ["......kk", ".....kwk", "....kwk.", "...kwk..", "..kwk...", ".kCCk...", "kCCk....", "kkk....."],
    ["..kkk...", "..kwk...", ".kzzzk..", "kzwzzzk.", "kzzzzZk.", "kzzzzZk.", ".kkkkk..", "........"],
    ["........", "kk....kk", "kukkkkuk", "kuuwwuuk", "kukkkkuk", "kk....kk", "........", "........"],
    ["...ss...", "..kssk..", "..kkkk..", ".kzzzzk.", ".kzwzzk.", ".kzzzZk.", ".kzzzZk.", "..kkkk.."],
]


def light_circle():
    size = 64
    grid = [["."] * size for _ in range(size)]
    ellipse(grid, 31.5, 31.5, 31.5, 31.5, "w")
    return ["".join(row) for row in grid]


def cloud_frame(fill, shade):
    grid = [["."] * 16 for _ in range(16)]
    for cx, cy, r in ((6, 8, 4), (10, 7, 4), (8, 10, 4), (5, 11, 3), (11, 11, 3)):
        ellipse(grid, cx, cy, r, r * 0.8, fill)
    for cx, cy in ((6, 7), (10, 9), (8, 12)):
        grid[cy][cx] = shade
    return ["".join(row) for row in grid]


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


def polygon(grid, points, color):
    size = len(grid)
    for y in range(size):
        for x in range(len(grid[0])):
            px, py = x + 0.5, y + 0.5
            inside = False
            for (x1, y1), (x2, y2) in zip(points, points[1:] + points[:1]):
                if (y1 > py) != (y2 > py) and px < x1 + (py - y1) * (x2 - x1) / (y2 - y1):
                    inside = not inside
            if inside:
                grid[y][x] = color


def mirrored(points):
    return [(32 - x, y) for x, y in points]


MEW = [
    "..........................",
    "..........kkk.............",
    ".......kkk777kk...........",
    ".....kk777kkk77k..........",
    "....k777kk...kk7k.........",
    "...k8778k.....k7k.........",
    "...k888k....kk7k..........",
    "....kkk...kk77k...........",
    ".........k77kk............",
    "........k8kk........kkk...",
    "........k8k...kkkkkk77k...",
    ".........k8kkkk87777k7k...",
    ".........k877988877777k...",
    ".........k887787997779wk..",
    ".........k8877879w77790k..",
    "........k88877879077791k..",
    ".......k88k8kkk79177777k..",
    "......k8kkkk.kkk977777kk..",
    "......kkk......k977888k...",
    "................kkkkkk....",
]


def mew_frame(step):
    rows = ["." * 32] * (6 + step) + ["..." + row + "..." for row in MEW]
    return (rows + ["." * 32] * 32)[:32]


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
    return palette


SHINY_HUE_SHIFT = {
    "rattata": 150, "gyarados": 140, "magikarp": 40, "pikachu": -15, "dratini": 110, "dragonair": 120, "charmander": 20, "gengar": -40,
    "haunter": -40, "gastly": -40, "snorlax": 40, "voltorb": 200, "geodude": 30,
    "venusaur": 60, "bulbasaur": 60, "squirtle": 40, "vulpix": 30, "growlithe": 20, "lapras": 90,
    "machop": 60, "machoke": 60, "abra": -20, "kadabra": -20, "seel": 40, "slowpoke": 60, "arbok": 60,
}


SHINY_COLORS = {
    "mew": {"7": (192, 224, 248), "8": (128, 176, 232), "9": (64, 104, 176)},
    "ditto": {"m": (40, 64, 136), "d": (80, 120, 200), "p": (120, 168, 232), "h": (200, 224, 248)},
    "onix": {"a": (216, 184, 72), "A": (160, 128, 40), "s": (240, 216, 120)},
    "zubat": {"u": (128, 192, 96), "U": (64, 128, 64), "z": (184, 216, 136), "Z": (96, 152, 80)},
    "dragonite": {"o": (152, 176, 80)},
    "mewtwo": {"h": (232, 236, 228), "p": (200, 208, 200), "z": (120, 192, 96), "Z": (72, 144, 64)},
}


def shiny_color(color, degrees):
    import colorsys
    red, green, blue = (value / 255 for value in color)
    hue, saturation, value = colorsys.rgb_to_hsv(red, green, blue)
    if saturation < 0.18 or color in (COLORS["m"], TRANSPARENT):
        return color
    red, green, blue = colorsys.hsv_to_rgb((hue + degrees / 360) % 1, saturation, value)
    return (round(red * 255), round(green * 255), round(blue * 255))


def save_shiny_palette(name, palette):
    if name in SHINY_COLORS:
        swaps = {COLORS[letter]: color for letter, color in SHINY_COLORS[name].items()}
        shiny = [swaps.get(color, color) for color in palette]
    else:
        shiny = [shiny_color(color, SHINY_HUE_SHIFT.get(name, 120)) for color in palette]
    shiny += [(0, 0, 0)] * (16 - len(shiny))
    save_indexed(f"{name}_shiny", [[0] * 8 for _ in range(8)], shiny,
                 {"type": "sprite_palette", "bpp_mode": "bpp_4", "colors_count": 16})


def save_species(name, walk_1, walk_2, size=16, extra_frames=()):
    own_outline = {"k": "m"}
    palette = save_sprite_sheet(name, [walk_1, walk_2, whiten(walk_1), recolor(walk_1, own_outline),
                                       recolor(walk_2, own_outline), *extra_frames], size)
    save_shiny_palette(name, palette)


def save_wave():
    palette = [TRANSPARENT, (136, 136, 144)]
    pixels = []
    strokes = [(6, 11), (3, 15), (25, 11), (28, 15)]
    for shift in (0, 1):
        frame = [[0] * 32 for _ in range(32)]
        for x, y in strokes:
            x += -shift if x < 16 else shift
            for step in range(5):
                frame[y + step + shift][x + (step + shift) % 2] = 1
        pixels.extend(frame)
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
    palette = [TRANSPARENT, (40, 48, 72), (112, 136, 184), (200, 208, 224), (248, 248, 240)]
    pixels = [[0] * 256 for _ in range(256)]
    top, bottom, left, right = 178, 207, 9, 246
    for y in range(top, bottom + 1):
        for x in range(left, right + 1):
            depth = min(x - left, y - top, right - x, bottom - y)
            pixels[y][x] = 1 if depth == 0 else 2 if depth == 1 else 3 if depth == 2 else 4
    for cx, cy in ((left, top), (right, top), (left, bottom), (right, bottom)):
        dx = 1 if cx == left else -1
        dy = 1 if cy == top else -1
        for x, y in ((cx, cy), (cx + dx, cy), (cx, cy + dy)):
            pixels[y][x] = 0
        pixels[cy + dy][cx + dx] = 1
    save_indexed("text_box", pixels, palette, {"type": "regular_bg"})


def blank(value):
    return [[value] * 8 for _ in range(8)]


def split_quad(big):
    return [[row[x0:x0 + 8] for row in big[y0:y0 + 8]] for y0 in (0, 8) for x0 in (0, 8)]


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
    (40, 48, 72),
    (112, 136, 184),
    (200, 208, 224),
    (232, 216, 168),
    (152, 104, 64),
    (28, 34, 54),
]

WINDOW_STYLES = {
    "white": (10, 11, 12, 5),
    "paper": (2, 14, 5, 13),
    "blue": (10, 12, 5, 11),
    "dark": (10, 11, 3, 1),
}


def window_tiles(outline, frame, highlight, fill):
    size = 24
    big = [[fill] * size for _ in range(size)]
    for y in range(size):
        for x in range(size):
            depth = min(x, y, size - 1 - x, size - 1 - y)
            big[y][x] = outline if depth == 0 else frame if depth == 1 else highlight if depth == 2 else fill
    for cx, cy in ((0, 0), (size - 1, 0), (0, size - 1), (size - 1, size - 1)):
        dx = 1 if cx == 0 else -1
        dy = 1 if cy == 0 else -1
        big[cy][cx] = 0
        big[cy][cx + dx] = 0
        big[cy + dy][cx] = 0
        big[cy + dy][cx + dx] = outline
        big[cy][cx + 2 * dx] = outline
        big[cy + 2 * dy][cx] = outline
    return [[row[x0:x0 + 8] for row in big[y0:y0 + 8]] for y0 in (0, 8, 16) for x0 in (0, 8, 16)]


def backdrop_tiles():
    big = [[1] * 16 for _ in range(16)]
    for y in range(16):
        for x in range(16):
            band = (x + y) % 16
            big[y][x] = 15 if band < 5 else 10 if band < 7 else 1
    return split_quad(big)


def room_marker(fill, border, icon=None, icon_color=None):
    big = [[1] * 16 for _ in range(16)]
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
    tile = blank(1)
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
    *backdrop_tiles(),
    *[tile for style in ("white", "paper", "blue", "dark") for tile in window_tiles(*WINDOW_STYLES[style])],
    *room_marker(9, 5),
]


TILESET_SIZE = 67


def room_tileset(name):
    import importlib
    import sys
    sys.path.insert(0, str(ROOT / "tools" / "tilesets"))
    tiles, palette = getattr(importlib.import_module(name), f"{name}_tileset")()
    assert len(tiles) == TILESET_SIZE and len(palette) == 16, name
    return tiles, palette


TILE_ANIMATIONS = {
    "lake": {"water": "sway", "flows": True},
    "den": {"water": "sway", "flows": True, "special": "spin"},
    "chasm": {"flows": True},
    "hideout": {"flows": True},
    "volcano": {"special": "drift", "door": "drift"},
}
ANIMATION_FRAMES = 4


def join_block(tiles, first):
    return [tiles[first + (y // 8) * 2 + x // 8][y % 8][x % 8] for y in range(16) for x in range(16)]


def split_flat_block(flat):
    return [[[flat[(y0 + y) * 16 + x0 + x] for x in range(8)] for y in range(8)] for y0 in (0, 8) for x0 in (0, 8)]


def shifted_block(flat, dx, dy):
    return [flat[((y - dy) % 16) * 16 + (x - dx) % 16] for y in range(16) for x in range(16)]


def rotated_block(flat, turns):
    for _ in range(turns):
        flat = [flat[(15 - x) * 16 + y] for y in range(16) for x in range(16)]
    return flat


def flow_step(flat, dx, dy):
    period_8 = shifted_block(flat, 8 * (dx != 0), 8 * (dy != 0)) == flat
    return 2 if period_8 else 4


def animated_tiles(name, tiles, frame):
    config = TILE_ANIMATIONS[name]
    tiles = [tile for tile in tiles]
    if config.get("water"):
        flat = join_block(tiles, 35)
        tiles[35:39] = split_flat_block(shifted_block(flat, [0, 1, 2, 1][frame], 0))
    if config.get("flows"):
        for first, dx, dy in ((39, 1, 0), (43, -1, 0), (47, 0, 1), (51, 0, -1)):
            flat = join_block(tiles, first)
            step = flow_step(flat, dx, dy) * frame
            tiles[first:first + 4] = split_flat_block(shifted_block(flat, dx * step, dy * step))
    firsts = []
    if config.get("special"):
        firsts += [(55 + phase * 4, config["special"]) for phase in range(3)]
    if config.get("door"):
        firsts.append((19, config["door"]))
    for first, kind in firsts:
        flat = join_block(tiles, first)
        flat = rotated_block(flat, frame) if kind == "spin" else shifted_block(flat, 4 * frame, 0)
        tiles[first:first + 4] = split_flat_block(flat)
    return tiles


def save_tiles(name, tiles, palette):
    if name in TILE_ANIMATIONS:
        for frame in range(1, ANIMATION_FRAMES):
            save_tile_strip(f"{name}_tiles_{frame}", animated_tiles(name, tiles, frame), palette)
    save_tile_strip(f"{name}_tiles", tiles, palette)
    save_indexed(f"{name}_palette", [[0] * 8 for _ in range(8)], palette,
                 {"type": "bg_palette", "bpp_mode": "bpp_4", "colors_count": 16})


def save_tile_strip(name, tiles, palette):
    pixels = [[0] * (8 * len(tiles)) for _ in range(8)]
    for index, tile in enumerate(tiles):
        for y in range(8):
            for x in range(8):
                pixels[y][index * 8 + x] = tile[y][x]
    save_indexed(name, pixels, palette, {"type": "regular_bg_tiles", "bpp_mode": "bpp_4"})


ONIX_SEGMENT = ["".join(row).replace(".", " ") for row in _onix_segment_grid_a()]


def main():
    GRAPHICS.mkdir(exist_ok=True)
    for old in ["room", "chansey"]:
        for suffix in [".bmp", ".json"]:
            (GRAPHICS / f"{old}{suffix}").unlink(missing_ok=True)

    save_shiny_palette("ditto", save_sprite_sheet("ditto", [DITTO, DITTO_SQUISH, whiten(DITTO), DITTO, DITTO_SQUISH,
                                                          DITTO_FLAT], 16))
    save_species("rattata", RATTATA_1, RATTATA_2)
    save_species("meowth", MEOWTH_1, MEOWTH_2)
    save_species("porygon", PORYGON_1, PORYGON_2)
    save_species("snorlax", snorlax_frame(0, False), snorlax_frame(1, False), 32, [snorlax_frame(0, True)])
    save_sprite_sheet("poke_flute", [POKE_FLUTE], 16)
    save_sprite_sheet("mart", [mart_frame()], 32)
    save_sprite_sheet("item_icons", ITEM_ICONS, 8)
    save_species("oddish", ODDISH_1, ODDISH_2)
    save_species("caterpie", CATERPIE_1, CATERPIE_2)
    save_species("paras", PARAS_1, PARAS_2)
    save_species("beedrill", BEEDRILL_1, BEEDRILL_2)
    save_species("venusaur", venusaur_frame(0, False), venusaur_frame(1, False), 32, [venusaur_frame(0, True)])
    save_sprite_sheet("clouds", [cloud_frame("y", "g"), cloud_frame("h", "p"), cloud_frame("o", "n"),
                                 cloud_frame("d", "m"), cloud_frame("w", "i"), cloud_frame("s", "w")], 16)
    save_species("geodude", GEODUDE_1, GEODUDE_2)
    save_species("diglett", DIGLETT_1, DIGLETT_2, 16, [DIGLETT_MOUND])
    save_species("zubat", ZUBAT_1, ZUBAT_2)
    save_species("onix", onix_frame(0, False), onix_frame(1, False), 32, [onix_frame(0, True)])
    save_sprite_sheet("onix_segment", [ONIX_SEGMENT, whiten(ONIX_SEGMENT)], 16)
    save_sprite_sheet("light", [light_circle()], 64)
    save_species("magikarp", MAGIKARP_1, MAGIKARP_2)
    save_species("poliwag", POLIWAG_1, POLIWAG_2)
    save_species("staryu", STARYU_1, STARYU_2)
    save_species("horsea", HORSEA_1, HORSEA_2)
    save_species("gyarados", gyarados_frame(0, False), gyarados_frame(1, False), 32, [gyarados_frame(0, True)])
    save_sprite_sheet("water_projectiles", [BUBBLE, WATER_DROP, STAR, DRAGON_FIRE, ICE_SHARD, HEART, SLUDGE,
                                            SHADOW_BALL, BONE], 8)
    save_species("pikachu", PIKACHU_1, PIKACHU_2)
    save_species("voltorb", VOLTORB_1, VOLTORB_2)
    save_species("magnemite", MAGNEMITE_1, MAGNEMITE_2)
    save_species("zapdos", zapdos_frame(0, False), zapdos_frame(1, False), 32, [zapdos_frame(0, True)])
    save_sprite_sheet("electric_projectiles", [SPARK, BOLT, THUNDER_WAVE, EMBER, FEATHER], 8)
    save_species("machop", MACHOP_1, MACHOP_2)
    save_species("charmander", CHARMANDER_1, CHARMANDER_2)
    save_species("bulbasaur", BULBASAUR_1, BULBASAUR_2)
    save_species("sandshrew", SANDSHREW_1, SANDSHREW_2)
    save_sprite_sheet("pickups", [ITEM_BALL, JOURNAL_PAGE, SILPH_SCOPE], 16)
    save_species("vulpix", VULPIX_1, VULPIX_2)
    save_species("ponyta", PONYTA_1, PONYTA_2)
    save_species("growlithe", GROWLITHE_1, GROWLITHE_2)
    save_species("magmar", MAGMAR_1, MAGMAR_2)
    save_species("squirtle", SQUIRTLE_1, SQUIRTLE_2)
    save_species("moltres", moltres_frame(0, False), moltres_frame(1, False), 32, [moltres_frame(0, True)])
    save_species("seel", SEEL_1, SEEL_2)
    save_species("jynx", JYNX_1, JYNX_2)
    save_species("shellder", SHELLDER_1, SHELLDER_2)
    save_species("omanyte", OMANYTE_1, OMANYTE_2)
    save_species("articuno", articuno_frame(0, False), articuno_frame(1, False), 32, [articuno_frame(0, True)])
    save_species("pidgey", PIDGEY_1, PIDGEY_2)
    save_species("spearow", SPEAROW_1, SPEAROW_2)
    save_species("aerodactyl", AERODACTYL_1, AERODACTYL_2)
    save_species("kabuto", KABUTO_1, KABUTO_2)
    save_species("pidgeot", pidgeot_frame(0, False), pidgeot_frame(1, False), 32, [pidgeot_frame(0, True)])
    save_species("koffing", KOFFING_1, KOFFING_2)
    save_species("ekans", EKANS_1, EKANS_2)
    save_species("grimer", GRIMER_1, GRIMER_2)
    save_species("slowpoke", SLOWPOKE_1, SLOWPOKE_2)
    save_species("arbok", arbok_frame(0, False), arbok_frame(1, False), 32, [arbok_frame(0, True)])
    save_species("weezing", weezing_frame(0, False), weezing_frame(1, False), 32, [weezing_frame(0, True)])
    save_sprite_sheet("balloon", [BALLOON], 16)
    save_species("mankey", MANKEY_1, MANKEY_2)
    save_species("machoke", MACHOKE_1, MACHOKE_2)
    save_species("farfetchd", FARFETCHD_1, FARFETCHD_2)
    save_species("hitmonlee", hitmon_frame(0, False, True), hitmon_frame(1, False, True), 32,
                 [hitmon_frame(0, True, True)])
    save_species("hitmonchan", hitmon_frame(0, False, False), hitmon_frame(1, False, False), 32,
                 [hitmon_frame(0, True, False)])
    save_species("gastly", GASTLY_1, GASTLY_2)
    save_species("haunter", HAUNTER_1, HAUNTER_2)
    save_species("cubone", CUBONE_1, CUBONE_2)
    save_species("exeggcute", EXEGGCUTE_1, EXEGGCUTE_2)
    save_species("gengar", gengar_frame(0, False), gengar_frame(1, False), 32, [gengar_frame(0, True)])
    save_species("dratini", DRATINI_1, DRATINI_2)
    save_species("dragonair", DRAGONAIR_1, DRAGONAIR_2)
    save_species("seadra", SEADRA_1, SEADRA_2)
    save_species("lapras", LAPRAS_1, LAPRAS_2)
    save_species("dragonite", dragonite_frame(0, False), dragonite_frame(1, False), 32, [dragonite_frame(0, True)])
    save_species("abra", ABRA_1, ABRA_2)
    save_species("kadabra", KADABRA_1, KADABRA_2)
    save_species("drowzee", DROWZEE_1, DROWZEE_2)
    save_species("venomoth", VENOMOTH_1, VENOMOTH_2)
    save_species("mewtwo", mewtwo_frame(0, False), mewtwo_frame(1, False), 32, [mewtwo_frame(0, True)])
    save_species("mew", mew_frame(0), mew_frame(1), 32)
    save_sprite_sheet("projectiles", [SPIT, ENEMY_SHOT, IMPACT, COIN, TRI, PSYBEAM, LEAF, NEEDLE, STRING, BEAM,
                                      ROCK, SUPERSONIC], 8)
    save_sprite_sheet("slash", [SLASH], 16)
    save_wave()
    save_hp_bar()
    save_text_box()
    save_tiles("lab", *room_tileset("lab"))
    save_tiles("forest", *room_tileset("forest"))
    save_tiles("cave", *room_tileset("cave"))
    save_tiles("lake", *room_tileset("lake"))
    save_tiles("plant", *room_tileset("plant"))
    save_tiles("volcano", *room_tileset("volcano"))
    save_tiles("ice", *room_tileset("ice"))
    save_tiles("chasm", *room_tileset("chasm"))
    save_tiles("hideout", *room_tileset("hideout"))
    save_tiles("dojo", *room_tileset("dojo"))
    save_tiles("tower", *room_tileset("tower"))
    save_tiles("den", *room_tileset("den"))
    save_tiles("peak", *room_tileset("peak"))
    save_tiles("overlay", OVERLAY_TILES, OVERLAY_PALETTE)


if __name__ == "__main__":
    main()
