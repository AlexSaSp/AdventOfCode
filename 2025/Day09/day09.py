import sys

def intersect(a, b, c, d):
    ax, ay = a
    bx, by = b
    cx, cy = c
    dx, dy = d

    assert a != b
    assert c != d
    if ax == bx:
        assert ay != by
        return (cx <= ax <= dx or dx <= ax <= cx) and (ay <= cy <= by or by <= cy <= ay)
    assert ay == by
    if cx == dx:
        return (ax <= cx <= bx or bx <= cx <= ax) and (cy <= ay <= dy or dy <= ay <= cy)
    return False

def no_intersections(a, b, coords):
    if a == b:
        return True
    for i in range(1, len(coords)):
        c = coords[i - 1]
        d = coords[i]
        if a == (a[0], b[1]) or (a[0], b[1]) == b:
            if intersect(a, b, c, d):
                return False
        elif (intersect(a, (a[0], b[1]), c, d)
            or intersect((a[0], b[1]), b, c, d)
            or intersect(b, (b[0], a[1]), c, d)
            or intersect((b[0], a[1]), a, c, d)):
            return False
    return True

def move_inward(a, b):
    ax, ay = a
    bx, by = b
    if ax < bx:
        ax += 1
        bx -= 1
    elif ax > bx:
        ax -= 1
        bx += 1
    if ay < by:
        ay += 1
        by -= 1
    elif ay > by:
        ay -= 1
        by += 1
    return (ax, ay), (bx, by)

def is_left(p, a, b):
    ax, ay = a
    bx, by = b
    px, py = p
    if ax == bx and ay < by:
        return px > ax
    if ax == bx and ay > by:
        return px < ax
    if ay == by and ax < bx:
        return py > ay
    if ay == by and ax > bx:
        return py < ay
    return False

# Check if a cornner moved inwards the rectangle is inside the polygon
def is_inside(p, id_p, coords):
    prev_c = coords[(id_p - 1) % len(coords)]
    next_c = coords[(id_p + 1) % len(coords)]
    current_c = coords[id_p]

    return is_left(p, prev_c, current_c) and is_left(p, current_c, next_c)
    

def main():
    args = sys.argv

    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()

    curr_max = 0

    coords = list(map(lambda x: (int(x.split(',')[0]), int(x.split(',')[1])), lines))

    for i_idx, i in enumerate(coords):
        for j in coords:
            if i != j:
                area = (abs(i[0] - j[0]) + 1) * (abs(i[1] - j[1]) + 1)
                i_inv, j_inv = move_inward(i, j)
                if (area > curr_max # <- For part 1 only this condition is needed
                    and is_inside(i_inv, i_idx, coords)
                    and i[0] != j[0] and i[1] != j[1] # Assume that the rectangle with the largest area is not a line 
                    and no_intersections(i_inv, j_inv, coords)):
                    curr_max = area
    
    print("Largest area:", curr_max)

if __name__ == "__main__":
    main()
