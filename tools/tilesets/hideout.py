from common import split_block, split_pair

HIDEOUT_PALETTE = [
    (14, 14, 24),
    (138, 148, 172),
    (166, 176, 198),
    (112, 120, 146),
    (76, 84, 110),
    (50, 58, 90),
    (84, 94, 132),
    (66, 80, 116),
    (36, 42, 66),
    (206, 70, 70),
    (22, 22, 38),
    (244, 204, 68),
    (190, 142, 40),
    (160, 92, 188),
    (210, 162, 226),
    (236, 238, 246),
]


def _grid(fill, height=16):
    return [[fill] * 16 for _ in range(height)]


def _floor():
    block = _grid(1)
    for i in range(16):
        block[0][i] = 2
        block[i][0] = 2
        block[15][i] = 3
        block[i][15] = 3
    block[15][0] = block[0][15] = 1
    for x, y in ((2, 2), (13, 2), (2, 13), (13, 13)):
        block[y][x] = 4
        block[y - 1][x] = 2
    return block


def _floor_detail():
    block = _floor()
    for y in range(4, 12):
        for x in range(4, 12):
            block[y][x] = 3
    for x in range(4, 12):
        block[4][x] = 2
        block[11][x] = 4
    for y in range(4, 12):
        block[y][4] = 2
        block[y][11] = 4
    for x in range(6, 10):
        block[7][x] = 4
        block[8][x] = 2
    return block


def _shadow():
    strip = [row[:] for row in _floor()[:8]]
    for x in range(16):
        strip[0][x] = 8
        strip[1][x] = 4
        strip[2][x] = 3
    return strip


def _wall_top():
    block = _grid(5)
    for i in range(16):
        block[7][i] = 6
        block[8][i] = 8
        block[i][7] = 6 if i < 7 else 5
        block[i][15] = 8
    for x, y in ((3, 3), (11, 3), (3, 12), (11, 12)):
        block[y][x] = 6
    return block


def _face(upper):
    strip = _grid(7, 8)
    for y in range(8):
        strip[y][7] = 8
        strip[y][8] = 6
    if upper:
        for x in range(16):
            strip[0][x] = 8
            strip[1][x] = 9
            strip[2][x] = 9 if x % 4 else 8
            strip[3][x] = 8
        for x in (2, 12):
            strip[5][x] = 6
    else:
        for x in (2, 12):
            strip[2][x] = 6
        for x in range(16):
            strip[5][x] = 4
            strip[6][x] = 8
            strip[7][x] = 10
    return strip


def _door():
    block = _grid(3)
    for y in range(16):
        if y % 4 == 0:
            for x in range(16):
                block[y][x] = 8
        elif y % 4 == 1:
            for x in range(16):
                block[y][x] = 2
    for y in (6, 7):
        for x in range(16):
            block[y][x] = 11 if (x + y) % 8 < 4 else 10
    return block


def _stairs():
    block = _grid(8)
    for step in range(4):
        top = step * 4
        for x in range(2, 14):
            block[top][x] = 15
            block[top + 1][x] = 2
            block[top + 2][x] = 3
            block[top + 3][x] = 4
    for y in range(16):
        block[y][0] = block[y][15] = 10
        block[y][1] = block[y][14] = 9
    return block


def _spinner(dx, dy):
    arrow = [
        "...k....",
        "...kyk..",
        "kkkkyyk.",
        "kyyyyyyk",
        "kGGGGGGk",
        "kkkkGGk.",
        "...kGk..",
        "...k....",
    ]
    key = {"k": 10, "y": 11, "G": 12}
    mark = [[key.get(c) for c in row] for row in arrow]
    if dx < 0:
        mark = [row[::-1] for row in mark]
    if dy:
        mark = [list(row) for row in zip(*mark)]
        if dy < 0:
            mark = mark[::-1]
    block = _floor()
    for y0 in (0, 8):
        for x0 in (0, 8):
            for y in range(8):
                for x in range(8):
                    if mark[y][x] is not None:
                        block[y0 + y][x0 + x] = mark[y][x]
    return block


def _vent(phase):
    block = _floor()
    for y in range(3, 13):
        for x in range(3, 13):
            edge = x in (3, 12) or y in (3, 12)
            block[y][x] = 10 if edge else (8 if y % 2 else 4)
    if phase == 1:
        for x in range(4, 12):
            block[4][x] = 13
            block[11][x] = 13
        for y in range(4, 12):
            block[y][4] = 13
            block[y][11] = 13
    if phase == 2:
        for cx, cy, r in ((5, 6, 4), (10, 5, 4), (8, 10, 5), (3, 11, 3), (13, 11, 3)):
            for y in range(16):
                for x in range(16):
                    d = (x - cx) ** 2 + (y - cy) ** 2
                    if d <= r * r:
                        block[y][x] = 14 if d <= (r - 2) ** 2 else 13
    return block


def hideout_tileset():
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
    tiles += floor
    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        tiles += split_block(_spinner(dx, dy))
    for phase in range(3):
        tiles += split_block(_vent(phase))
    assert len(tiles) == 67
    return tiles, HIDEOUT_PALETTE
