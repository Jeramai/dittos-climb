EMPTY, FLOOR, DETAIL, SHADOW, WALL_TOP, FACE_HIGH, FACE_LOW, DOOR, STAIRS = 0, 1, 5, 9, 11, 15, 17, 19, 23
GRASS, BUSH, WATER, FLOW_RIGHT, FLOW_LEFT, FLOW_DOWN, FLOW_UP, SPECIAL = 27, 31, 35, 39, 43, 47, 51, 55
TILESET_SIZE = 67

ROOM = [
    "########################################",
    "################DDDD####################",
    "################DDDD####################",
    "##..........................GGGGGG....##",
    "##..........................GGGGGG....##",
    "##....####..................GGGGGG....##",
    "##....####............................##",
    "##....####.......SS...................##",
    "##...............SS......>>>>>>vv.....##",
    "BB.......................wwwwww..v....##",
    "BB.......PPQQRR..........wwwwww..v....##",
    "BB.......PPQQRR..........^^<<<<<<.....##",
    "BB....................................##",
    "##......XX............................##",
    "##......XX............................##",
    "########################################",
    "########################################",
]

WALKABLE = set(".SGwPQRX><v^")


def _at(column, row):
    if 0 <= row < len(ROOM) and 0 <= column < len(ROOM[0]):
        return ROOM[row][column]
    return " "


def _quad(base, column, row):
    return base + (column & 1) + (row & 1) * 2


def _detail(column, row):
    block = ((column // 2) * 73856093 ^ (row // 2) * 19349663) & 0xffffffff
    return (block >> 3) % 7 == 0


def tile_for(column, row):
    value = _at(column, row)
    if value == "#":
        if _at(column, row + 1) in WALKABLE:
            return FACE_LOW + (column & 1)
        if _at(column, row + 1) == "#" and _at(column, row + 2) in WALKABLE:
            return FACE_HIGH + (column & 1)
        return _quad(WALL_TOP, column, row)
    if value == ".":
        if _at(column, row - 1) in "#DB":
            return SHADOW + (column & 1)
        return _quad(DETAIL if _detail(column, row) else FLOOR, column, row)
    if value == "S":
        stairs_column = next(c for c in range(len(ROOM[0])) if _at(c, row) == "S")
        stairs_row = next(r for r in range(len(ROOM)) if _at(stairs_column, r) == "S")
        return _quad(STAIRS, column - stairs_column, row - stairs_row)
    bases = {"D": DOOR, "G": GRASS, "B": BUSH, "w": WATER, ">": FLOW_RIGHT, "<": FLOW_LEFT, "v": FLOW_DOWN,
             "^": FLOW_UP, "P": SPECIAL, "Q": SPECIAL + 4, "R": SPECIAL + 8, "X": SPECIAL}
    return _quad(bases[value], column, row) if value in bases else EMPTY


def room_indices():
    return [[tile_for(column, row) for column in range(len(ROOM[0]))] for row in range(len(ROOM))]
