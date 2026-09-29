from common import split_block, split_pair

CHASM_PALETTE = [
    (20, 18, 34),
    (204, 180, 138),
    (226, 206, 166),
    (178, 152, 114),
    (140, 114, 86),
    (164, 176, 104),
    (124, 140, 80),
    (126, 96, 70),
    (92, 68, 52),
    (160, 126, 92),
    (50, 38, 34),
    (14, 14, 34),
    (40, 42, 76),
    (250, 250, 244),
    (206, 222, 234),
    (84, 92, 136),
]


def _grid(fill, height=16):
    return [[fill] * 16 for _ in range(height)]


def _floor():
    block = _grid(1)
    for x, y in ((1, 1), (2, 1), (9, 4), (10, 4), (5, 8), (13, 10), (14, 10), (3, 13), (8, 14)):
        block[y][x] = 3
    for x, y in ((2, 2), (10, 5), (14, 11)):
        block[y][x] = 4
    for x, y in ((1, 0), (9, 3), (13, 9), (6, 7)):
        block[y][x] = 2
    return block


def _floor_detail():
    block = _floor()
    rock = [
        ".kk.",
        "k2mk",
        "km7k",
        ".kk.",
    ]
    key = {"k": 4, "2": 2, "m": 9, "7": 7}
    for y, row in enumerate(rock):
        for x, c in enumerate(row):
            if c != ".":
                block[5 + y][5 + x] = key[c]
    for x, y in ((10, 8), (11, 8)):
        block[y][x] = 4
    block[7][10] = 2
    for x, y, value in ((12, 12, 5), (13, 12, 6), (12, 13, 6), (11, 13, 5), (13, 13, 5)):
        block[y][x] = value
    return block


def _shadow():
    strip = [row[:] for row in _floor()[:8]]
    for x in range(16):
        strip[0][x] = 7
        strip[1][x] = 4
        if (x + 1) % 3:
            strip[2][x] = 3
    return strip


def _wall_top():
    block = _grid(5)
    for y in range(16):
        for x in range(16):
            if (x * 5 + y * 3) % 17 == 0:
                block[y][x] = 6
    for cx, cy in ((4, 4), (12, 12)):
        for dx, dy in ((0, 0), (1, 0), (-1, 1), (0, 1), (1, 1), (2, 1)):
            block[(cy + dy) % 16][(cx + dx) % 16] = 6
    for x, y in ((9, 7), (2, 10), (14, 2)):
        block[y][x] = 2
    return block


def _face(upper):
    strip = _grid(7, 8)
    for y in range(8):
        for x in range(16):
            if (x + (y // 3) * 5) % 8 == 0:
                strip[y][x] = 8
            elif (x + (y // 3) * 5) % 8 == 1:
                strip[y][x] = 9
    if upper:
        for x in range(16):
            strip[0][x] = 6
            strip[1][x] = 10 if x % 7 == 3 else 9
    else:
        for x in range(16):
            strip[6][x] = 8
            strip[7][x] = 10
    return strip


def _door():
    block = _grid(15)
    for y in range(16):
        for x in range(16):
            if (x + y * 2) % 16 in (0, 1, 2):
                block[y][x] = 14
            elif (x + y * 2) % 16 == 3:
                block[y][x] = 13
            elif (x - y) % 8 == 0:
                block[y][x] = 12
    return block


def _stairs():
    block = _grid(8)
    for step in range(4):
        top = step * 4
        for x in range(1, 15):
            block[top][x] = 2
            block[top + 1][x] = 1
            block[top + 2][x] = 9
            block[top + 3][x] = 7
    for y in range(16):
        block[y][0] = block[y][15] = 10
    return block


def _pit():
    block = _grid(11)
    for x, y in ((3, 2), (12, 5), (7, 11), (14, 14)):
        block[y][x] = 12
    block[9][1] = 15
    return block


def _wind(dx, dy):
    block = _floor()
    for y, start in ((3, 0), (11, 8)):
        for step in range(11):
            x = (start + step) % 16
            lift = 1 if 3 <= step <= 6 else 0
            block[(y - lift) % 16][x] = 13 if 2 <= step <= 8 else 14
        block[(y - 1) % 16][(start + 9) % 16] = 13
        block[(y - 2) % 16][(start + 8) % 16] = 14
        block[(y + 1) % 16][(start + 9) % 16] = 13
        block[(y + 2) % 16][(start + 8) % 16] = 14
    if dx < 0:
        block = [row[::-1] for row in block]
    if dy:
        block = [list(row) for row in zip(*block)]
        if dy < 0:
            block = block[::-1]
    return block


def chasm_tileset():
    floor = split_block(_floor())
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(_floor_detail())
    tiles += split_pair(_shadow())
    tiles += split_block(_wall_top())
    tiles += split_pair(_face(True))
    tiles += split_pair(_face(False))
    tiles += split_block(_door())
    tiles += split_block(_stairs())
    tiles += floor
    tiles += floor
    tiles += split_block(_pit())
    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        tiles += split_block(_wind(dx, dy))
    for _ in range(3):
        tiles += floor
    assert len(tiles) == 67
    return tiles, CHASM_PALETTE
