from common import split_block, split_pair, from_chars

FOREST_PALETTE = [
    (16, 32, 24),
    (24, 40, 24),
    (48, 96, 48),
    (80, 144, 64),
    (112, 184, 80),
    (160, 216, 112),
    (32, 72, 48),
    (208, 240, 144),
    (136, 200, 96),
    (128, 88, 48),
    (80, 56, 32),
    (224, 72, 72),
    (248, 248, 232),
    (248, 208, 72),
    (208, 168, 104),
    (152, 112, 64),
]

FOREST_KEY = {"k": 1, "D": 2, "m": 3, "g": 4, "l": 5, "T": 6, "L": 7, "h": 8, "b": 9, "B": 10, "r": 11,
              "w": 12, "y": 13, "p": 14, "s": 15}

FOREST_FLOOR = [
    "gggggggggggggggg",
    "gggggggggggggggg",
    "gggggggggggggggg",
    "gglggggggggglggg",
    "gmlmggggggggmlmg",
    "gggggggggggggggg",
    "gggggggggggggggg",
    "gggggglggggggggg",
    "gggggmlmgggggggg",
    "gggggggggggggggg",
    "gggggggggggggggg",
    "gggggggggggglggg",
    "ggglggggggggmlmg",
    "ggmlmggggggggggg",
    "gggggggggggggggg",
    "gggggggggggggggg",
]

FOREST_FLOWERS = [
    "gggggggggggggggg",
    "gggggggggggggggg",
    "gggrgggggggggggg",
    "ggryrggggggggggg",
    "gggrmggggggglggg",
    "ggggmgggggggmlmg",
    "gggggggggggggggg",
    "gggggggggggggggg",
    "gggggggggggggggg",
    "ggggggggggwggggg",
    "gglggggggwywgggg",
    "gmlmggggggwmgggg",
    "gggggggggggmgggg",
    "gggggggggggggggg",
    "gggggggggggggggg",
    "gggggggggggggggg",
]

FOREST_SHADOW = [
    "DDDDDDDDDDDDDDDD",
    "DmDDDmDDDDmDDDmD",
    "mmmmmmmmmmmmmmmm",
    "mgmmmgmmmmgmmmgm",
    "gggggggggggggggg",
    "gggggggggggggggg",
    "gggglggggggggggg",
    "gggmlmgggggggggg",
]

FOREST_CANOPY = [
    "TTTTkkkkkkkkTTTT",
    "TTkkhhhhmmmmkkTT",
    "Tkhhhhhhhmmmmmkk",
    "kkhhLhhmmhhmmmDk",
    "khhLLhhmmhhhmmDk",
    "khhhhhmmmhhmmmDk",
    "kmhhmmmmhhhmmDDk",
    "kmmmmhhmmhhmmmDk",
    "kmmmhhhmmmmmmDDk",
    "kmmmhhmmmmhhmmDk",
    "kDmmmmmmmhhmmDDk",
    "kDmmhhmmmmmmDDDk",
    "kkDmmmmmmmmDDDkk",
    "TkDDDmmmmDDDDDkT",
    "TTkkDDDDDDDDkkTT",
    "TTTTkkkkkkkkTTTT",
]

FOREST_TRUNKS = [
    "TTTTkkkkkkkkTTTT",
    "TTTTTTkbBkTTTTTT",
    "TTTTTTkbBkTTTTTT",
    "TTTTTTkbBkTTTTTT",
    "TTTTTTkbBkTTTTTT",
    "TTTTTkbbbBkTTTTT",
    "DDDDkbbkbBBkDDDD",
    "mmmmmkkmkkkmmmmm",
]

FOREST_VINES = [
    "TTTDTTTTTTTTmTTT",
    "TTTTDTTTTTTmTTTT",
    "TTTTTmTTTTmhmTTT",
    "TmTTTTmTTDTmTTTT",
    "TmhTTTTmDTTTTTTT",
    "TmTTTTTDmTTTTTDT",
    "TTDTTTDTTmTTTDTT",
    "TTTDTDTTTTmTDTTT",
    "TTTTmTTTTTTmTTTT",
    "TTTDTDTTTTDTmTTT",
    "TTDTTTmTTDTTTmTT",
    "TDTTTTTmDTTTTTmT",
    "DTTTmTTDmTTTTmhm",
    "TTTmhmDTTmTTTTmT",
    "TTTTmDTTTTmTTTTT",
    "TTTTDTTTTTTmTTTT",
]

FOREST_STAIRS = [
    "ggkkkkkkkkkkkkgg",
    "gkBBBBBBBBBBBBkg",
    "gkBkkkkkkkkkkBkg",
    "gkBkppppppppkBkg",
    "gkBkssssssssBBkg",
    "gkkkkkkkkkkkkkkg",
    "gkkppppppppppkkg",
    "gkksssssssssskkg",
    "gkkkkkkkkkkkkkkg",
    "gkppppppppppppkg",
    "gkssssssssssssBk",
    "kkkkkkkkkkkkkkkk",
    "kppppppppppppppk",
    "kppppppppppppppk",
    "ksssssssssssssBk",
    "mkkkkkkkkkkkkkkm",
]

FOREST_TALL_GRASS = [
    "DlDDDDDlDlDDDDDl",
    "mlmDDDmlmlmDDDml",
    "mLmmDmmLmLmmDmmL",
    "mmmmmmmmmmmmmmmm",
    "DmmmDDDmmmmDDDmm",
    "DDmDDDDDmDDDDDmD",
    "DDDDDDDDDDDDDDDD",
    "DDDDlDDDDDDDlDDD",
    "DDDmlmDDDDDmlmDD",
    "DDmmLmmDDDmmLmmD",
    "mmmmmmmmmmmmmmmm",
    "mmDDDmmmmmmDDDmm",
    "mDDDDDmDDmDDDDDm",
    "DDDDDDDDDDDDDDDD",
    "lDDDDDDlDlDDDDDD",
    "mlDDDDDmlmDDDDDm",
]

FOREST_CUT_TREE = [
    "gggggkkkkkkggggg",
    "gggkkhhhmmmkkggg",
    "ggkhhhLhhmmmDkgg",
    "gkhhLLhhmmmmmDkg",
    "gkhhhhhmmhhmmDkg",
    "kmhhmmmmhhmmmDDk",
    "kmmmhhmmmmmmDDDk",
    "kDmmmmmmmhmmDDDk",
    "gkDmmhmmmmmDDDkg",
    "gkkDDmmmmDDDDkkg",
    "ggkkkkDDDDkkkkgg",
    "gggggkbbBBkggggg",
    "gggggkbbBBkggggg",
    "ggggkbbbbBBkgggg",
    "gggmmkkkkkkmmggg",
    "gggggmmmmmmggggg",
]


def forest_tileset():
    key = FOREST_KEY
    floor = split_block(from_chars(FOREST_FLOOR, key))
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(from_chars(FOREST_FLOWERS, key))
    tiles += split_pair(from_chars(FOREST_SHADOW, key))
    tiles += split_block(from_chars(FOREST_CANOPY, key))
    tiles += split_pair(from_chars(FOREST_CANOPY[8:], key))
    tiles += split_pair(from_chars(FOREST_TRUNKS, key))
    tiles += split_block(from_chars(FOREST_VINES, key))
    tiles += split_block(from_chars(FOREST_STAIRS, key))
    tiles += split_block(from_chars(FOREST_TALL_GRASS, key))
    tiles += split_block(from_chars(FOREST_CUT_TREE, key))
    tiles += floor * 8
    return tiles, list(FOREST_PALETTE)
