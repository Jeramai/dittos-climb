from common import split_block, split_pair, from_chars

LAB_PALETTE = [
    (16, 16, 24),
    (32, 32, 48),
    (240, 232, 208),
    (216, 208, 184),
    (184, 176, 160),
    (56, 64, 88),
    (80, 96, 120),
    (224, 232, 240),
    (184, 200, 216),
    (88, 136, 200),
    (104, 112, 136),
    (192, 184, 164),
    (152, 160, 176),
    (104, 112, 128),
    (232, 192, 64),
    (200, 80, 72),
]

LAB_KEY = {"k": 1, "f": 2, "F": 3, "g": 4, "T": 5, "t": 6, "w": 7, "W": 8, "b": 9, "B": 10, "s": 11, "m": 12,
           "M": 13, "y": 14, "r": 15}

LAB_FLOOR = [
    "ffffffffffffffFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "FFFFFFFFFFFFFFFg",
    "gggggggggggggggg",
]

LAB_CRACK = [
    "ffffffffffffffFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFgFFFFFFFFFg",
    "fFFFFFgFFFFFFFFg",
    "fFFFFFFgFFFFFFFg",
    "fFFFFFFgFFFFFFFg",
    "fFFFFFFFgFFFFFFg",
    "fFFFFFFgFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFfFFg",
    "fFFFFFFFFFFFFFFg",
    "FFFFFFFFFFFFFFFg",
    "gggggggggggggggg",
]

LAB_SHADOW = [
    "gggggggggggggggg",
    "ssssssssssssssss",
    "ssssssssssssssss",
    "sFFFsFFFsFFFsFFF",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
    "fFFFFFFFFFFFFFFg",
]

LAB_WALL_TOP = [
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTtTTTTTTTTTtTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTtTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTT",
]

LAB_FACE_HIGH = [
    "kkkkkkkkkkkkkkkk",
    "tttttttttttttttt",
    "wwwwwwwwwwwwwwwW",
    "wwwwwwwwwwwwwwwW",
    "wwwwwwwwwwwwwwwW",
    "wwwwwwwwwwwwwwwW",
    "wwwwwwwwwwwwwwwW",
    "bbbbbbbbbbbbbbbb",
]

LAB_FACE_LOW = [
    "WWWWWWWWWWWWWWWW",
    "wwwwwwwwwwwwwwwW",
    "wwwwwwwwwwwwwwwW",
    "wwwwwwwwwwwwwwwW",
    "WWWWWWWWWWWWWWWW",
    "BBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBB",
    "kkkkkkkkkkkkkkkk",
]

LAB_SHUTTER = [
    "kkkkkkkkkkkkkkkk",
    "mmmmmmmmmmmmmmmm",
    "MMMMMMMMMMMMMMMM",
    "mmmmmmmmmmmmmmmm",
    "MMMMMMMMMMMMMMMM",
    "mmmmmmmmmmmmmmmm",
    "MMMMMMMMMMMMMMMM",
    "mmmmmmmmmmmmmmmm",
    "MMMMMMMMMMMMMMMM",
    "yykkyykkyykkyykk",
    "ykkyykkyykkyykky",
    "kkyykkyykkyykkyy",
    "MMMMMMMMMMMMMMMM",
    "mmmmmmmmmmmmmmmm",
    "MMMMMMMMMMMMMMMM",
    "kkkkkkkkkkkkkkkk",
]

LAB_STAIRS = [
    "kkkkkkkkkkkkkkkk",
    "kTTTTTTTTTTTTTTk",
    "kTTTTTTTTTTTTTTk",
    "kMffffffffffffMk",
    "kMggggggggggggMk",
    "kMssssssssssssMk",
    "kmffffffffffffmk",
    "kmFFFFFFFFFFFFmk",
    "kmggggggggggggmk",
    "kmffffffffffffmk",
    "kmFFFFFFFFFFFFmk",
    "kmggggggggggggmk",
    "kmffffffffffffmk",
    "kmFFFFFFFFFFFFmk",
    "kmggggggggggggmk",
    "kkkkkkkkkkkkkkkk",
]


def lab_tileset():
    key = LAB_KEY
    floor = split_block(from_chars(LAB_FLOOR, key))
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(from_chars(LAB_CRACK, key))
    tiles += split_pair(from_chars(LAB_SHADOW, key))
    tiles += split_block(from_chars(LAB_WALL_TOP, key))
    tiles += split_pair(from_chars(LAB_FACE_HIGH, key))
    tiles += split_pair(from_chars(LAB_FACE_LOW, key))
    tiles += split_block(from_chars(LAB_SHUTTER, key))
    tiles += split_block(from_chars(LAB_STAIRS, key))
    tiles += floor * 10
    return tiles, list(LAB_PALETTE)
