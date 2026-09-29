from common import split_block, split_pair

DOJO_PALETTE = [
    (20, 12, 8),
    (212, 168, 112),
    (190, 146, 94),
    (232, 196, 142),
    (146, 102, 62),
    (86, 52, 32),
    (128, 84, 52),
    (240, 232, 208),
    (206, 194, 166),
    (44, 26, 16),
    (112, 72, 44),
    (188, 180, 168),
    (146, 138, 130),
    (100, 94, 90),
    (176, 184, 112),
    (136, 148, 84),
]

_SEAMS = (5, 13, 9, 1)


def _block(function, height=16):
    return [[function(x, y) for x in range(16)] for y in range(height)]


def _floor(x, y):
    band, row = y // 4, y % 4
    if row == 3:
        return 2
    if x == _SEAMS[band]:
        return 4 if row == 1 else 2
    if row == 0:
        return 3 if (x + band * 3) % 6 else 1
    if row == 2 and (x * 5 + band) % 9 < 2:
        return 2
    return 1


_KNOT = {(10, 5): 2, (11, 5): 4, (12, 5): 2, (10, 6): 4, (12, 6): 4}


def _detail(x, y):
    if (x, y) in _KNOT:
        return _KNOT[(x, y)]
    if y == 11 and 3 <= x <= 7:
        return 4 if x in (3, 7) else 2
    return _floor(x, y)


def _shadow(x, y):
    if y == 0:
        return 9
    if y == 1:
        return 4
    if y == 2:
        return 2 if x % 2 else 4
    return _floor(x, y)


def _wall_top(x, y):
    if y % 8 == 2:
        return 6
    if y % 8 == 7:
        return 9
    if y % 8 == 3 and x % 4 == 0:
        return 10
    return 5


def _face_upper(x, y):
    if y == 0 or x % 8 == 0:
        return 10
    if y == 7:
        return 10
    if x % 8 == 4 or y == 3:
        return 8
    return 7


def _face_lower(x, y):
    if y == 7:
        return 9
    if y == 0 or y == 6 or x % 8 == 0:
        return 10
    if y == 1:
        return 5
    return 6


def _door(x, y):
    if x % 8 in (0, 7) or y % 8 == 0:
        return 10
    if x % 8 == 4 or y % 8 == 4:
        return 6
    return 8


def _rock(x, y):
    dx, dy = x - 7.5, y - 8.5
    r = (dx * dx / 1.1 + dy * dy) ** 0.5
    if r > 7.4:
        return _floor(x, y)
    if r > 6.5:
        return 9
    cracks = {(6, 4), (7, 5), (7, 6), (8, 7), (8, 8), (9, 9), (7, 8), (6, 9), (10, 10), (11, 10), (5, 10)}
    if (x, y) in cracks:
        return 9
    if dx + dy < -4:
        return 11
    if dx + dy > 4:
        return 13
    return 12


def _stairs(x, y):
    if x in (0, 15):
        return 9
    step = y // 4
    if x < 1 + step // 2 or x > 14 - step // 2:
        return 10
    row = y % 4
    return 3 if row == 0 else 1 if row == 1 else 4 if row == 2 else 9


def dojo_tileset():
    floor = split_block(_block(_floor))
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(_block(_detail))
    tiles += split_pair(_block(_shadow, 8))
    tiles += split_block(_block(_wall_top))
    tiles += split_pair(_block(_face_upper, 8))
    tiles += split_pair(_block(_face_lower, 8))
    tiles += split_block(_block(_door))
    tiles += split_block(_block(_stairs))
    tiles += floor
    tiles += split_block(_block(_rock))
    tiles += floor * 8
    return tiles, DOJO_PALETTE
