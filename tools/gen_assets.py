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


ODDISH_TOP = [
    "",
    "   v   v   v",
    "  vVv vVv vVv",
    "   vVvVVvVVv",
    "    vVVVVVv",
    "     kkkkk",
    "    kuuuuuk",
    "   kuuuuuuuk",
    "   kuekuekuk",
    "   kuuuuuuuk",
    "   kUuuuuuUk",
    "    kUUUUUk",
]

ODDISH_1 = ODDISH_TOP + ["    kuk kuk", "    kkk kkk"]
ODDISH_2 = ODDISH_TOP + ["   kuk   kuk", "   kkk   kkk"]

CATERPIE_1 = [
    "",
    "",
    "",
    "           nn",
    "          n",
    "        kkkkk",
    "       kvvvvvk",
    "   kkk kvvvwek",
    "  kvvvkvvvvvvk",
    " kyvvvvkvvvvvk",
    " kyyvvvkvvvvk",
    "  kkvvvvkvvk",
    "    kkkkkkk",
    "    c c c c",
]

CATERPIE_2 = [
    "",
    "",
    "",
    "           nn",
    "          n",
    "        kkkkk",
    "       kvvvvvk",
    "    kk kvvvwek",
    "   kvvkvvvvvvk",
    "  kyvvvkvvvvvk",
    "  kyyvvkvvvvk",
    "   kkvvvkvvk",
    "     kkkkkk",
    "     c c c",
]

PARAS_TOP = [
    "",
    "  kkkk  kkkk",
    " knwnnkknnwnk",
    " knnnnkknnnnk",
    "  kkyk  kykk",
    "   kookkook",
    "  kooooooook",
    " kooeooooeook",
    " kooooooooook",
    "  kowooooowok",
    "   kkkkkkkkk",
]

PARAS_1 = PARAS_TOP + ["  kok kok kok", "  kk  kk  kk"]
PARAS_2 = PARAS_TOP + ["   kok kok kok", "   kk  kk  kk"]

BEEDRILL_1 = [
    "",
    "   ss   ss",
    "  ssss ssss",
    "   ssskss",
    "     kyyk   k",
    "    kyekyk kwk",
    "  kkyyyykkkkwk",
    " kwkkkkyyk  k",
    "kwk kyykkyk",
    " k kyyyykkyk",
    "   kkykkyyk",
    "    kyyyyk",
    "     kkkwk",
    "       kk",
]

