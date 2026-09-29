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


GEODUDE_BODY = [
    "",
    "",
    "",
    "      kkkk",
    "    kkaaaakk",
    "   kaaaaaaaak",
    "   kaakaakaak",
    "  kaaaaaaaaaak",
    "  kaaaAAAAaaak",
    "   kaaaaaaaak",
    "    kkaaaakk",
]

GEODUDE_1 = GEODUDE_BODY + [
    " kaak kkkk kaak",
    " kaaak    kaaak",
    "  kkk      kkk",
]

GEODUDE_2 = GEODUDE_BODY + [
    "kaak  kkkk  kaak",
    " kaak      kaak",
    "  kk        kk",
]

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
    "",
    "      kkk",
    "     kyyyk",
    "   kkkkkkkk",
    "  koooooooook",
    " kowkooooooookk",
    " kokkoooooooookw",
    "kyooooccoooookww",
    " kooooccooooookw",
    "  kyooooooooookk",
    "   kkkooooookk",
    "     kwkkkwk",
    "      k   k",
]

MAGIKARP_2 = [
    "",
    "",
    "",
    "      kkk",
    "     kyyyk",
    "   kkkkkkkk  kk",
    "  koooooooookwk",
    " kowkoooooooookw",
    "kyokkoooccooookw",
    " kooooooccoooook",
    "  kyooooooooook",
    "   kkkooooookk",
    "     kwkkkwk",
    "      k   k",
]

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
    "      kkkk",
    "     kssssk",
    "    ksksskskk",
    "    ksssssssk",
    "    ksskkksk",
    "  kk kssssk kk",
    " kssksssssskssk",
    " ksskssbbsskssk",
    "  kk ksssssk kk",
    "     ksssssk",
    "     kssksk",
]

MACHOP_1 = MACHOP_TOP + ["    kssk kssk", "    kkkk kkkk"]
MACHOP_2 = MACHOP_TOP + ["   kssk   kssk", "   kkkk   kkkk"]

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
    "    kk    kk",
    "    kykkkkyk",
    "   kyyyyyyyyk",
    "   kykyyyykyk",
    "   kyyyxxyyyk",
    "  kkyyyyyyyykk",
    " kxxkxxxxxxkxxk",
    " kxkxxxxxxxxkxk",
    "  kkxxxxxxxxkk",
    "   kyyyyyyyyk",
]

SANDSHREW_1 = SANDSHREW_TOP + ["   kwk    kwk", "   kkk    kkk"]
SANDSHREW_2 = SANDSHREW_TOP + ["  kwk      kwk", "  kkk      kkk"]

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
    "   kk    kk",
    "  kbok  kobk",
    "  kbook koobk",
    "   koookooook  kk",
    "   koowooowok kook",
    "    kookoookk koook",
    "    koooooook kooook",
    "     kocccok  koook",
    "    kooccccokkoook",
    "    koocccoooook",
]

VULPIX_1 = [row[:16] for row in VULPIX_TOP + ["    kbk kbkkk", "    kkk kkk"]]
VULPIX_2 = [row[:16] for row in VULPIX_TOP + ["   kbk   kbkk", "   kkk   kkk"]]

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
    "   kk     kk",
    "  kook   kook",
    "  koookkkoook",
    "  kookoookook",
    "  kooowooowok",
    "   kccookccck",
    "  kccccccccck",
    "  koookkkoook  kk",
    " koooooooooookcck",
    " kokoookoookookk",
    "  kooooooooook",
]

GROWLITHE_1 = [row[:16] for row in GROWLITHE_TOP + ["  kook    kook", "  kkk     kkk"]]
GROWLITHE_2 = [row[:16] for row in GROWLITHE_TOP + [" kook      kook", " kkk       kkk"]]

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
    "     kkk",
    "    kwwwk",
    "   kwwwwwk",
    "   kwkwkwk",
    "   kwwnwwk",
    "  kkwwwwwkkk",
    " kwwwwwwwwwwk",
    " kwwwwwwwwwwwk  kk",
    "  kwwwwwwwwwwwkkwk",
    "   kkwwwwwwwwwwwk",
]

SEEL_1 = [row[:16] for row in SEEL_TOP + ["    kiik  kiikk", "    kkk   kkk"]]
SEEL_2 = [row[:16] for row in SEEL_TOP + ["   kiik    kiik", "   kkk     kkk"]]

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

