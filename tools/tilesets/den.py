from common import split_block, split_pair

DEN_PALETTE = [
    (8, 14, 22),
    (116, 132, 128),
    (100, 116, 114),
    (140, 156, 150),
    (72, 86, 88),
    (58, 72, 84),
    (86, 102, 112),
    (78, 92, 106),
    (52, 62, 78),
    (28, 36, 50),
    (48, 96, 176),
    (72, 132, 208),
    (128, 184, 236),
    (232, 244, 252),
    (96, 144, 100),
    (60, 104, 76),
]


def _noise(x, y, seed):
    value = (x * 374761393 + y * 668265263 + seed * 2246822519) & 0xffffffff
    value = ((value ^ (value >> 13)) * 1274126177) & 0xffffffff
    return (value ^ (value >> 16)) & 0xff


def _block(function, height=16):
    return [[function(x, y) for x in range(16)] for y in range(height)]


def _floor(x, y):
    n = _noise(x // 4, y // 4, 1)
    fine = _noise(x, y, 2)
    if fine < 5:
        return 4
    if fine > 250:
        return 3
    if n < 70 and (x + y) % 2 == 0:
        return 2
    return 1


def _detail(x, y):
    rocks = {(4, 6): 9, (5, 6): 9, (6, 6): 9, (3, 7): 9, (7, 7): 9, (3, 8): 9, (7, 8): 9, (4, 9): 9, (5, 9): 9,
             (6, 9): 9, (4, 7): 3, (5, 7): 3, (6, 7): 6, (4, 8): 6, (5, 8): 6, (6, 8): 5}
    if (x, y) in rocks:
        return rocks[(x, y)]
    if (x - 11) ** 2 + (y - 11) ** 2 * 2 <= 7:
        return 15 if _noise(x, y, 9) < 110 else 14
    return _floor(x, y)


def _shadow(x, y):
    if y < 2:
        return 8 if y == 0 else 4
    if y == 2:
        return 4 if (x + y) % 2 else 2
    return _floor(x, y)


_BUMPS = [(4, 4, 4.2), (12, 3, 3.6), (9, 11, 4.4), (1, 12, 3.4), (16, 11, 3.0)]


def _wall_top(x, y):
    for cx, cy, r in _BUMPS:
        for ox in (-16, 0, 16):
            for oy in (-16, 0, 16):
                dx, dy = x - cx - ox, y - cy - oy
                d = (dx * dx + dy * dy) ** 0.5
                if d <= r:
                    if d > r - 1:
                        return 9 if dx + dy > 0 else 8
                    return 6 if dx + dy < -r * 0.6 else 5
    return 8


_RIBS = [0, 5, 9, 14]


def _face(x, y, upper):
    starts = [r for r in _RIBS if r <= x]
    start = starts[-1]
    end = next((r for r in _RIBS if r > x), 16)
    offset, width = x - start, end - start
    if upper and y == 0:
        return 9
    if not upper and y >= 6:
        return 9 if y == 7 else 8
    if offset == 0:
        return 9 if upper and y < 3 else 8
    if offset == 1:
        return 6
    if offset >= width - 1:
        return 8
    crack = (x * 3 + y * 5 + (0 if upper else 3)) % 11 == 0
    return 8 if crack else 7


def _water(x, y, seed=3):
    wave = (x + 2 * (y // 4)) % 8
    if y % 4 == 1 and wave in (1, 2, 3):
        return 12
    if y % 4 == 2 and wave == 4:
        return 12
    return 10 if _noise(x // 4, y // 2, seed) < 70 else 11


def _flow(dx, dy):
    def function(x, y):
        u, v = (x, y) if dx else (y, x)
        sign = dx or dy
        along = u if sign > 0 else 15 - u
        across = v % 8
        if abs(across - 4) <= 2 and (along + abs(across - 4) * 2) % 8 in (0, 1):
            return 13 if abs(across - 4) < 2 else 12
        return _water(x, y, 4)
    return function


def _waterfall(x, y):
    lane = (x * 5) % 7
    if (y + lane * 3) % 8 < 3:
        return 13 if x % 3 else 12
    return 12 if x % 4 == 1 else 11


def _whirlpool(x, y):
    dx, dy = x - 7.5, y - 7.5
    r = (dx * dx + dy * dy) ** 0.5
    if r > 7.6:
        return _water(x, y)
    import math
    angle = math.atan2(dy, dx)
    band = (r * 0.9 + angle * 1.6 / math.pi) % 2
    if r < 1.5:
        return 10
    return 13 if band < 0.45 else 12 if band < 0.9 else 10 if r < 4 else 11


def _surge(x, y):
    lane = (x * 3) % 5
    if (y + lane * 2) % 6 < 2:
        return 13
    return 12 if (x + y) % 3 == 0 else 11


def _stairs(x, y):
    if x in (0, 15):
        return 9
    step = y // 4
    inset = step
    if x < 1 + inset // 2 or x > 14 - inset // 2:
        return 5
    row = y % 4
    return 3 if row == 0 else 1 if row == 1 else 2 if row == 2 else 8


def den_tileset():
    floor = _block(_floor)
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += split_block(floor)
    tiles += split_block(_block(_detail))
    tiles += split_pair(_block(_shadow, 8))
    tiles += split_block(_block(_wall_top))
    tiles += split_pair(_block(lambda x, y: _face(x, y, True), 8))
    tiles += split_pair(_block(lambda x, y: _face(x, y, False), 8))
    tiles += split_block(_block(_surge))
    tiles += split_block(_block(_stairs))
    tiles += split_block(floor)
    tiles += split_block(floor)
    tiles += split_block(_block(_water))
    tiles += split_block(_block(_flow(1, 0)))
    tiles += split_block(_block(_flow(-1, 0)))
    tiles += split_block(_block(_waterfall))
    tiles += split_block(_block(_flow(0, -1)))
    whirlpool = split_block(_block(_whirlpool))
    tiles += whirlpool * 3
    return tiles, DEN_PALETTE
