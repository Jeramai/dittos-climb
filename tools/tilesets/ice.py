from common import split_block, split_pair

ICE_PALETTE = [
    (16, 24, 40),
    (184, 206, 222),
    (206, 224, 236),
    (160, 184, 206),
    (136, 160, 190),
    (234, 242, 250),
    (190, 210, 232),
    (108, 132, 172),
    (76, 94, 136),
    (146, 170, 204),
    (40, 48, 82),
    (156, 210, 240),
    (244, 252, 255),
    (104, 166, 222),
    (128, 190, 232),
    (60, 116, 176),
]


def _grid(fill, height=16):
    return [[fill] * 16 for _ in range(height)]


def _floor():
    block = _grid(1)
    for x, y in ((2, 3), (3, 3), (10, 1), (13, 6), (6, 9), (7, 9), (11, 12), (1, 13), (14, 14)):
        block[y][x] = 3
    for x, y in ((3, 4), (7, 10), (14, 7), (11, 13)):
        block[y][x] = 4
    for x, y in ((9, 5), (4, 12), (15, 2)):
        block[y][x] = 2
    return block


def _floor_detail():
    block = _floor()
    for x, y, value in ((5, 5, 4), (6, 5, 3), (7, 5, 3), (5, 6, 3), (6, 6, 2), (7, 6, 4), (8, 6, 3),
                        (6, 7, 4), (7, 7, 4), (10, 9, 12), (11, 9, 11), (10, 10, 11), (12, 10, 12),
                        (3, 11, 11), (4, 11, 12)):
        block[y][x] = value
    return block


def _shadow():
    strip = [row[:] for row in _floor()[:8]]
    for x in range(16):
        strip[0][x] = 8
        strip[1][x] = 4
        if x % 3:
            strip[2][x] = 3
    return strip


def _wall_top():
    block = _grid(6)
    for cx, cy in ((3, 2), (11, 6), (6, 11), (14, 13)):
        for dx in range(-2, 3):
            for dy in range(-1, 1):
                x, y = (cx + dx) % 16, (cy + dy) % 16
                block[y][x] = 5
        block[(cy + 1) % 16][cx % 16] = 9
        block[(cy + 1) % 16][(cx + 1) % 16] = 9
    for x, y in ((8, 3), (1, 8), (12, 10)):
        block[y][x] = 9
    return block


def _face(upper):
    strip = _grid(7, 8)
    for y in range(8):
        for x in range(16):
            if x in (3, 11) or (x in (7, 15) and y > 2):
                strip[y][x] = 8
            elif x in (4, 12):
                strip[y][x] = 9
    if upper:
        for x in range(16):
            strip[0][x] = 6 if x % 4 else 5
            strip[1][x] = 9 if x % 5 else 7
    else:
        for x in range(16):
            strip[6][x] = 8
            strip[7][x] = 10
    return strip


def _door():
    block = _grid(14)
    for y in range(16):
        for x in range(16):
            if (x + y) % 8 == 0:
                block[y][x] = 12
            elif (x + y) % 8 == 1:
                block[y][x] = 11
            elif (x - y) % 16 == 5:
                block[y][x] = 13
    for x in range(16):
        block[15][x] = 15
    return block


def _stairs():
    block = _grid(8)
    for step in range(4):
        top = step * 4
        for x in range(1, 15):
            block[top][x] = 12
            block[top + 1][x] = 9
            block[top + 2][x] = 7
            block[top + 3][x] = 8
        block[top + 1][1] = block[top + 2][1] = 5
    for y in range(16):
        block[y][0] = block[y][15] = 10
    return block


def _ice_patch():
    block = _grid(11)
    for y in range(16):
        for x in range(16):
            if (x + y) % 16 in (0, 1):
                block[y][x] = 12
            elif (x + y) % 16 == 9:
                block[y][x] = 5
            elif (x - y) % 16 == 12 and y % 2:
                block[y][x] = 14
    for x, y in ((5, 4), (13, 12)):
        block[y][x] = 12
    return block


def _frozen_block():
    block = _grid(14)
    for x in range(16):
        block[0][x] = 15
        block[8][x] = 15
    for y in range(1, 8):
        block[y][4] = 15
    for y in range(9, 16):
        block[y][12] = 15
    for x0, y0 in ((5, 1), (13, 9)):
        for x in range(x0, x0 + 6):
            block[y0][x % 16] = 12
        block[y0 + 1][x0 % 16] = 12
    for x0, y0 in ((0, 9), (8, 1)):
        for x in range(x0, x0 + 3):
            block[y0 + 5][x % 16] = 11
    for x, y in ((7, 4), (8, 5), (1, 12), (2, 13), (14, 3)):
        block[y][x] = 11
    return block


def ice_tileset():
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
    tiles += split_block(_ice_patch())
    tiles += split_block(_frozen_block())
    for _ in range(8):
        tiles += floor
    assert len(tiles) == 67
    return tiles, ICE_PALETTE
