def split_block(block):
    return [[row[x0:x0 + 8] for row in block[y0:y0 + 8]] for y0 in (0, 8) for x0 in (0, 8)]


def split_pair(block):
    return [[row[x0:x0 + 8] for row in block] for x0 in (0, 8)]


def from_chars(rows, key):
    return [[0 if c in ". " else key[c] for c in row] for row in rows]