BEEDRILL_2 = [
    "",
    "",
    "  ssss ssss",
    " sssss sssss",
    "     kyyk   k",
    "    kyekyk kwk",
    "  kkyyyykkkkwk",
    " kwkkkkyyk  k",
    "kwk kyykkyk",
    " k kyyyykkyk",
    "   kkykkyyk",
    "    kyyyyk",
    "     kkkwk",
    "       kk",
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

DIGLETT_1 = [
    "",
    "",
    "",
    "      kkkk",
    "     kxxxxk",
    "    kxxxxxxk",
    "    kxkxxkxk",
    "    kxxxxxxk",
    "    kxxqqxxk",
    "    kxxqqxxk",
    "    kxxxxxxk",
    "  kkkkkkkkkkkk",
    " kAAbAAbAAbAAk",
    "kAAbAAAAAAbAAAk",
    " kkkkkkkkkkkkk",
]

DIGLETT_2 = [""] * 3 + DIGLETT_1[:3] + DIGLETT_1[5:12] + DIGLETT_1[12:]

DIGLETT_MOUND = [
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
    "      kkkk",
    "    kkAbbAkk",
    "  kkAbAAAAbAkk",
    " kAAAAbAAAbAAAk",
    " kkkkkkkkkkkkkk",
]

ZUBAT_1 = [
    "",
    "",
    "k             k",
    "kZk    k k   kZk",
    "kZZk   kzk  kZZk",
    " kZZk kzzzk kZZk",
    " kZzZkzkzkzkZzZk",
    "  kZzZzzzzzzZzZk",
    "   kZzzwzwzzZk",
    "    kkzzzzzkk",
    "      kzzzk",
    "      kqkqk",
    "       k k",
]

ZUBAT_2 = [
    "",
    "",
    "",
    "",
    "        k k",
    "  kkk  kzk  kkk",
    " kZZZkkzzzkkZZZk",
    "kZzZzkzkzkzkZzZk",
    " kZzZzzzzzzZzZk",
    "  kkZzzwzwzzZkk",
    "    kkzzzzzkk",
    "      kzzzk",
    "      kqkqk",
    "       k k",
]

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

ONIX_SEGMENT = [
    "",
    "     kkkkk",
    "   kkaaaaakk",
    "  kaaaaaaaaak",
    " kaaaAaaaaaaak",
    " kaaAaaaaaAaak",
    "kaaaaaaaaaaaaak",
    "kaaaaaaaaaaAAak",
    "kaaAAaaaaaaaaak",
    "kaaaaaaaaAaaaak",
    " kaaaaaaaaaaak",
    " kAaaaaAaaaaAk",
    "  kAAaaaaaAAk",
    "   kkAAAAAkk",
    "     kkkkk",
]


def onix_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    ellipse(grid, 16, 18, 13, 11, "a")
    ellipse(grid, 16, 22, 10, 6, "A")
    ellipse(grid, 16, 17, 11, 8, "a")
    for x in range(10, 23):
        grid[4 + (x % 3)][x] = "a"
    for y in range(2, 9):
        grid[y][16] = "a"
        grid[y][17] = "A"
    eye = "w" if charging else "k"
    for x in (10, 11, 21, 22):
        grid[15][x] = eye
    grid[14][10] = grid[14][22] = "k"
    for x in range(11, 22):
        grid[23 + step][x] = "k"
    for x, y in ((8, 12), (24, 12), (13, 26), (19, 26)):
        grid[y][x] = "A"
    return ["".join(row) for row in outline(grid)]


MAGIKARP_1 = [
    "",
    ".....k.k.k......",
    "....kckckck...kk",
    "...kkcccccck.kck",
    "..knnnnnnnnnkcck",
    ".knwwnnonnonnkck",
    ".knwkwnnonnonnkk",
    "kcnwwnonnonnonk",
    "kcccnnnnonnonnk",
    ".kcknnnnnnnnnnkk",
    "ykcknnnnnnnnnkck",
    "y..kccnnnnnnkcck",
    "....kcckkcck.kck",
    ".....kk..kk...kk",
]
MAGIKARP_1 = [row.replace(".", " ") for row in MAGIKARP_1]

MAGIKARP_2 = [""] + MAGIKARP_1[:-1]

POLIWAG_1 = [
    "",
    "",
    "",
    "      kkkk",
    "    kkuuuukk",
    "   kuuuuuuuuk",
    "  kuwkuuuuwkuk",
    "  kuwwkkkkwwuk",
    "  kuuwwwwwwuuk",
    "  kuuwwkkwwuuk",
    "  kuuwwkwwwuuk",
    "   kuuwwwwuuk",
    "    kkuuuukkk",
    "     kk  kiiik",
    "          kkk",
]

POLIWAG_2 = [""] + POLIWAG_1[:-1]

STARYU_1 = [
    "",
    "       kk",
    "      kxxk",
    "      kxxk",
    "     kxxxxk",
    "kkkkkkxxxxkkkkkk",
    "kxxxxxxnnxxxxxxk",
    " kxxxxnwnnxxxxk",
    "  kxxxxnnxxxxk",
    "   kxxxxxxxxk",
    "  kxxxxkkxxxxk",
    "  kxxxk  kxxxk",
    " kxxxk    kxxxk",
    " kxxk      kxxk",
    " kkk        kkk",
]

STARYU_2 = [""] + STARYU_1[:-1]

HORSEA_1 = [
    "",
    "      kkk",
    "     kBBBk",
    "  kkkBBwkBk",
    " kBBBBBBBBk",
    "  kkkkBBBBk",
    "      kBBBk",
    "     kBByBkk",
    "    kBByyBkwk",
    "    kBByyBkwk",
    "     kBByBkk",
    "      kBBBk",
    "       kBBk",
    "    kk kBk",
    "     kkBk",
    "       k",
]

HORSEA_2 = [""] + HORSEA_1[:-1]

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


PIKACHU_1 = [
    "",
    " kk          kk",
    " kkk        kkk",
    "  kyk      kyk",
    "  kyyk kkk kyyk",
    "   kyykyyykyyk   kk",
    "    kyyyyyyyk   kyk",
    "   kyykyyykyyk kyyk",
    "   kynyyyyynyk kyk",
    "   kyyyykyyyyk kk",
    "    kyyyyyyyk kyk",
    "   kyxyyyyyxyk k",
    "   kyyyyyyyyykk",
    "    kyk   kyk",
    "    kkk   kkk",
]

PIKACHU_2 = [
    "",
    "  kk        kk",
    "  kkk      kkk",
    "   kyk    kyk",
    "   kyyk kkkyyk",
    "    kyykyyykyk   kk",
    "    kyyyyyyyk   kyk",
    "   kyykyyykyyk kyyk",
    "   kynyyyyynyk kyk",
    "   kyyyykyyyyk kk",
    "    kyyyyyyyk kyk",
    "   kyxyyyyyxyk k",
    "   kyyyyyyyyykk",
    "   kyk     kyk",
    "   kkk     kkk",
]

PIKACHU_1 = [row[:16] for row in PIKACHU_1]
PIKACHU_2 = [row[:16] for row in PIKACHU_2]

VOLTORB_1 = [
    "",
    "",
    "     kkkkkk",
    "   kknnnnnnkk",
    "  knnhnnnnnnnk",
    " knnnnnnnnnnnnk",
    " knkkknnnnkkknk",
    "kkkwwkkkkkkwwkkk",
    "kwwkkwwwwwwkkwwk",
    "kwwwwwwwwwwwwwwk",
    " kwwwkkkkkkwwwk",
    " kwwwwwwwwwwwwk",
    "  kwwwwwwwwwwk",
    "   kkwwwwwwkk",
    "     kkkkkk",
]

VOLTORB_2 = [""] + VOLTORB_1[:-1]

MAGNEMITE_1 = [
    "",
    "       kk",
    "       ks",
    "      kkkk",
    "     kssssk",
    "kkk kssssssk kkk",
    "kBkkssskkssskkBk",
    "kBBksskwwksskBBk",
    "kkkksskwkksskkkk",
    "kBBksssskssskBBk",
    "kBkkssssssssskBk",
    "kkk kssssssk kkk",
    "      kssssk",
    "    kk kkkk kk",
    "    ks      sk",
    "     k      k",
]

MAGNEMITE_2 = [""] + MAGNEMITE_1[:-1]

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

CHARMANDER_TOP = [
    "",
    "     kkkk",
    "    koooook",
    "   kooookook",
    "   koookwkok",
    "   kooooooook   n",
    "    koooooook  nyn",
    "     kkooook   nyn",
    "    koocccok   kok",
    "   kookccccok kook",
    "   kok ccccokooook",
    "       kcccooookk",
]

CHARMANDER_1 = [row[:16] for row in CHARMANDER_TOP + ["      koook kok", "      kkkk  kk"]]
CHARMANDER_2 = [row[:16] for row in CHARMANDER_TOP + ["     koook  kok", "     kkkk   kk"]]

BULBASAUR_TOP = [
    "",
    "      kkkk",
    "     kvVvvk",
    "    kvVvvVvk",
    "   kvvVvvVvvk",
    "  kjjkvvvvkjjk",
    " kjjjjkkkkjjjjk",
    " kjjejjjjjejjjk",
    " kjjjjjjjjjjjjk",
    "  kjjjkkkkjjjk",
    "   kjjjjjjjjk",
]

BULBASAUR_1 = BULBASAUR_TOP + ["   kJk kk kJk", "   kkk    kkk"]
BULBASAUR_2 = BULBASAUR_TOP + ["  kJk  kk  kJk", "  kkk      kkk"]

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

PONYTA_TOP = [
    "",
    "        kyk",
    "      kyonyk",
    "     kyonnook",
    "  kk kccyonok",
    " kcckccccyok",
    " kcekccccck",
    "  kkkccccck   kyk",
    "     kcccccckyonk",
    "    kccccccccony",
    "    kcccccccckk",
    "    kcck  kcck",
]

PONYTA_1 = [row[:16] for row in PONYTA_TOP + ["    kbbk  kbbk", "    kkk   kkk"]]
PONYTA_2 = [row[:16] for row in PONYTA_TOP + ["   kbbk    kbbk", "   kkk     kkk"]]

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

MAGMAR_TOP = [
    "",
    "     knk  knk",
    "    knyk knyk",
    "    knnnknnnk",
    "   knnwnnnwnk",
    "   knnnyynnnk",
    "  kknnnnnnnnkk",
    " knnknyyyynknnk",
    " knk kyyyyk knk",
    "  k  knyynk  k",
    "     knnnnk",
    "    knnkknnk",
]

MAGMAR_1 = MAGMAR_TOP + ["    kyk  kyk", "    kkk  kkk"]
MAGMAR_2 = MAGMAR_TOP + ["   kyk    kyk", "   kkk    kkk"]

SQUIRTLE_TOP = [
    "",
    "     kkkk",
    "    kiiiik",
    "   kiiwkiik",
    "   kiikkiik",
    "    kiiiik",
    "  kkkbbbbkkk",
    " kiikbccbkiik",
    "  kkbccccbkk  kk",
    "   kbccccbk  kiik",
    "   kbbbbbbk kiik",
    "    kiiiiikkik",
]

SQUIRTLE_1 = [row[:16] for row in SQUIRTLE_TOP + ["   kiik kiik", "   kkk  kkk"]]
SQUIRTLE_2 = [row[:16] for row in SQUIRTLE_TOP + ["  kiik   kiik", "  kkk    kkk"]]


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

JYNX_TOP = [
    "",
    "    kkkkkk",
    "   kyyyyyyk",
    "  kyyyyyyyyk",
    "  kyzzzzzzyk",
    "  kyzkzzkzyk",
    "  kyzznnzzyk",
    "  kyyzzzzyyk",
    " kyykmmmmkyyk",
    "  kkmmmmmmkk",
    "   kmmmmmmk",
    "  kmmmmmmmmk",
]

JYNX_1 = JYNX_TOP + ["  kmmmmmmmmk", "   kkkkkkkk"]
JYNX_2 = JYNX_TOP + [" kmmmmmmmmmmk", "  kkkkkkkkkk"]

SHELLDER_1 = [
    "",
    "",
    "      kkkk",
    "    kkzzzzkk",
    "   kzZzzzzZzk",
    "  kzZzzZZzzZzk",
    "  kzzkkkkkkzzk",
    " kzzkwwkkwwkzzk",
    " kzzkwkkkkwkzzk",
    " kzzkkqqqqkkzzk",
    "  kzZkqqqqkZzk",
    "  kzzZzzzzZzzk",
    "   kkzzzzzzkk",
    "     kkkkkk",
]

SHELLDER_2 = [""] + SHELLDER_1[:-1]

OMANYTE_1 = [
    "",
    "",
    "     kkkkk",
    "   kkiiiiikk",
    "  kiiBBBBiiik",
    " kiiBkkkkBiiik",
    " kiBkiiiikBiik",
    " kiBkiBBikBiik",
    " kiiBkkkiBiik",
    "  kiiBBBBiik",
    "  kkkkkkkkkk",
    " kcckcckcckk",
    " kckkckkckk",
    "  k  k  k",
]

OMANYTE_2 = [""] + OMANYTE_1[:-1]

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
    "",
    "      kkkk",
    "     kbbbbk",
    "    kbbwkbbk",
    "   kkbbkkbbk",
    "  kyykbbbbbkkk",
    "   kkcccbbbbbbk",
    "    kccccbbbbbk",
    "    kccccbbbbk",
    "     kccccbkk",
]

