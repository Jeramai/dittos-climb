from common import split_block, split_pair

PLANT_PALETTE = [
    (12, 14, 20),
    (172, 180, 188),
    (146, 154, 166),
    (200, 206, 214),
    (112, 120, 136),
    (84, 92, 110),
    (60, 66, 82),
    (40, 44, 56),
    (22, 24, 32),
    (240, 208, 56),
    (176, 136, 32),
    (255, 250, 190),
    (72, 200, 104),
    (224, 72, 64),
    (104, 208, 240),
    (124, 134, 150),
]

FLOOR, FLOOR_SHADE, FLOOR_LIGHT, SEAM = 1, 2, 3, 4
STEEL_LIGHT, STEEL, STEEL_DARK, OUTLINE = 5, 6, 7, 8
YELLOW, YELLOW_DARK, SPARK, GREEN, RED, CYAN, PANEL = 9, 10, 11, 12, 13, 14, 15


def plant_grid(width, height, fill):
    return [[fill] * width for _ in range(height)]


def plant_floor():
    grid = plant_grid(16, 16, FLOOR)
    for i in range(16):
        grid[15][i] = SEAM
        grid[i][15] = SEAM
        grid[0][i] = FLOOR_LIGHT
        grid[i][0] = FLOOR_LIGHT
    for x, y in ((2, 2), (13, 2), (2, 13), (13, 13)):
        grid[y][x] = FLOOR_SHADE
    for i in range(3, 13):
        grid[8][i] = FLOOR_SHADE if i % 2 else FLOOR
    return grid


def plant_floor_detail():
    grid = plant_floor()
    for y in range(4, 12):
        for x in range(4, 12):
            grid[y][x] = SEAM if (x + y) % 2 else FLOOR_SHADE
    for i in range(4, 12):
        grid[3][i] = FLOOR_LIGHT
        grid[12][i] = STEEL
    return grid


def plant_shadow():
    grid = plant_floor()[:8]
    for x in range(16):
        grid[0][x] = SEAM
        grid[1][x] = FLOOR_SHADE
        grid[2][x] = FLOOR_SHADE if x % 2 == 0 else grid[2][x]
    return grid


def plant_wall_top():
    grid = plant_grid(16, 16, STEEL_DARK)
    for i in range(16):
        grid[0][i] = STEEL
        grid[i][0] = STEEL
        grid[7][i] = OUTLINE
        grid[i][7] = OUTLINE
        grid[8][i] = STEEL
        grid[i][8] = STEEL
    for x, y in ((2, 2), (5, 5), (10, 10), (13, 13), (2, 13), (13, 2)):
        grid[y][x] = STEEL_LIGHT
    for x in range(10, 15):
        grid[3][x] = PANEL
        grid[4][x] = PANEL
    return grid


def plant_face_high():
    grid = plant_grid(16, 8, PANEL)
    for x in range(16):
        grid[0][x] = YELLOW if (x // 2) % 2 else OUTLINE
        grid[1][x] = STEEL_LIGHT
        grid[7][x] = STEEL
    for x in (0, 8):
        for y in range(1, 8):
            grid[y][x] = STEEL
    for x in range(2, 7):
        grid[3][x] = OUTLINE
        grid[4][x] = CYAN if x % 2 else OUTLINE
        grid[5][x] = OUTLINE
    grid[3][11] = GREEN
    grid[3][13] = RED
    for x in range(10, 15):
        grid[5][x] = STEEL
    return grid


def plant_face_low():
    grid = plant_grid(16, 8, STEEL)
    for x in (0, 8):
        for y in range(0, 6):
            grid[y][x] = STEEL_DARK
    for y in (1, 3):
        for x in range(2, 7):
            grid[y][x] = STEEL_DARK
        for x in range(10, 15):
            grid[y][x] = STEEL_DARK
    for x in range(16):
        grid[5][x] = STEEL_DARK
        grid[6][x] = OUTLINE
        grid[7][x] = OUTLINE
    return grid


def plant_door():
    grid = plant_grid(16, 16, STEEL_DARK)
    for x in (1, 5, 10, 14):
        for y in range(16):
            grid[y][x] = STEEL
    for y in (3, 11):
        for x in range(16):
            zig = (x // 2) % 2
            grid[y + zig][x] = YELLOW
            grid[y + 1 - zig][x] = SPARK if x % 4 == 0 else grid[y + 1 - zig][x]
    return grid


def plant_stairs():
    grid = plant_grid(16, 16, STEEL_DARK)
    for step in range(4):
        top = step * 4
        for x in range(1 + step, 15 - step):
            grid[top][x] = YELLOW if (x // 2) % 2 else OUTLINE
            grid[top + 1][x] = STEEL_LIGHT
            grid[top + 2][x] = STEEL
            grid[top + 3][x] = STEEL_DARK
    for y in range(16):
        grid[y][0] = grid[y][15] = OUTLINE
    return grid


def plant_plate(phase):
    small = [[FLOOR] * 8 for _ in range(8)]
    frame = YELLOW_DARK if phase == 0 else YELLOW
    inside = STEEL_DARK if phase == 0 else YELLOW_DARK if phase == 1 else YELLOW
    for y in range(8):
        for x in range(8):
            if x == 7 or y == 7:
                small[y][x] = OUTLINE
            elif x == 0 or y == 0 or x == 6 or y == 6:
                small[y][x] = frame
            else:
                small[y][x] = inside
    for x, y in ((4, 1), (3, 2), (2, 3), (3, 3), (4, 3), (3, 4), (2, 5)):
        small[y][x] = YELLOW if phase == 0 else SPARK if phase == 1 else CYAN
    if phase == 2:
        for x in range(7):
            small[0][x] = SPARK if x % 2 else CYAN
        small[5][5] = small[1][1] = SPARK
    return [row * 2 for row in small] * 2


def plant_tileset():
    floor = split_block(plant_floor())
    tiles = [[[0] * 8 for _ in range(8)]]
    tiles += floor
    tiles += split_block(plant_floor_detail())
    tiles += split_pair(plant_shadow())
    tiles += split_block(plant_wall_top())
    tiles += split_pair(plant_face_high())
    tiles += split_pair(plant_face_low())
    tiles += split_block(plant_door())
    tiles += split_block(plant_stairs())
    tiles += floor * 7
    for phase in range(3):
        tiles += split_block(plant_plate(phase))
    return tiles, PLANT_PALETTE
