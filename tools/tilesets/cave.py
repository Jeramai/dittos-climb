from common import split_block, split_pair, from_chars

CAVE_PALETTE = [
    (16, 12, 8),
    (40, 28, 20),
    (152, 120, 88),
    (176, 144, 104),
    (128, 96, 72),
    (200, 168, 120),
    (168, 132, 92),
    (120, 88, 64),
    (88, 64, 48),
    (144, 108, 76),
    (64, 44, 32),
    (112, 84, 64),
    (136, 128, 120),
    (184, 176, 160),
    (92, 86, 80),
    (216, 192, 152),
]

CAVE_KEY = {"k": 1, "f": 2, "F": 3, "d": 4, "H": 5, "t": 6, "r": 7, "R": 8, "q": 9, "c": 10, "s": 11,
            "o": 12, "O": 13, "x": 14, "S": 15}

CAVE_FLOOR = [
    "ffffffffffffffff",
    "ffFfffffffffffff",
    "ffffffffffdfffff",
    "fffffffffffffFff",
    "fffffdffffffffff",
    "ffffffffffffffff",
    "ffFfffffffFfffff",
    "fffffffffffffffd",
    "ffffffffffffffff",
    "ffffffdfffffffff",
    "ffffffffffffFfff",
    "fFffffffffffffff",
    "ffffffffffffffff",
    "ffffffffFffdffff",
    "fffdffffffffffff",
    "ffffffffffffffff",
]

CAVE_PEBBLES = [
    "ffffffffffffffff",
    "ffFfffffffffffff",
    "ffffffffffffffff",
    "ffffffFFffffffff",
    "fffffFFFdfffffff",
    "fffffdddffffFfff",
    "ffffffffffffffff",
    "fffffffffffFffff",
    "ffffffffffFFdfff",
    "ffFfffffffddffff",
    "ffffffffffffffff",
    "fffffffffffffFff",
    "ffffFfffffffffff",
    "fffFFdffffffffff",
    "ffffddffffffffff",
    "ffffffffffffffff",
]

CAVE_SHADOW = [
    "cccccccccccccccc",
    "ssssssssssssssss",
    "sdssssdssssdssss",
    "dddddddddddddddd",
    "dfddfdddfddfdddf",
    "ffffffffffffffff",
    "ffffffFfffffffff",
    "ffffffffffffffdf",
]


CAVE_FACE_HIGH = [
    "kkkkkkkkkkkkkkkk",
    "HqqqqqqHqqqqqqqq",
    "rrqrrrrrRrrqrrrr",
    "rrrrRrrrRrrrrrRr",
    "rRrrRrrrrrrRrrRr",
    "rRrrrrqrrrrRrrrr",
    "rrrrrrqrrRrrrrrr",
    "rrRrrrrrrRrrrqrr",
]

CAVE_FACE_LOW = [
    "rrRrrrrrrrrrrqrr",
    "rrRrrqrrrRrrrrrr",
    "rrrrrqrrrRrrRrrr",
    "RrrrrrrrrrrrRrrR",
    "RRRRRRRRRRRRRRRR",
    "RRcRRRRRcRRRRRcR",
    "cccccccccccccccc",
    "kkkkkkkkkkkkkkkk",
]

CAVE_BOULDERS = [
    "cckkkkkcckkkkkcc",
    "ckOOOookkOOOookc",
    "kOOoooooxkOooooxk"[:16],
    "kOoooooxxkoooooxk"[:16],
    "koooooxxxkooooxxk"[:16],
    "kxooooxxkkkxxxxkc",
    "ckxxxxxkOOOkkkkcc",
    "cckkkkkOOoooookcc",
    "ckOOokOoooooooxkc",
    "kOoooOooooooooxxk",
    "koooxoooooooooxxk",
    "kooxxkoooooooxxxk",
    "kxxxxkxooooxxxxkc",
    "ckxxkckxxxxxxxkcc",
    "cckkccckkkkkkkkcc",
    "cccccccccccccccc",
]

CAVE_STAIRS = [
    "kkkkkkkkkkkkkkkk",
    "kccccccccccccccc",
    "kccccccccccccccc",
    "kRSSSSSSSSSSSSRk",
    "kRFFFFFFFFFFFFRk",
    "kRddddddddddddRk",
    "kRSSSSSSSSSSSSRk",
    "kRFFFFFFFFFFFFRk",
    "kRddddddddddddRk",
    "kRSSSSSSSSSSSSRk",
    "kRFFFFFFFFFFFFRk",
    "kRddddddddddddRk",
    "kSSSSSSSSSSSSSSk",
    "kFFFFFFFFFFFFFFk",
    "kddddddddddddddk",
    "kkkkkkkkkkkkkkkk",
]

CAVE_ROCK = [
    "ffffffffffffffff",
    "fffffkkkkkkfffff",
    "fffkkOOOoookkfff",
    "ffkOOOooooooxkff",
    "fkOOoooooooooxkf",
    "fkOoooooooooxxkf",
    "kOoooooooooooxxk",
    "kooooooooooooxxk",
    "koooooooooooxxxk",
    "kxooooooooooxxxk",
    "kxxoooooooxxxxkf",
    "fkxxxxooxxxxxxkf",
    "fkkxxxxxxxxxxkkf",
    "fdskkkkkkkkkksdf",
    "ffddssssssssddff",
    "ffffffffffffffff",
]


CAVE_LUMPS = [(4, 4, 4.6), (12.5, 5, 4.4), (8, 12.5, 4.8), (0, 12.5, 4.2), (16, 12.5, 4.2), (4, 20, 4.6),
              (12.5, 21, 4.4), (8, -3.5, 4.8), (4, -12, 4.6)]


def cave_wall_top():
    rows = []
    for y in range(16):
        row = ""
        for x in range(16):
            best = None
            for cx, cy, radius in CAVE_LUMPS:
                for ox in (-16, 0, 16):
                    for oy in (-16, 0, 16):
                        dx, dy = x + 0.5 - (cx + ox), y + 0.5 - (cy + oy)
                        value = (dx * dx + dy * dy) ** 0.5 / radius
                        if best is None or value < best[0]:
                            best = (value, dx / radius, dy / radius)
            value, dx, dy = best
            if value > 1:
                row += "c"
            elif value > 0.84:
                row += "k"
            else:
                light = -(dx + dy)
                row += "H" if light > 0.55 else "t" if light > -0.35 else "q"
        rows.append(row)
    return rows


def cave_tileset():
    key = CAVE_KEY
    floor = split_block(from_chars(CAVE_FLOOR, key))
    boulders = [(row + "cccccccccccccccc")[:16] for row in CAVE_BOULDERS]
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(from_chars(CAVE_PEBBLES, key))
    tiles += split_pair(from_chars(CAVE_SHADOW, key))
    tiles += split_block(from_chars(cave_wall_top(), key))
    tiles += split_pair(from_chars(CAVE_FACE_HIGH, key))
    tiles += split_pair(from_chars(CAVE_FACE_LOW, key))
    tiles += split_block(from_chars(boulders, key))
    tiles += split_block(from_chars(CAVE_STAIRS, key))
    tiles += floor
    tiles += split_block(from_chars(CAVE_ROCK, key))
    tiles += floor * 8
    return tiles, list(CAVE_PALETTE)