AERODACTYL_1 = [
    "",
    "  k           k",
    " kzk   kkk   kzk",
    " kzzk kzzzk kzzk",
    " kzzzkzwzzzkzzzk",
    "  kzzzzkzzzzzzk",
    "  kzzzzzzzzzzzk",
    "   kzzkzwzwzkzk",
    "   kzk kzzzk kzk",
    "    k  kzzzk  k",
    "       kzzk",
    "      kzzk",
    "      kzk",
    "       k",
]

AERODACTYL_2 = [
    "",
    "",
    "",
    "        kkk",
    "  kkkk kzzzk kkkk",
    " kzzzzkzwzzzkzzzzk",
    "  kzzzzkzzzzzzzk",
    "   kzzkzwzwzkzk",
    "    kk kzzzk kk",
    "       kzzzk",
    "       kzzk",
    "      kzzk",
    "      kzk",
    "       k",
]

AERODACTYL_1 = [row[:16] for row in AERODACTYL_1]
AERODACTYL_2 = [row[:16] for row in AERODACTYL_2]

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

SLOWPOKE_TOP = [
    "",
    "",
    "   kkk",
    "  kqqqk",
    " kqqqqqk",
    " kqwkqqk",
    " kqqqqqqkkkkk",
    "  kcckqqqqqqqqk",
    "   kkqqqqqqqqqk",
    "     kqqqqqqqqk  k",
    "     kqqqqqqqqkkwk",
    "     kqqqqqqqqqqqk",
]

SLOWPOKE_1 = [row[:16] for row in SLOWPOKE_TOP + ["      kqk  kqk", "      kkk  kkk"]]
SLOWPOKE_2 = [row[:16] for row in SLOWPOKE_TOP + ["     kqk    kqk", "     kkk    kkk"]]

