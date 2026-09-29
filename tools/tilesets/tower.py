from common import split_block, split_pair

TOWER_PALETTE = [
    (8, 6, 14),
    (112, 100, 136),
    (90, 78, 114),
    (134, 122, 158),
    (74, 64, 98),
    (44, 36, 64),
    (84, 70, 112),
    (104, 92, 132),
    (70, 60, 96),
    (24, 18, 36),
    (184, 178, 196),
    (140, 134, 156),
    (248, 208, 112),
    (255, 248, 224),
    (160, 104, 228),
    (224, 200, 255),
]


def _noise(x, y, seed):
    value = (x * 374761393 + y * 668265263 + seed * 2246822519) & 0xffffffff
    value = ((value ^ (value >> 13)) * 1274126177) & 0xffffffff
    return (value ^ (value >> 16)) & 0xff


def _block(function, height=16):
    return [[function(x, y) for x in range(16)] for y in range(height)]


def _floor(x, y):
    tx, ty = x % 8, y % 8
    if tx == 7 or ty == 7:
        return 2
    if tx == 0 or ty == 0:
        return 3
    if (x // 8 + y // 8) % 2 and tx in (2, 3) and ty == 5:
        return 2
    return 1


_CANDLE = {(7, 3): 13, (7, 4): 12, (6, 5): 12, (7, 5): 13, (8, 5): 12, (6, 6): 9, (7, 6): 10, (8, 6): 9,
           (6, 7): 9, (7, 7): 10, (8, 7): 9, (6, 8): 9, (7, 8): 11, (8, 8): 9, (5, 9): 9, (6, 9): 9, (7, 9): 9,
           (8, 9): 9, (9, 9): 9}


def _detail(x, y):
    if (x, y) in _CANDLE:
        return _CANDLE[(x, y)]
    if (x, y) in ((11, 10), (12, 11), (12, 12), (13, 13), (11, 12)):
        return 4
    return _floor(x, y)


def _shadow(x, y):
    if y < 2:
        return 9 if y == 0 else 4
    if y == 2:
        return 4 if x % 2 else 2
    return _floor(x, y)


def _wall_top(x, y):
    d = (abs((x % 8) - 3.5) + abs((y % 8) - 3.5))
    if d > 3.6:
        return 9
    if d > 2.6:
        return 6
    return 5


def _face_upper(x, y):
    if y == 0:
        return 6
    if x % 8 in (0, 7):
        return 8 if x % 8 == 7 else 7
    if x % 8 == 1:
        return 3
    if x % 16 == 11 and 2 <= y <= 3:
        return 13 if y == 2 else 12
    if x % 16 in (10, 12) and y == 4:
        return 9
    if x % 16 == 11 and y == 4:
        return 10
    return 7 if y < 6 else 8


def _face_lower(x, y):
    if y == 7:
        return 9
    if y == 6:
        return 8
    if y == 0:
        return 6
    if x % 8 in (0, 7):
        return 8
    return 7 if y < 3 else 8 if x % 4 == 0 else 7


def _spirit(x, y):
    t = (x * 2 + (y * y) // 5) % 10
    if t == 0:
        return 15
    if t in (1, 9):
        return 14
    if (x + y) % 2 == 0 and t in (2, 8):
        return 14
    return 5 if (x // 4 + y // 4) % 2 else 9


def _chill(x, y):
    t = (x + 16 - y) % 8
    if t == 0:
        return 15
    if t == 1:
        return 14
    return 6 if (x + y) % 2 else 5


def _stairs(x, y):
    if x in (0, 15):
        return 9
    step = y // 4
    if x < 1 + step // 2 or x > 14 - step // 2:
        return 5
    row = y % 4
    return 3 if row == 0 else 1 if row == 1 else 2 if row == 2 else 9


def tower_tileset():
    floor = split_block(_block(_floor))
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(_block(_detail))
    tiles += split_pair(_block(_shadow, 8))
    tiles += split_block(_block(_wall_top))
    tiles += split_pair(_block(_face_upper, 8))
    tiles += split_pair(_block(_face_lower, 8))
    tiles += split_block(_block(_chill))
    tiles += split_block(_block(_stairs))
    tiles += floor
    tiles += split_block(_block(_spirit))
    tiles += floor * 8
    return tiles, TOWER_PALETTE