PIDGEY_1 = PIDGEY_TOP + ["      kykyk", "      kk kk"]
PIDGEY_2 = PIDGEY_TOP + ["     kyk kyk", "     kk   kk"]

SPEAROW_TOP = [
    "",
    "      kkk",
    "     kxxxk",
    "    kxxwkxk",
    "   kkxxkkxk",
    "  kyyykxxxxk kk",
    "   kkkcxxxxkknk",
    "    kccxnnnnnnk",
    "    kcccnnnnnk",
    "     kcccxnkk",
    "      kkkkk",
]

SPEAROW_1 = SPEAROW_TOP + ["      kykyk", "      kk kk"]
SPEAROW_2 = SPEAROW_TOP + ["     kyk kyk", "     kk   kk"]

KABUTO_1 = [
    "",
    "",
    "",
    "     kkkkkk",
    "   kkxxxxxxkk",
    "  kxxbxxxxbxxk",
    " kxxxxbbbbxxxxk",
    " kxxxxxxxxxxxxk",
    " kkxkyykkyykxkk",
    "  kxkyykkyykxk",
    "   kkkkkkkkkk",
    "   kxk kk kxk",
    "   kk      kk",
]

KABUTO_2 = [""] + KABUTO_1[:-1]

FEATHER = [
    "",
    "     kk",
    "    kwk",
    "   kwsk",
    "  kwsk",
    " kwsk",
    " kkk",
]


