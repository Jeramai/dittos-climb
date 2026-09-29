from common import split_block, split_pair

LAKE_PALETTE = [
    (14, 18, 30),
    (178, 170, 146),
    (154, 146, 122),
    (202, 194, 170),
    (118, 110, 94),
    (140, 150, 170),
    (106, 116, 138),
    (78, 86, 106),
    (34, 38, 52),
    (64, 116, 200),
    (44, 84, 164),
    (124, 172, 232),
    (224, 238, 250),
    (56, 62, 80),
    (92, 70, 50),
    (134, 104, 74),
]

FLOOR, FLOOR_SHADE, FLOOR_LIGHT, PEBBLE = 1, 2, 3, 4
ROCK_LIGHT, ROCK, ROCK_SHADE, OUTLINE = 5, 6, 7, 8
WATER, DEEP, RIPPLE, FOAM, FACE_DARK = 9, 10, 11, 12, 13


def lake_grid(width, height, fill):
    return [[fill] * width for _ in range(height)]


def lake_speckle(grid, seed, color, count):
    state = seed
    for _ in range(count):
        state = (state * 1103515245 + 12345) & 0x7fffffff
        x = (state >> 8) % len(grid[0])
        state = (state * 1103515245 + 12345) & 0x7fffffff
        y = (state >> 8) % len(grid)
        grid[y][x] = color


def lake_floor():
    grid = lake_grid(16, 16, FLOOR)
    lake_speckle(grid, 11, FLOOR_SHADE, 10)
    lake_speckle(grid, 29, FLOOR_LIGHT, 6)
    for x, y in ((3, 12), (4, 12), (11, 4), (12, 4)):
        grid[y][x] = FLOOR_SHADE
    return grid


def lake_floor_detail():
    grid = lake_floor()
    for x, y, rock in ((4, 5, 3), (10, 9, 2), (6, 11, 2)):
        for dy in range(rock):
            for dx in range(rock + 1):
                grid[y + dy][x + dx] = PEBBLE
        grid[y][x] = FLOOR_LIGHT
        for dx in range(rock + 1):
            grid[y + rock][x + dx] = FLOOR_SHADE
    return grid


def lake_shadow():
    grid = lake_floor()[:8]
    for x in range(16):
        grid[0][x] = PEBBLE
        grid[1][x] = FLOOR_SHADE
        grid[2][x] = FLOOR_SHADE if x % 2 == 0 else grid[2][x]
    return grid


def lake_wall_top():
    grid = lake_grid(16, 16, ROCK)
    for ox, oy in ((0, 0), (8, 0), (4, 8), (12, 8)):
        for y in range(8):
            for x in range(8):
                gx, gy = (ox + x) % 16, oy + y
                dx, dy = x - 3.5, y - 3.5
                distance = dx * dx + dy * dy
                if distance > 15:
                    grid[gy][gx] = ROCK_SHADE if distance < 20 else OUTLINE
                elif dx + dy < -3:
                    grid[gy][gx] = ROCK_LIGHT
                elif dx + dy > 3:
                    grid[gy][gx] = ROCK_SHADE
    return grid


def lake_face_high():
    grid = lake_grid(16, 8, ROCK)
    for x in range(16):
        grid[0][x] = ROCK_LIGHT if x % 4 else ROCK
    for x, top in ((1, 1), (5, 2), (9, 1), (13, 2)):
        for y in range(top, 8):
            grid[y][x] = ROCK_SHADE
            grid[y][(x + 1) % 16] = ROCK_LIGHT if y < 4 else ROCK
    for x in (3, 11):
        grid[5][x] = ROCK_SHADE
        grid[6][x] = ROCK_SHADE
    return grid


def lake_face_low():
    grid = lake_grid(16, 8, ROCK_SHADE)
    for x, bottom in ((1, 4), (5, 3), (9, 5), (13, 3)):
        for y in range(0, bottom):
            grid[y][x] = FACE_DARK
            grid[y][(x + 1) % 16] = ROCK
    for x in range(16):
        grid[5][x] = FACE_DARK if x % 2 else ROCK_SHADE
        grid[6][x] = FACE_DARK
        grid[7][x] = OUTLINE
    return grid


def lake_water(seed=0, deep=False):
    grid = lake_grid(16, 16, DEEP if deep else WATER)
    for x0, y in ((1, 3), (9, 7), (3, 12), (11, 14)):
        x0 = (x0 + seed) % 16
        for dx, dy in ((0, 1), (1, 0), (2, 0), (3, 1)):
            grid[(y + dy) % 16][(x0 + dx) % 16] = RIPPLE
    grid[(4 + seed) % 16][6] = FOAM
    grid[(11 + seed) % 16][14] = FOAM
    return grid


def lake_flow(dx, dy):
    grid = lake_grid(16, 16, WATER)
    for lane, offset in ((3, 0), (11, 5)):
        for i in range(16):
            if (i + offset) % 8 < 4:
                grid[lane][i] = RIPPLE
        for tip in ((offset + 5) % 16, (offset + 13) % 16):
            for k in (-2, -1, 0, 1, 2):
                grid[(lane + k) % 16][(tip - abs(k)) % 16] = FOAM
    grid[7][2] = grid[15][9] = DEEP
    grid[7][3] = grid[15][10] = DEEP
    if dx < 0:
        grid = [row[::-1] for row in grid]
    if dy:
        grid = [[grid[x][y] for x in range(16)] for y in range(16)]
        if dy < 0:
            grid = grid[::-1]
    return grid


def lake_door():
    grid = lake_water(3, deep=True)
    for x in range(16):
        grid[0][x] = FOAM if x % 3 else RIPPLE
        grid[8][x] = RIPPLE if x % 4 < 2 else grid[8][x]
    return grid


def lake_stairs():
    grid = lake_grid(16, 16, ROCK_SHADE)
    for step in range(4):
        top = step * 4
        left, right = 1 + step, 15 - step
        for x in range(left, right):
            grid[top][x] = FLOOR_LIGHT
            grid[top + 1][x] = FLOOR
            grid[top + 2][x] = FLOOR_SHADE
            grid[top + 3][x] = PEBBLE
        grid[top + 1][left] = grid[top + 2][left] = FLOOR_LIGHT
    for y in range(16):
        grid[y][0] = OUTLINE
        grid[y][15] = OUTLINE
    return grid


def lake_tileset():
    floor = split_block(lake_floor())
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(lake_floor_detail())
    tiles += split_pair(lake_shadow())
    tiles += split_block(lake_wall_top())
    tiles += split_pair(lake_face_high())
    tiles += split_pair(lake_face_low())
    tiles += split_block(lake_door())
    tiles += split_block(lake_stairs())
    tiles += floor
    tiles += floor
    tiles += split_block(lake_water())
    tiles += split_block(lake_flow(1, 0))
    tiles += split_block(lake_flow(-1, 0))
    tiles += split_block(lake_flow(0, 1))
    tiles += split_block(lake_flow(0, -1))
    tiles += floor * 3
    return tiles, LAKE_PALETTE
