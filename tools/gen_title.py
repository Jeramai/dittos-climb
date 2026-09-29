#!/usr/bin/env python3
import json
import math
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
GRAPHICS = ROOT / "graphics"
SIZE = 256
VISIBLE_WIDTH, VISIBLE_HEIGHT = 240, 160

SKY = [(16, 16, 40), (22, 22, 54), (30, 30, 70), (40, 38, 88), (52, 46, 104)]
STAR_DIM, STAR, STAR_BRIGHT = (96, 96, 144), (176, 176, 216), (248, 248, 255)
MOON, MOON_SHADE, MOON_CRATER, MOON_GLOW = (240, 236, 208), (208, 204, 176), (216, 212, 184), (64, 60, 116)
ROCK_DARK, ROCK, ROCK_LIT, ROCK_EDGE = (30, 24, 54), (58, 48, 92), (90, 80, 124), (128, 116, 164)
ROCK_SPECK, ROCK_LIT_SPECK = (46, 38, 76), (114, 102, 150)
STEP, STEP_LIT, STEP_RISER = (152, 140, 188), (176, 168, 204), (40, 32, 66)
CRYSTAL_DARK, CRYSTAL, CRYSTAL_BRIGHT = (64, 144, 184), (120, 208, 232), (208, 248, 255)
FAR_HILL, FAR_HILL_LIT, CLOUD, CLOUD_DARK = (34, 30, 66), (46, 42, 84), (84, 80, 128), (60, 56, 104)
TREE_BACK, TREE_BACK_LIT, TREE_FRONT, TREE_FRONT_LIT = (14, 30, 30), (22, 50, 44), (8, 18, 16), (16, 34, 28)
GRASS, GRASS_BLADE = (12, 26, 20), (26, 52, 36)
LOGO_OUTLINE, LOGO, LOGO_LIGHT, LOGO_SHADE, LOGO_SHADOW = (
    (104, 48, 128), (200, 144, 216), (236, 204, 244), (160, 104, 184), (8, 6, 20))

GLYPHS = {
    "D": ["1110.", "1..1.", "1...1", "1...1", "1...1", "1..1.", "1110."],
    "I": ["111", ".1.", ".1.", ".1.", ".1.", ".1.", "111"],
    "T": ["11111", "..1..", "..1..", "..1..", "..1..", "..1..", "..1.."],
    "O": [".111.", "1...1", "1...1", "1...1", "1...1", "1...1", ".111."],
    "'": ["1", "1", ".", ".", ".", ".", "."],
    "S": [".1111", "1....", "1....", ".111.", "....1", "....1", "1111."],
    "C": [".1111", "1....", "1....", "1....", "1....", "1....", ".1111"],
    "L": ["1....", "1....", "1....", "1....", "1....", "1....", "11111"],
    "M": ["1...1", "11.11", "1.1.1", "1.1.1", "1...1", "1...1", "1...1"],
    "B": ["1111.", "1...1", "1...1", "1111.", "1...1", "1...1", "1111."],
}


def noise(x, y, seed):
    value = (x * 374761393 + y * 668265263 + seed * 2246822519) & 0xffffffff
    value = ((value ^ (value >> 13)) * 1274126177) & 0xffffffff
    return (value ^ (value >> 16)) & 0xff