KOFFING_1 = [
    "",
    "   k  kk  k",
    "  kzk kzk kzk",
    "   kkzzzzkkk",
    "  kzzzzzzzzk",
    " kzzwkzzzwkzk",
    " kzzkkzzzkkzzk",
    "kzzzzzzzzzzzzk",
    "kzzzzwwwwzzzzk",
    "kzzzzwkkwzzzzk",
    " kzzzzwwzzzzk",
    " kzzzzzzzzzzk",
    "  kkzzzzzzkk",
    "    kkkkkk",
]

KOFFING_2 = [""] + KOFFING_1[:-1]

EKANS_TOP = [
    "",
    "",
    "     kkkk",
    "    kzzzzk",
    "   kzzwkzzk",
    "   kzzzzzzk",
    "    kkyzzkk",
    "      kzzk",
    "     kzzk   kkk",
    "    kzzk   kzzzk",
    "   kzyzk  kzzyzk",
    "   kzzzzkkzzzzk",
]

EKANS_1 = EKANS_TOP + ["    kzzzzzzzzk", "     kkkkkkkk"]
EKANS_2 = EKANS_TOP + ["   kzzzzzzzzzk", "    kkkkkkkkk"]

GRIMER_TOP = [
    "",
    "",
    "",
    "      kkkk",
    "     kmmmmk",
    "    kmmmmmmk",
    "   kmwkmmwkmk",
    "   kmkkmmkkmk",
    "  kmmmmmmmmmmk",
    "  kmmmkkkkmmmk",
    " kmmmmmmmmmmmmk",
]

