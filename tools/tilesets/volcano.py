from common import split_block, split_pair

VOLCANO_PALETTE = [
    (14, 8, 8),
    (150, 110, 92),
    (124, 88, 74),
    (176, 138, 116),
    (92, 64, 56),
    (116, 92, 88),
    (86, 66, 64),
    (60, 44, 44),
    (28, 18, 18),
    (132, 72, 52),
    (176, 44, 28),
    (236, 116, 36),
    (250, 200, 72),
    (255, 244, 184),
    (54, 38, 36),
    (170, 160, 156),
]

FLOOR, FLOOR_SHADE, FLOOR_LIGHT, PEBBLE = 1, 2, 3, 4
ROCK_LIGHT, ROCK, ROCK_SHADE, OUTLINE = 5, 6, 7, 8
STRATA, LAVA_RED, LAVA_ORANGE, LAVA_YELLOW, LAVA_WHITE, CRUST, ASH = 9, 10, 11, 12, 13, 14, 15


def volcano_grid(width, height, fill):
    return [[fill] * width for _ in range(height)]


def volcano_speckle(grid, seed, color, count):
    state = seed
    for _ in range(count):
        state = (state * 1103515245 + 12345) & 0x7fffffff
        x = (state >> 8) % len(grid[0])
        state = (state * 1103515245 + 12345) & 0x7fffffff
        y = (state >> 8) % len(grid)
        grid[y][x] = color


def volcano_floor():
    grid = volcano_grid(16, 16, FLOOR)
    volcano_speckle(grid, 7, FLOOR_SHADE, 14)
    volcano_speckle(grid, 41, FLOOR_LIGHT, 6)
    volcano_speckle(grid, 93, ASH, 4)
    return grid


def volcano_floor_detail():
    grid = volcano_floor()
    crack = [(3, 4), (4, 5), (5, 5), (6, 6), (7, 7), (8, 7), (9, 8), (10, 9), (11, 9), (12, 10)]
    for x, y in crack:
        grid[y][x] = CRUST
        grid[y + 1][x] = FLOOR_SHADE
    for x, y in ((6, 6), (9, 8)):
        grid[y][x] = LAVA_RED
    for x, y in ((12, 3), (13, 3), (12, 4), (2, 12), (3, 12)):
        grid[y][x] = PEBBLE
    return grid


def volcano_shadow():
    grid = volcano_floor()[:8]
    for x in range(16):
        grid[0][x] = PEBBLE
        grid[1][x] = FLOOR_SHADE
        grid[2][x] = FLOOR_SHADE if x % 2 else grid[2][x]
    return grid


def volcano_wall_top():
    grid = volcano_grid(16, 16, ROCK)
    peaks = ((3, 3), (11, 2), (7, 10), (14, 11))
    for y in range(16):
        for x in range(16):
            best = None
            for px, py in peaks:
                dx = (x - px + 8) % 16 - 8
                dy = (y - py + 8) % 16 - 8
                distance = dx * dx + dy * dy
                if best is None or distance < best[0]:
                    best = (distance, dx, dy)
            distance, dx, dy = best
            if distance <= 2:
                grid[y][x] = ROCK_LIGHT
            elif dx + dy < -2 and distance < 10:
                grid[y][x] = ROCK_LIGHT
            elif dx + dy > 3:
                grid[y][x] = ROCK_SHADE
            if distance > 22:
                grid[y][x] = CRUST
    return grid


def volcano_face_high():
    grid = volcano_grid(16, 8, ROCK)
    for x in range(16):
        grid[0][x] = ROCK_LIGHT if x % 3 else ROCK
        grid[3][x] = STRATA
        grid[4][x] = STRATA if x % 4 else ROCK_SHADE
    for x in (2, 9, 13):
        for y in range(1, 8):
            grid[y][x] = ROCK_SHADE
    grid[6][5] = grid[6][6] = LAVA_RED
    return grid


def volcano_face_low():
    grid = volcano_grid(16, 8, ROCK_SHADE)
    for x in range(16):
        grid[1][x] = STRATA if (x + 1) % 5 else ROCK_SHADE
        grid[2][x] = STRATA
        grid[5][x] = CRUST
        grid[6][x] = CRUST if x % 2 else OUTLINE
        grid[7][x] = OUTLINE
    for x in (2, 9, 13):
        for y in range(0, 5):
            grid[y][x] = CRUST
    return grid


def volcano_lava(phase):
    grid = volcano_grid(16, 16, CRUST if phase < 2 else LAVA_ORANGE)
    lines = [(x, (3 + (x // 4) % 2) % 16) for x in range(16)] + [(x, (11 - (x // 5) % 2) % 16) for x in range(16)]
    lines += [((5 + (y // 4) % 2) % 16, y) for y in range(4, 11)] + [((12 - (y // 3) % 2) % 16, y) for y in range(11, 20)]
    if phase == 0:
        for x, y in lines:
            grid[y % 16][x] = LAVA_RED
        volcano_speckle(grid, 5, ROCK_SHADE, 10)
    elif phase == 1:
        for x, y in lines:
            grid[y % 16][x] = LAVA_ORANGE
            grid[(y + 1) % 16][x] = LAVA_RED
        for x, y in lines[::5]:
            grid[y % 16][x] = LAVA_YELLOW
    else:
        for x, y in lines:
            grid[y % 16][x] = LAVA_RED
        for cx, cy in ((3, 7), (11, 5), (8, 13), (14, 14)):
            for dx in (-1, 0, 1):
                grid[cy][(cx + dx) % 16] = LAVA_YELLOW
            grid[cy][cx] = LAVA_WHITE
            grid[(cy - 1) % 16][cx] = LAVA_YELLOW
    return grid


def volcano_door():
    grid = volcano_lava(2)
    for x in range(16):
        grid[0][x] = CRUST if x % 3 else LAVA_RED
        grid[15][x] = LAVA_RED
    return grid


def volcano_stairs():
    grid = volcano_grid(16, 16, ROCK_SHADE)
    for step in range(4):
        top = step * 4
        left, right = 1 + step, 15 - step
        for x in range(left, right):
            grid[top][x] = FLOOR_LIGHT
            grid[top + 1][x] = FLOOR
            grid[top + 2][x] = FLOOR_SHADE
            grid[top + 3][x] = CRUST
    for y in range(16):
        grid[y][0] = grid[y][15] = OUTLINE
    return grid


def volcano_tileset():
    floor = split_block(volcano_floor())
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(volcano_floor_detail())
    tiles += split_pair(volcano_shadow())
    tiles += split_block(volcano_wall_top())
    tiles += split_pair(volcano_face_high())
    tiles += split_pair(volcano_face_low())
    tiles += split_block(volcano_door())
    tiles += split_block(volcano_stairs())
    tiles += floor * 7
    for phase in range(3):
        tiles += split_block(volcano_lava(phase))
    return tiles, VOLCANO_PALETTE