def sky(x, y):
    band = min(y // 24, len(SKY) - 1)
    if y % 24 >= 20 and band + 1 < len(SKY) and (x + y) % 2 == 0:
        return SKY[band + 1]
    return SKY[band]


def draw_stars(pixels):
    for index in range(70):
        x = noise(index, 1, 3) * VISIBLE_WIDTH // 256
        y = noise(index, 2, 3) * 96 // 256
        kind = noise(index, 3, 3)
        pixels[y][x] = STAR_BRIGHT if kind > 225 else STAR if kind > 120 else STAR_DIM
        if kind > 225:
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                if 0 <= x + dx < SIZE and 0 <= y + dy < SIZE:
                    pixels[y + dy][x + dx] = STAR


def draw_moon(pixels, cx, cy, radius):
    for y in range(cy - radius - 3, cy + radius + 4):
        for x in range(cx - radius - 3, cx + radius + 4):
            distance = math.hypot(x - cx, y - cy)
            if distance <= radius:
                shade = math.hypot(x - cx - 5, y - cy + 4) > radius + 1
                pixels[y][x] = MOON_SHADE if shade else MOON
            elif distance <= radius + 2 and (x + y) % 2 == 0:
                pixels[y][x] = MOON_GLOW
    for dx, dy, r in ((-6, -3, 3), (4, 5, 2), (-2, 8, 2), (7, -6, 1)):
        for y in range(cy + dy - r, cy + dy + r + 1):
            for x in range(cx + dx - r, cx + dx + r + 1):
                if math.hypot(x - cx - dx, y - cy - dy) <= r and pixels[y][x] == MOON:
                    pixels[y][x] = MOON_CRATER


def mountain_top(x):
    apex = 120
    distance = abs(x - apex)
    height = 62 + distance * 0.72 + 4 * math.sin(x * 0.35) + 3 * math.sin(x * 0.11 + 1)
    if 90 < x < 100 or 146 < x < 154:
        height -= 5
    return int(height)


def draw_far_hills(pixels):
    for x in range(SIZE):
        top = int(100 + 10 * math.sin(x * 0.05 + 2) + 6 * math.sin(x * 0.13))
        for y in range(top, 150):
            pixels[y][x] = FAR_HILL_LIT if y == top else FAR_HILL


def draw_cloud(pixels, cx, cy, width):
    for x in range(cx - width, cx + width):
        half = int(3 * (1 - abs(x - cx) / width) + 1)
        for y in range(cy - half, cy + 1):
            pixels[y][x] = CLOUD if y < cy - half + 2 else CLOUD_DARK


def draw_mountain(pixels):
    for x in range(20, 222):
        top = mountain_top(x)
        for y in range(max(top, 0), 150):
            lit = x > 120 + (y - 62) * 0.15
            n = noise(x // 2, y // 2, 7)
            if y == top or (lit and y == top + 1):
                color = ROCK_EDGE if lit else ROCK_LIT
            elif lit:
                color = ROCK_LIT_SPECK if n < 40 else ROCK_LIT
            else:
                color = ROCK_SPECK if n < 50 else ROCK
            if n > 246 or (y - top > 6 and (y + int(4 * math.sin(x * 0.2))) % 13 == 0):
                color = ROCK_DARK if not lit else ROCK
            pixels[y][x] = color
    for x, y in ((74, 118), (166, 112), (98, 96), (150, 90), (60, 128), (186, 124)):
        for dy in range(4):
            for dx in range(-(dy // 2), dy // 2 + 1):
                if pixels[y + dy][x + dx] != SKY[0]:
                    pixels[y + dy][x + dx] = CRYSTAL if dx <= 0 else CRYSTAL_DARK
        pixels[y][x] = CRYSTAL_BRIGHT


def draw_stairs(pixels):
    path = [(150, 134), (92, 116), (142, 100), (106, 86), (130, 74), (120, 66)]
    mask = set()
    for (x0, y0), (x1, y1) in zip(path, path[1:]):
        steps = int(max(abs(x1 - x0), abs(y1 - y0)))
        for step in range(steps + 1):
            t = step / steps
            x = round(x0 + (x1 - x0) * t)
            y = round(y0 + (y1 - y0) * t)
            mask |= {(x + dx, y + dy) for dx in range(-3, 4) for dy in range(0, 3)}
    for x, y in mask:
        if (x - 1, y) not in mask or (x + 1, y) not in mask or (x, y + 1) not in mask:
            color = STEP_RISER
        elif y % 3 == 2:
            color = STEP_RISER
        elif y % 3 == 0:
            color = STEP_LIT
        else:
            color = STEP
        pixels[y][x] = color


def draw_trees(pixels):
    for row, (base, radius, dark, lit) in enumerate(((124, 10, TREE_BACK, TREE_BACK_LIT),
                                                      (138, 12, TREE_FRONT, TREE_FRONT_LIT))):
        offset = 8 * row
        for cx in range(-16 + offset, SIZE + 16, 16):
            for y in range(base - radius, SIZE):
                for x in range(cx - radius, cx + radius + 1):
                    if not (0 <= x < SIZE and 0 <= y < SIZE):
                        continue
                    if y < base and math.hypot((x - cx) * 0.9, y - base) > radius:
                        continue
                    edge = y < base and math.hypot((x - cx) * 0.9 - 2, y - base + 2) > radius - 1.5
                    pixels[y][x] = lit if edge and x >= cx - 2 else dark
    for y in range(152, SIZE):
        for x in range(SIZE):
            pixels[y][x] = GRASS_BLADE if y < 156 and (x * 3 + y) % 8 == 0 else GRASS


def bold(rows):
    return ["".join("1" if c == "1" or (i > 0 and row[i - 1] == "1") else "." for i, c in enumerate(row + "."))
            for row in rows]


def draw_text(pixels, text, top, scale):
    glyphs = {c: bold(rows) for c, rows in GLYPHS.items()}
    widths = [len(glyphs[c][0]) if c != " " else 3 for c in text]
    total = sum(w * scale + 2 for w in widths) - 2
    x = (VISIBLE_WIDTH - total) // 2
    mask = {}
    for char, width in zip(text, widths):
        if char != " ":
            for gy, row in enumerate(glyphs[char]):
                for gx, bit in enumerate(row):
                    if bit == "1":
                        for sy in range(scale):
                            for sx in range(scale):
                                mask[(x + gx * scale + sx, top + gy * scale + sy)] = gy * scale + sy
        x += width * scale + 2
    height = 7 * scale
    outline = {(px + dx, py + dy) for px, py in mask for dx in (-1, 0, 1) for dy in (-1, 0, 1)} - set(mask)
    for px, py in outline | set(mask):
        for dx, dy in ((3, 3), (2, 3), (3, 2)):
            if (px + dx, py + dy) not in mask and (px + dx, py + dy) not in outline:
                pixels[py + dy][px + dx] = LOGO_SHADOW
    for px, py in outline:
        pixels[py][px] = LOGO_OUTLINE
    for (px, py), row in mask.items():
        if row < scale:
            color = LOGO_LIGHT
        elif row >= height - scale:
            color = LOGO_SHADE
        else:
            color = LOGO
        if (px - 1, py) not in mask and row >= scale:
            color = LOGO_LIGHT if row < height // 2 else color
        pixels[py][px] = color


def build():
    pixels = [[sky(x, y) for x in range(SIZE)] for y in range(SIZE)]
    draw_stars(pixels)
    draw_moon(pixels, 198, 80, 16)
    draw_cloud(pixels, 204, 90, 22)
    draw_cloud(pixels, 40, 70, 18)
    draw_far_hills(pixels)
    draw_mountain(pixels)
    draw_stairs(pixels)
    draw_trees(pixels)
    draw_text(pixels, "DITTO'S", 7, 3)
    draw_text(pixels, "CLIMB", 34, 3)
    return pixels


def save(pixels):
    colors = [SKY[0]]
    for row in pixels:
        for color in row:
            if color not in colors:
                colors.append(color)
    assert len(colors) <= 256, len(colors)
    index = {color: i for i, color in enumerate(colors)}
    image = Image.new("P", (SIZE, SIZE))
    flat = [channel for color in colors for channel in color]
    image.putpalette(flat + [0] * (768 - len(flat)))
    image.putdata([index[color] for row in pixels for color in row])
    image.save(GRAPHICS / "title_bg.bmp")
    (GRAPHICS / "title_bg.json").write_text(json.dumps({"type": "regular_bg", "bpp_mode": "bpp_8"}, indent=4) + "\n")
    tiles = {tuple(tuple(row[x:x + 8]) for row in pixels[y:y + 8]) for y in range(0, SIZE, 8) for x in range(0, SIZE, 8)}
    return len(colors), len(tiles)


def main():
    pixels = build()
    colors, tiles = save(pixels)
    print(f"title_bg: {colors} colours, {tiles} unique tiles")
    return pixels


if __name__ == "__main__":
    main()