GRIMER_1 = GRIMER_TOP + [" kmmpmmmmmmpmmk", "kmmmmmmmmmmmmmmk", " kkkkkkkkkkkkkk"]
GRIMER_2 = GRIMER_TOP + ["kmmpmmmmmmmmpmmk", "kmmmmmmmmmmmmmmk", "kkkkkkkkkkkkkkkk"]

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
    "        vk",
    "   kkk  vVk",
    "  kbbbk vVk",
    " kbbwkbk vVk",
    " kbbbbbbkvVk",
    "kyykbbbbbkVk",
    " kkkcccbbbbk",
    "   kcccbbbbbk",
    "   kccccbbbbk",
    "    kcccbbkk",
]

FARFETCHD_1 = FARFETCHD_TOP + ["     kykyk", "     kk kk"]
FARFETCHD_2 = FARFETCHD_TOP + ["    kyk kyk", "    kk   kk"]


GASTLY_1 = [
    "",
    "   z   zz   z",
    "  zpz zppz zpz",
    " zppzzkkkkzzppz",
    "  zpkkkkkkkkpz",
    " zpkkwwkkwwkkpz",
    " zpkkwkkkkwkkpz",
    "zppkkkkkkkkkkppz",
    " zpkkknnnnkkkpz",
    " zpkkkkwwkkkkpz",
    "  zpkkkkkkkkpz",
    " zppzkkkkkkzppz",
    "  zpz zzzz zpz",
    "   z        z",
]