BALLOON = [
    "     kkkkkk",
    "   kkzzzzzzkk",
    "  kzzwzzzzzzzk",
    " kzzwzzzzzzzzzk",
    " kzzzzzkkzzzzzk",
    " kzzzzkwwkzzzzk",
    " kzzzzkwwkzzzzk",
    "  kzzzzkkzzzzk",
    "   kkzzzzzzkk",
    "     kkzzkk",
    "   kk  kk  kk",
    "  kbck kk kcbk",
    "  kcckkkkkkcck",
    "   kckekkekck",
    "   kccccccccck",
    "    kkkkkkkkk",
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


def arbok_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    for i in range(10):
        cx = 16 + (6 if (i // 2 + step) % 2 else -6) * (1 if i > 4 else 0)
        ellipse(grid, cx, 30 - i * 1.2, 5, 2.5, "z")
    ellipse(grid, 16, 14, 11, 9, "z")
    ellipse(grid, 16, 15, 7, 6, "y")
    ellipse(grid, 16, 15, 4.5, 4, "n")
    grid[13][14] = grid[13][18] = "k"
    grid[16][16] = "k"
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
    wing = 2 * step
    for side in (-1, 1):
        for i in range(12):
            x = 16 + side * (4 + i)
            top = 8 + i // 2 - wing
            for y in range(top, 18 + i // 4):
                if 0 <= x < 32 and 0 <= y < 32:
                    grid[y][x] = "b" if y < top + 5 else "x"
    ellipse(grid, 16, 17, 5.5, 7, "c")
    ellipse(grid, 16, 10, 4, 3.5, "b")
    for i, x in enumerate(range(12, 18)):
        grid[3 + (i % 2)][x] = "n"
        grid[2 + (i % 2)][x + 1] = "y"
    for x in range(18, 22):
        grid[11][x] = "y"
    grid[9][18] = "w" if charging else "k"
    for y in range(23, 31):
        for x in range(13 - (y - 23) // 2, 20 + (y - 23) // 2):
            grid[y][x] = "n" if (x + y) % 3 == 0 else "b"
    return ["".join(row) for row in outline(grid)]


def articuno_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    wing = 2 * step
    for side in (-1, 1):
        for i in range(12):
            x = 16 + side * (4 + i)
            top = 7 + i // 2 - wing
            for y in range(top, 16 + i // 4):
                if 0 <= x < 32 and 0 <= y < 32:
                    grid[y][x] = "i" if y > top + 1 else "w"
    ellipse(grid, 16, 16, 5, 6.5, "i")
    ellipse(grid, 16, 9, 3.5, 3.5, "i")
    for i, x in enumerate(range(14, 19)):
        grid[4 - (i % 2)][x] = "B"
    for x in range(18, 21):
        grid[10][x] = "s"
    grid[8][17] = "w" if charging else "k"
    for y in range(22, 31):
        for x in range(14 - (y - 22) // 3, 19 + (y - 22) // 3):
            grid[y][x] = "B"
    return ["".join(row) for row in outline(grid)]


def moltres_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    wing = 2 * step
    for side in (-1, 1):
        for i in range(12):
            x = 16 + side * (4 + i)
            top = 6 + i // 2 - wing
            for y in range(top, 17 + i // 3):
                if 0 <= x < 32 and 0 <= y < 32:
                    grid[y][x] = "n" if y < top + 3 else "o"
            if 0 <= x < 32 and top - 2 >= 0:
                grid[top - 1][x] = "y"
                if i % 2:
                    grid[top - 2][x] = "y"
    ellipse(grid, 16, 17, 5.5, 7, "y")
    ellipse(grid, 16, 9, 3.5, 3.5, "y")
    for i, x in enumerate(range(13, 20)):
        grid[4 - (i % 3)][x] = "n"
        grid[5][x] = "o"
    for x in range(18, 22):
        grid[10][x] = "s"
    grid[8][17] = "w" if charging else "k"
    for x in (14, 18):
        for y in range(24, 29):
            grid[y][x] = "s"
    for x in range(12, 21):
        grid[25 + (x % 3)][x] = "n"
    return ["".join(row) for row in outline(grid)]


def zapdos_frame(step, charging):
    grid = [["."] * 32 for _ in range(32)]
    wing = 2 * step
    for side in (-1, 1):
        for i in range(12):
            x = 16 + side * (4 + i)
            top = 8 + i // 2 - wing + (i % 3)
            for y in range(top, 18 + i // 3):
                if 0 <= x < 32 and 0 <= y < 32:
                    grid[y][x] = "y"
            if i % 3 == 0 and 0 <= x < 32:
                grid[top][x] = "k"
    ellipse(grid, 16, 17, 6, 7, "y")
    ellipse(grid, 16, 9, 4, 3.5, "y")
    for i, x in enumerate(range(13, 20)):
        grid[4 - (i % 2)][x] = "y"
    for x in range(17, 22):
        grid[10][x] = "o"
    grid[9][21] = "o"
    grid[8][17] = "w" if charging else "k"
    for y in (15, 18, 21):
        for x in range(13, 20):
            grid[y][x] = "k"
    for x in (13, 19):
        for y in range(24, 29):
            grid[y][x] = "o"
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
    floor_tile(False),
    wall_top_tile(),
]

FOREST_PALETTE = [
    (16, 24, 16),
    (104, 168, 72),
    (88, 144, 64),
    (136, 192, 96),
    (64, 112, 56),
    (32, 72, 40),
    (48, 104, 48),
    (112, 80, 48),
    (80, 56, 32),
    (144, 112, 64),
    (56, 96, 48),
    (232, 216, 96),
    (216, 88, 104),
    (240, 240, 232),
    (176, 136, 80),
    (24, 48, 24),
]


def forest_floor_tile(variant):
    tile = blank(1)
    for x, y in ((1, 2), (5, 5), (3, 6)):
        tile[y][x] = 2
    if variant == 1:
        tile[2][5] = 3
        tile[3][5] = 3
    return tile


def forest_shadow_tile():
    tile = blank(4)
    for x, y in ((1, 2), (5, 5)):
        tile[y][x] = 10
    return tile


def canopy_tile():
    tile = blank(5)
    for x, y in ((1, 1), (5, 2), (2, 5), (6, 6), (4, 4)):
        tile[y][x] = 6
    tile[0][3] = tile[3][0] = 15
    return tile


def trunk_tile():
    tile = blank(6)
    for y in range(3, 8):
        for x in (1, 2, 5, 6):
            tile[y][x] = 7 if x in (1, 5) else 8
    tile[0] = [5] * 8
    return tile


def vine_door_tile():
    tile = blank(15)
    for y in range(8):
        tile[y][(y * 3) % 8] = 6
        tile[y][(y * 3 + 4) % 8] = 5
    return tile


def forest_stairs_tiles():
    big = [[8] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 14
            big[top + 1][x] = 9
            big[top + 2][x] = 7
    return split_quad(big)


def flower_tile():
    tile = forest_floor_tile(0)
    tile[3][3] = 12
    tile[2][3] = tile[4][3] = tile[3][2] = tile[3][4] = 13
    return tile


def tall_grass_tile():
    tile = blank(2)
    for x in range(0, 8, 2):
        for y in range(1, 8):
            tile[y][x] = 3 if y < 3 else 6
        tile[0][x + 1] = 3
    return tile


def bush_tile():
    tile = blank(6)
    for x, y in ((1, 1), (4, 1), (2, 3), (5, 4), (1, 5), (6, 6), (3, 6)):
        tile[y][x] = 3
    tile[7] = [5] * 8
    return tile


CAVE_PALETTE = [
    (8, 8, 12),
    (96, 84, 72),
    (80, 70, 60),
    (120, 108, 92),
    (60, 52, 46),
    (40, 36, 36),
    (64, 58, 54),
    (104, 96, 88),
    (72, 66, 60),
    (136, 124, 112),
    (52, 46, 40),
    (24, 20, 20),
    (168, 152, 128),
    (200, 188, 168),
    (148, 116, 72),
    (32, 28, 28),
]


def cave_floor_tile(variant):
    tile = blank(1)
    for x, y in ((2, 1), (6, 4), (1, 6)):
        tile[y][x] = 2
    if variant:
        tile[3][4] = tile[3][5] = 3
        tile[4][4] = 12
    return tile


def cave_shadow_tile():
    tile = blank(4)
    tile[5][2] = tile[2][6] = 10
    return tile


def cave_wall_top_tile():
    tile = blank(5)
    for x, y in ((1, 2), (5, 1), (3, 5), (6, 6)):
        tile[y][x] = 6
    tile[0] = [15] * 8
    return tile


def cave_wall_face_tile():
    tile = blank(7)
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    for y in range(1, 7):
        tile[y][(y * 5) % 8] = 8
    tile[3][2] = tile[4][6] = 9
    return tile


def cave_door_tile():
    big = blank(5)
    for cx, cy in ((2, 2), (6, 3), (3, 6)):
        for dx in (-1, 0, 1):
            for dy in (-1, 0, 1):
                if 0 <= cx + dx < 8 and 0 <= cy + dy < 8:
                    big[cy + dy][cx + dx] = 9 if dx + dy < 0 else 7
    return big


def cave_stairs_tiles():
    big = [[11] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 13
            big[top + 1][x] = 12
            big[top + 2][x] = 3
    return split_quad(big)


def hole_tile():
    tile = cave_floor_tile(0)
    for y in range(2, 6):
        for x in range(2, 6):
            tile[y][x] = 11
    tile[2][2] = tile[2][5] = tile[5][2] = tile[5][5] = 10
    return tile


def boulder_tile():
    tile = blank(1)
    for y in range(1, 7):
        for x in range(1, 7):
            tile[y][x] = 7
    tile[1][2] = tile[2][1] = 9
    tile[6][5] = tile[5][6] = 8
    return tile


LAKE_PALETTE = [
    (8, 8, 16),
    (72, 84, 104),
    (60, 72, 92),
    (96, 112, 136),
    (44, 52, 68),
    (28, 32, 48),
    (48, 56, 80),
    (64, 80, 112),
    (44, 56, 84),
    (96, 120, 160),
    (36, 44, 60),
    (56, 96, 160),
    (40, 72, 152),
    (88, 136, 208),
    (200, 224, 248),
    (24, 28, 40),
]


def lake_floor_tile(variant):
    tile = blank(1)
    for x, y in ((1, 1), (5, 3), (2, 6)):
        tile[y][x] = 2
    if variant:
        tile[4][5] = tile[4][6] = 3
    return tile


def lake_shadow_tile():
    tile = blank(4)
    tile[2][2] = tile[5][6] = 10
    return tile


def lake_wall_top_tile():
    tile = blank(5)
    for x, y in ((2, 2), (6, 1), (1, 6), (5, 5)):
        tile[y][x] = 6
    return tile


def lake_wall_face_tile():
    tile = blank(7)
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    for y in range(1, 7):
        tile[y][(y * 3) % 8] = 8
    tile[2][5] = 11
    return tile


def lake_door_tile():
    tile = blank(5)
    for x in (1, 4, 6):
        for y in range(8):
            if y >= abs(x - 4):
                tile[y][x] = 9 if y < 4 else 7
    return tile


def lake_stairs_tiles():
    big = [[15] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 14
            big[top + 1][x] = 3
            big[top + 2][x] = 7
    return split_quad(big)


def puddle_tile():
    tile = lake_floor_tile(0)
    for y in range(3, 6):
        for x in range(1, 7):
            tile[y][x] = 11
    tile[3][2] = 13
    return tile


def water_tile():
    tile = blank(12)
    tile[2][1] = tile[2][2] = tile[1][3] = 13
    tile[6][5] = tile[6][6] = tile[5][7] = 13
    return tile


def flow_tile(dx, dy):
    tile = blank(12)
    for i in range(-2, 3):
        cx, cy = 3 - dx, 3 - dy
        if dx:
            tile[3 + i][cx + dx * (2 - abs(i))] = 14
        else:
            tile[cy + dy * (2 - abs(i))][3 + i] = 14
    tile[7][1] = tile[0][6] = 13
    return tile


PLANT_PALETTE = [
    (8, 8, 12),
    (136, 136, 120),
    (112, 112, 100),
    (168, 168, 152),
    (88, 88, 80),
    (40, 44, 48),
    (64, 72, 80),
    (96, 104, 112),
    (64, 68, 76),
    (136, 144, 152),
    (72, 72, 64),
    (232, 200, 48),
    (248, 240, 160),
    (240, 240, 232),
    (200, 80, 40),
    (32, 32, 36),
]


def plant_floor_tile(variant):
    tile = blank(1)
    for i in range(8):
        tile[0][i] = 3
        tile[i][0] = 3
        tile[7][i] = 2
        tile[i][7] = 2
    if variant:
        tile[3][3] = tile[4][4] = 2
    return tile


def plant_shadow_tile():
    tile = blank(4)
    for i in range(8):
        tile[7][i] = 10
        tile[i][7] = 10
    return tile


def plant_wall_top_tile():
    tile = blank(5)
    tile[0] = [6] * 8
    tile[4][2] = tile[4][5] = 6
    return tile


def plant_wall_face_tile():
    tile = blank(7)
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    for y in range(2, 6):
        tile[y][2] = tile[y][5] = 8
    tile[3][3] = 11
    return tile


def plant_door_tile():
    tile = blank(15)
    for y in range(8):
        tile[y][(y * 2) % 8] = 11
        tile[y][(y * 2 + 3) % 8] = 12
    return tile


def plant_stairs_tiles():
    big = [[15] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 13
            big[top + 1][x] = 3
            big[top + 2][x] = 7
    return split_quad(big)


def cable_tile():
    tile = plant_floor_tile(0)
    for x in range(8):
        tile[4][x] = 15
    tile[3][2] = tile[5][6] = 15
    return tile


def plate_tile(state):
    fill = {0: 10, 1: 11, 2: 12}[state]
    tile = blank(fill)
    for i in range(8):
        tile[0][i] = tile[7][i] = tile[i][0] = tile[i][7] = 15
    for x, y in ((2, 2), (5, 5), (2, 5), (5, 2)):
        tile[y][x] = 13 if state == 2 else 2
    return tile


VOLCANO_PALETTE = [
    (12, 6, 6),
    (72, 56, 56),
    (60, 46, 46),
    (96, 76, 72),
    (44, 32, 32),
    (28, 18, 18),
    (48, 32, 30),
    (88, 60, 52),
    (60, 40, 36),
    (120, 84, 72),
    (40, 28, 28),
    (200, 72, 32),
    (240, 144, 48),
    (248, 224, 120),
    (104, 40, 24),
    (20, 12, 12),
]


def volcano_floor_tile(variant):
    tile = blank(1)
    for x, y in ((2, 1), (6, 3), (1, 6), (5, 6)):
        tile[y][x] = 2
    if variant:
        tile[3][3] = tile[3][4] = 3
    return tile


def volcano_shadow_tile():
    tile = blank(4)
    tile[3][2] = tile[5][6] = 10
    return tile


def volcano_wall_top_tile():
    tile = blank(5)
    for x, y in ((1, 1), (5, 2), (3, 5), (6, 6)):
        tile[y][x] = 6
    return tile


def volcano_wall_face_tile():
    tile = blank(7)
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    for y in range(1, 7):
        tile[y][(y * 3 + 1) % 8] = 8
    tile[4][5] = 11
    tile[5][5] = 14
    return tile


def volcano_door_tile():
    tile = blank(14)
    for y in range(8):
        tile[y][(y * 5) % 8] = 11
        tile[y][(y * 5 + 2) % 8] = 12
    return tile


def volcano_stairs_tiles():
    big = [[15] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 9
            big[top + 1][x] = 3
            big[top + 2][x] = 7
    return split_quad(big)


def ember_crack_tile():
    tile = volcano_floor_tile(0)
    tile[3][2] = tile[4][3] = tile[4][4] = tile[5][5] = 11
    return tile


def lava_tile(state):
    if state == 0:
        tile = blank(10)
        tile[2][2] = tile[2][3] = tile[5][5] = tile[6][2] = 14
    elif state == 1:
        tile = blank(14)
        tile[2][2] = tile[2][3] = tile[3][4] = tile[5][5] = tile[6][2] = tile[6][3] = 11
    else:
        tile = blank(11)
        for x, y in ((1, 1), (5, 2), (2, 5), (6, 6), (4, 4)):
            tile[y][x] = 12
        tile[3][6] = tile[6][4] = 13
    return tile


ICE_PALETTE = [
    (8, 12, 20),
    (176, 200, 216),
    (152, 176, 200),
    (216, 232, 240),
    (120, 144, 168),
    (48, 64, 88),
    (72, 96, 128),
    (112, 144, 176),
    (80, 104, 136),
    (168, 200, 224),
    (104, 128, 152),
    (144, 208, 240),
    (200, 240, 255),
    (248, 252, 255),
    (88, 160, 208),
    (32, 40, 56),
]


def ice_floor_tile(variant):
    tile = blank(1)
    for x, y in ((2, 2), (6, 5), (1, 6)):
        tile[y][x] = 2
    if variant:
        tile[3][4] = tile[4][5] = 3
    return tile


def ice_shadow_tile():
    tile = blank(4)
    tile[2][3] = tile[6][6] = 10
    return tile


def ice_wall_top_tile():
    tile = blank(5)
    for x, y in ((1, 2), (5, 1), (3, 5), (6, 6)):
        tile[y][x] = 6
    return tile


def ice_wall_face_tile():
    tile = blank(7)
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    for y in range(1, 7):
        tile[y][(y * 3 + 2) % 8] = 8
    tile[2][2] = tile[3][2] = 12
    return tile


def ice_door_tile():
    tile = blank(14)
    for y in range(8):
        tile[y][(y * 3) % 8] = 12
        tile[y][(y * 3 + 5) % 8] = 11
    return tile


def ice_stairs_tiles():
    big = [[15] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 13
            big[top + 1][x] = 3
            big[top + 2][x] = 7
    return split_quad(big)


def frost_tile():
    tile = ice_floor_tile(0)
    tile[3][2] = tile[2][3] = tile[4][3] = tile[3][4] = 13
    return tile


def slippery_ice_tile():
    tile = blank(11)
    tile[1][2] = tile[1][3] = tile[2][1] = 13
    tile[5][5] = tile[6][4] = 12
    tile[4][1] = 14
    return tile


def ice_block_tile():
    tile = blank(12)
    for i in range(8):
        tile[0][i] = tile[i][0] = 13
        tile[7][i] = tile[i][7] = 14
    tile[2][2] = tile[3][3] = 13
    return tile


CHASM_PALETTE = [
    (16, 20, 48),
    (184, 160, 120),
    (160, 136, 100),
    (208, 188, 148),
    (136, 116, 88),
    (96, 80, 64),
    (120, 100, 80),
    (152, 128, 96),
    (112, 92, 72),
    (192, 168, 128),
    (120, 100, 76),
    (32, 40, 88),
    (56, 72, 136),
    (232, 240, 248),
    (176, 200, 232),
    (64, 52, 40),
]


def chasm_floor_tile(variant):
    tile = blank(1)
    for x, y in ((1, 2), (5, 5), (3, 6)):
        tile[y][x] = 2
    if variant:
        tile[2][5] = tile[3][5] = 3
    return tile


def chasm_shadow_tile():
    tile = blank(4)
    tile[3][3] = tile[6][6] = 10
    return tile


def chasm_wall_top_tile():
    tile = blank(5)
    for x, y in ((2, 1), (6, 3), (1, 5), (4, 6)):
        tile[y][x] = 6
    return tile


def chasm_wall_face_tile():
    tile = blank(7)
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    for y in range(1, 7):
        tile[y][(y * 5 + 1) % 8] = 8
    return tile


def chasm_door_tile():
    tile = blank(12)
    for y in range(0, 8, 2):
        for x in range(8):
            if (x + y // 2) % 3 == 0:
                tile[y][x] = 13
    return tile


def chasm_stairs_tiles():
    big = [[15] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 13
            big[top + 1][x] = 3
            big[top + 2][x] = 7
    return split_quad(big)


def feather_tile():
    tile = chasm_floor_tile(0)
    tile[2][5] = tile[3][4] = tile[4][3] = 13
    tile[5][2] = 14
    return tile


def pit_tile():
    tile = blank(11)
    tile[1][2] = tile[5][5] = tile[3][6] = 12
    return tile


def wind_tile(dx, dy):
    tile = chasm_floor_tile(0)
    for i in range(3):
        if dx:
            tile[1 + i * 3][(2 + i * 2) % 8] = 14
            tile[1 + i * 3][(3 + i * 2) % 8] = 13
            tile[1 + i * 3][(4 + i * 2) % 8] = 14 if dx > 0 else 13
        else:
            tile[(2 + i * 2) % 8][1 + i * 3] = 14
            tile[(3 + i * 2) % 8][1 + i * 3] = 13
            tile[(4 + i * 2) % 8][1 + i * 3] = 14 if dy > 0 else 13
    return tile


HIDEOUT_PALETTE = [
    (10, 8, 12),
    (104, 104, 112),
    (88, 88, 96),
    (132, 132, 140),
    (64, 64, 72),
    (40, 32, 40),
    (80, 40, 48),
    (112, 56, 64),
    (80, 40, 48),
    (152, 80, 88),
    (56, 56, 64),
    (208, 48, 48),
    (240, 240, 232),
    (152, 88, 184),
    (200, 144, 224),
    (24, 20, 28),
]


def hideout_floor_tile(variant):
    tile = blank(1)
    for i in range(8):
        tile[7][i] = 2
        tile[i][7] = 2
    if variant:
        tile[0][0] = tile[0][1] = tile[1][0] = 3
    return tile


def hideout_shadow_tile():
    tile = blank(4)
    for i in range(8):
        tile[7][i] = 10
    return tile


def hideout_wall_top_tile():
    tile = blank(5)
    tile[0] = [6] * 8
    tile[4][1] = tile[4][6] = 6
    return tile


def hideout_wall_face_tile():
    tile = blank(7)
    tile[0] = [9] * 8
    tile[7] = [8] * 8
    for y in range(2, 6):
        tile[y][1] = tile[y][6] = 8
    return tile


def hideout_door_tile():
    tile = blank(10)
    for y in range(8):
        tile[y][0] = tile[y][7] = 4
        if y % 2:
            for x in range(1, 7):
                tile[y][x] = 2
    tile[3][3] = tile[3][4] = 11
    return tile


def hideout_stairs_tiles():
    big = [[15] * 16 for _ in range(16)]
    for step in range(4):
        top = 2 + step * 3
        for x in range(1 + step, 15 - step):
            big[top][x] = 12
            big[top + 1][x] = 3
            big[top + 2][x] = 1
    return split_quad(big)


def rocket_logo_tile():
    tile = hideout_floor_tile(0)
    for y in range(1, 7):
        tile[y][2] = 11
    tile[1][3] = tile[1][4] = tile[2][5] = tile[3][4] = tile[3][3] = 11
    tile[4][4] = tile[5][5] = tile[6][5] = 11
    return tile


def arrow_tile(dx, dy):
    tile = blank(4)
    for i in range(8):
        tile[0][i] = tile[7][i] = tile[i][0] = tile[i][7] = 10
    for i in range(-2, 3):
        if dx:
            tile[3 + i][3 - dx + dx * (2 - abs(i))] = 12
            tile[3 + i][4 - dx + dx * (2 - abs(i))] = 12
        else:
            tile[3 - dy + dy * (2 - abs(i))][3 + i] = 12
            tile[4 - dy + dy * (2 - abs(i))][3 + i] = 12
    return tile


def vent_tile(state):
    tile = blank({0: 4, 1: 13, 2: 14}[state])
    for i in range(8):
        tile[0][i] = tile[7][i] = tile[i][0] = tile[i][7] = 10
    for x in (2, 4, 6):
        for y in range(2, 6):
            tile[y][x] = 5 if state == 0 else 13
    if state == 2:
        tile[1][3] = tile[2][5] = tile[6][2] = 12
    return tile


HIDEOUT_TILES = [
    blank(0),
    hideout_floor_tile(0),
    hideout_floor_tile(1),
    hideout_shadow_tile(),
    hideout_wall_top_tile(),
    hideout_wall_face_tile(),
    hideout_door_tile(),
    *hideout_stairs_tiles(),
    rocket_logo_tile(),
    hideout_floor_tile(0),
    hideout_wall_top_tile(),
    hideout_floor_tile(0),
    arrow_tile(1, 0),
    arrow_tile(-1, 0),
    arrow_tile(0, 1),
    arrow_tile(0, -1),
    vent_tile(0),
    vent_tile(1),
    vent_tile(2),
]

CHASM_TILES = [
    blank(0),
    chasm_floor_tile(0),
    chasm_floor_tile(1),
    chasm_shadow_tile(),
    chasm_wall_top_tile(),
    chasm_wall_face_tile(),
    chasm_door_tile(),
    *chasm_stairs_tiles(),
    feather_tile(),
    chasm_floor_tile(0),
    chasm_wall_top_tile(),
    pit_tile(),
    wind_tile(1, 0),
    wind_tile(-1, 0),
    wind_tile(0, 1),
    wind_tile(0, -1),
]

ICE_TILES = [
    blank(0),
    ice_floor_tile(0),
    ice_floor_tile(1),
    ice_shadow_tile(),
    ice_wall_top_tile(),
    ice_wall_face_tile(),
    ice_door_tile(),
    *ice_stairs_tiles(),
    frost_tile(),
    slippery_ice_tile(),
    ice_block_tile(),
]

VOLCANO_TILES = [
    blank(0),
    volcano_floor_tile(0),
    volcano_floor_tile(1),
    volcano_shadow_tile(),
    volcano_wall_top_tile(),
    volcano_wall_face_tile(),
    volcano_door_tile(),
    *volcano_stairs_tiles(),
    ember_crack_tile(),
    volcano_floor_tile(0),
    volcano_wall_top_tile(),
    *[volcano_floor_tile(0)] * 5,
    lava_tile(0),
    lava_tile(1),
    lava_tile(2),
]

PLANT_TILES = [
    blank(0),
    plant_floor_tile(0),
    plant_floor_tile(1),
    plant_shadow_tile(),
    plant_wall_top_tile(),
    plant_wall_face_tile(),
    plant_door_tile(),
    *plant_stairs_tiles(),
    cable_tile(),
    plant_floor_tile(0),
    plant_wall_top_tile(),
    *[plant_floor_tile(0)] * 5,
    plate_tile(0),
    plate_tile(1),
    plate_tile(2),
]

LAKE_TILES = [
    blank(0),
    lake_floor_tile(0),
    lake_floor_tile(1),
    lake_shadow_tile(),
    lake_wall_top_tile(),
    lake_wall_face_tile(),
    lake_door_tile(),
    *lake_stairs_tiles(),
    puddle_tile(),
    lake_floor_tile(0),
    lake_wall_top_tile(),
    water_tile(),
    flow_tile(1, 0),
    flow_tile(-1, 0),
    flow_tile(0, 1),
    flow_tile(0, -1),
]

CAVE_TILES = [
    blank(0),
    cave_floor_tile(0),
    cave_floor_tile(1),
    cave_shadow_tile(),
    cave_wall_top_tile(),
    cave_wall_face_tile(),
    cave_door_tile(),
    *cave_stairs_tiles(),
    hole_tile(),
    cave_floor_tile(0),
    boulder_tile(),
]

FOREST_TILES = [
    blank(0),
    forest_floor_tile(0),
    forest_floor_tile(1),
    forest_shadow_tile(),
    canopy_tile(),
    trunk_tile(),
    vine_door_tile(),
    *forest_stairs_tiles(),
    flower_tile(),
    tall_grass_tile(),
    bush_tile(),
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
    save_sprite_sheet("water_projectiles", [BUBBLE, WATER_DROP, STAR, DRAGON_FIRE, ICE_SHARD, HEART, SLUDGE], 8)
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
    save_sprite_sheet("projectiles", [SPIT, ENEMY_SHOT, IMPACT, COIN, TRI, PSYBEAM, LEAF, NEEDLE, STRING, BEAM,
                                      ROCK, SUPERSONIC], 8)
    save_sprite_sheet("slash", [SLASH], 16)
    save_wave()
    save_hp_bar()
    save_text_box()
    save_tiles("lab", LAB_TILES, LAB_PALETTE)
    save_tiles("forest", FOREST_TILES, FOREST_PALETTE)
    save_tiles("cave", CAVE_TILES, CAVE_PALETTE)
    save_tiles("lake", LAKE_TILES, LAKE_PALETTE)
    save_tiles("plant", PLANT_TILES, PLANT_PALETTE)
    save_tiles("volcano", VOLCANO_TILES, VOLCANO_PALETTE)
    save_tiles("ice", ICE_TILES, ICE_PALETTE)
    save_tiles("chasm", CHASM_TILES, CHASM_PALETTE)
    save_tiles("hideout", HIDEOUT_TILES, HIDEOUT_PALETTE)
    save_tiles("overlay", OVERLAY_TILES, OVERLAY_PALETTE)


if __name__ == "__main__":
    main()
