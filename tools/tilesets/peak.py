from common import split_block, split_pair

PEAK_PALETTE = [
    (10, 8, 24),
    (128, 116, 164),
    (114, 102, 150),
    (152, 140, 188),
    (90, 80, 124),
    (64, 54, 104),
    (94, 84, 138),
    (86, 74, 126),
    (58, 48, 92),
    (30, 24, 54),
    (64, 144, 184),
    (120, 208, 232),
    (208, 248, 255),
    (192, 96, 192),
    (240, 168, 232),
    (176, 168, 204),
]


def _noise(x, y, seed):
    value = (x * 374761393 + y * 668265263 + seed * 2246822519) & 0xffffffff
    value = ((value ^ (value >> 13)) * 1274126177) & 0xffffffff
    return (value ^ (value >> 16)) & 0xff


def _block(function, height=16):
    return [[function(x, y) for x in range(16)] for y in range(height)]


def _floor(x, y):
    n = _noise(x // 4, y // 4, 11)
    fine = _noise(x, y, 12)
    if fine < 5:
        return 4
    if fine > 251:
        return 3
    if n < 70 and (x + y) % 2 == 0:
        return 2
    return 1


_CRYSTAL = [
    "................",
    "................",
    "........k.......",
    ".......kLk......",
    "......kkLck.....",
    ".....kLkcdk.....",
    ".....kcdkdk.....",
    "......kkkk......",
]


def _crystal(x, y, base):
    row = y - 4
    if 0 <= row < len(_CRYSTAL):
        c = _CRYSTAL[row][x]
        if c != ".":
            return {"k": 9, "L": 12, "c": 11, "d": 10}[c]
    return base(x, y)


def _detail(x, y):
    return _crystal(x, y, _floor)


def _shadow(x, y):
    if y < 2:
        return 8 if y == 0 else 4
    if y == 2:
        return 4 if (x + y) % 2 else 2
    return _floor(x, y)


_BUMPS = [(4, 4, 4.2), (12, 3, 3.6), (9, 11, 4.4), (1, 12, 3.4), (16, 11, 3.0)]


def _wall_top(x, y):
    if 11 <= x <= 13 and 9 <= y <= 13:
        spike = {(12, 9): 12, (11, 10): 9, (12, 10): 12, (13, 10): 9, (11, 11): 9, (12, 11): 11, (13, 11): 10,
                 (11, 12): 9, (12, 12): 11, (13, 12): 10, (11, 13): 9, (12, 13): 9, (13, 13): 9}
        if (x, y) in spike:
            return spike[(x, y)]
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
    start = [r for r in _RIBS if r <= x][-1]
    end = next((r for r in _RIBS if r > x), 16)
    offset, width = x - start, end - start
    if upper and y == 0:
        return 9
    if not upper and y >= 6:
        return 9 if y == 7 else 8
    if offset == 0:
        return 8
    if offset == 1:
        return 6
    if offset >= width - 1:
        return 8
    if upper and x == 11 and 2 <= y <= 4:
        return 11 if y > 2 else 12
    crack = (x * 3 + y * 5 + (0 if upper else 3)) % 11 == 0
    return 8 if crack else 7


def _stairs(x, y):
    if x in (0, 15):
        return 9
    step = y // 4
    if x < 1 + step // 2 or x > 14 - step // 2:
        return 5
    row = y % 4
    return 3 if row == 0 else 1 if row == 1 else 2 if row == 2 else 8


def _barrier(x, y):
    d = abs((x + y) % 8 - 4)
    if d == 0:
        return 12
    if d == 1:
        return 14
    return 13 if (x + 2 * y) % 5 else 9


def _warp(glow):
    def function(x, y):
        dx, dy = x - 7.5, y - 7.5
        r = (dx * dx + dy * dy) ** 0.5
        if r > 7.5:
            return _floor(x, y)
        if r > 6.6:
            return 9
        if r > 5.4:
            return 15 if dx + dy < 0 else 4
        if r > 4.4:
            return 9
        if r > 2.4:
            return glow[0] if (x + y) % 2 else glow[1]
        return glow[2]
    return function


def peak_tileset():
    floor = split_block(_block(_floor))
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(_block(_detail))
    tiles += split_pair(_block(_shadow, 8))
    tiles += split_block(_block(_wall_top))
    tiles += split_pair(_block(lambda x, y: _face(x, y, True), 8))
    tiles += split_pair(_block(lambda x, y: _face(x, y, False), 8))
    tiles += split_block(_block(_barrier))
    tiles += split_block(_block(_stairs))
    tiles += floor * 7
    warp = split_block(_block(_warp((11, 10, 12))))
    tiles += warp * 3
    return tiles, PEAK_PALETTE