GASTLY_2 = [
    "",
    "",
    "  z   zz   z",
    " zpzzkkkkzzpz",
    "  zpkkkkkkkkpz",
    " zpkkwwkkwwkkpz",
    " zpkkwkkkkwkkpz",
    "zppkkkkkkkkkkppz",
    "zppkkknnnnkkkppz",
    " zpkkkkwwkkkkpz",
    "  zpkkkkkkkkpz",
    " zppzkkkkkkzppz",
    "  zpzzzzzzzzpz",
    "   z        z",
]

HAUNTER_1 = [
    "",
    "   k  kkkk  k",
    "  kzk kzzzzkzk",
    "  kzzkzzzzzzzk",
    "   kzzwwzzwwzk",
    "   kzzwnzzwnzk",
    "kk  kzzzzzzzzk  kk",
    "kzk kznnnnnnzk kzk",
    " kzkkzzwzwzzzkkzk",
    "  kzzzzzzzzzzzk",
    "     kzzzzzzk",
    "      kzzzzk",
    "       kzzk",
    "        kk",
]

HAUNTER_1 = [row[:16] for row in HAUNTER_1]
HAUNTER_2 = [""] + HAUNTER_1[:-1]

CUBONE_TOP = [
    "",
    "   kk    kk",
    "  kwwk  kwwk",
    "  kwwkkkkwwk",
    "  kwwwwwwwwk",
    "  kwkkwwkkwk",
    "  kwkkwwkkwk",
    "   kwwkkwwk",
    "    kbbbbk   kk",
    "   kbbccbbk kwk",
    "  kbbccccbbkwk",
    "  kbbccccbbkk",
]

CUBONE_1 = CUBONE_TOP + ["   kbbk kbbk", "   kkk  kkk"]
CUBONE_2 = CUBONE_TOP + ["  kbbk   kbbk", "  kkk    kkk"]

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

ABRA_TOP = [
    "",
    "   kk    kk",
    "  kyyk  kyyk",
    "  kyyykkyyyk",
    "  kyyyyyyyyk",
    "  kykkyykkyk",
    "  kyyyyyyyyk",
    "   kyyxxyyk",
    "  kbbkyykbbk",
    " kbbbkkkkbbbk",
    "  kkyyyyyykk",
    "   kyyyyyyk",
]

ABRA_1 = ABRA_TOP + ["   kyk  kyk", "   kkk  kkk"]
ABRA_2 = ABRA_TOP + ["  kyk    kyk", "  kkk    kkk"]

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

VENOMOTH_1 = [
    "",
    " kk  k  k  kk",
    "kzzk kk kk kzzk",
    "kzhzk kkk kzhzk",
    "kzzzzkzzzkzzzzk",
    " kzhzzkekzzhzk",
    " kzzzzzzzzzzzk",
    "  kzzkzzzkzzk",
    " kzhzzkzkzzhzk",
    "kzzzzk kzk kzzzk",
    "kzhzk  kzk  kzhk",
    " kkk   kk   kkk",
]

VENOMOTH_2 = [
    "",
    "",
    "     k  k",
    "  kk  kk  kk",
    " kzzkkkkkkzzk",
    "kzhzzkzzzkzzhzk",
    "kzzzzkekzzzzzzk",
    " kzzzzzzzzzzzk",
    "  kkzzzzzzzkk",
    " kzhzzkzkzzhzk",
    " kzzzk kzk kzzk",
    "  kkk  kk  kkk",
]


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


def mewtwo_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    ellipse(grid, 16, 16, 6, 8, "h")
    ellipse(grid, 16, 21, 5, 4, "z")
    ellipse(grid, 16, 6, 5, 4.5, "h")
    for x in (11, 12, 20, 21):
        grid[2][x] = grid[3][x] = "h"
    for y in range(8, 11):
        grid[y][16] = "p"
    grid[6][14] = grid[6][18] = "w" if charging else "z"
    for side in (-1, 1):
        for i in range(6):
            grid[12 + i][16 + side * (7 + i // 2)] = "h"
        ellipse(grid, 16 + side * 11, 19, 1.5, 1.5, "h")
    for i in range(12):
        x = 22 + i // 2 - step
        y = 22 + (i // 3)
        if x < 32 and y < 32:
            grid[y][x] = "z"
            grid[y][min(x + 1, 31)] = "z"
    for x in (13, 14, 18, 19):
        for y in range(24, 31):
            grid[y][x] = "h"
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


def weezing_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    ellipse(grid, 11, 12 + step, 9, 8.5, "z")
    ellipse(grid, 22, 20 - step, 7, 6.5, "z")
    ellipse(grid, 22, 8, 3.5, 3, "z")
    for cx, cy, r in ((11, 12 + step, 9), (22, 20 - step, 7)):
        grid[cy - 2][cx - 3] = grid[cy - 2][cx + 2] = "w" if charging else "k"
        for x in range(cx - 3, cx + 3):
            grid[cy + 3][x] = "k"
        for dx in (-r // 2, r // 2):
            grid[cy - r + 1][cx + dx] = "y"
    ellipse(grid, 11, 15 + step, 3, 1.5, "w")
    ellipse(grid, 22, 22 - step, 2.5, 1.2, "w")
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


def gyarados_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    ellipse(grid, 16, 24 + step, 13, 6, "u")
    ellipse(grid, 16, 25 + step, 9, 3, "c")
    ellipse(grid, 4, 20, 3, 2, "w")
    ellipse(grid, 28, 20, 3, 2, "w")
    ellipse(grid, 16, 11, 10, 7.5, "u")
    for x in range(10, 23, 3):
        for y in range(1, 5):
            if abs(x - 16) // 2 + y > 1:
                grid[y][x] = "w"
    ellipse(grid, 16, 15, 5.5, 2.5, "B" if charging else "k")
    for x in (12, 14, 18, 20):
        grid[13][x] = "w"
    grid[8][11] = grid[8][12] = "e"
    grid[8][20] = grid[8][21] = "e"
    for x in (10, 11, 12, 13):
        grid[6][x] = "w"
    for x in (19, 20, 21, 22):
        grid[6][x] = "w"
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


def venusaur_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    ellipse(grid, 8, 28 - step, 4, 3, "J")
    ellipse(grid, 24, 28 - (1 - step), 4, 3, "J")
    ellipse(grid, 16, 22, 13, 7.5, "j")
    ellipse(grid, 5, 15, 5, 3, "v")
    ellipse(grid, 27, 15, 5, 3, "v")
    for y in range(10, 17):
        grid[y][15] = grid[y][16] = "b"
    ellipse(grid, 16, 9, 12, 4.5, "q")
    ellipse(grid, 16, 9, 4, 2.5, "w" if charging else "y")
    for x in range(8, 25, 4):
        grid[9][x] = "d"
    grid[20][10] = grid[20][11] = "e"
    grid[20][21] = grid[20][22] = "e"
    for x in range(12, 21):
        grid[24][x] = "J"
    return ["".join(row) for row in outline(grid)]


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
    ".............kkk..........",
    "...........kk777kkk.......",
    "..........k77kkk777kk.....",
    ".........k7kk...kk777k....",
    ".........k7k.....k8778k...",
    "..........k7kk....k888k...",
    "...........k77kk...kkk....",
    "............kk77k.........",
    "...kkk........kk8k........",
    "...k77kkkkkk...k8k........",
    "...k7k77778kkkk8k.........",
    "...k777778889778k.........",
    "..kw977799787788k.........",
    "..k09777w9787788k.........",
    "..k19777097877888k........",
    "..k77777197kkk8k88k.......",
    "..kk777779kkk.kkkk8k......",
    "...k888779k......kkk......",
    "....kkkkkk................",
]


def mew_frame(step):
    rows = ["." * 32] * (6 + step) + ["..." + row + "..." for row in MEW]
    return (rows + ["." * 32] * 32)[:32]


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
    "onix": {"a": (216, 184, 72), "A": (160, 128, 40)},
    "zubat": {"z": (128, 192, 96), "Z": (64, 128, 64)},
    "dragonite": {"o": (152, 176, 80)},
    "mewtwo": {"h": (232, 236, 228), "p": (200, 208, 200), "z": (120, 192, 96)},
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
