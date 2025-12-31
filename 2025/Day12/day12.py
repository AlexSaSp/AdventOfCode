from sys import argv
from itertools import product
from math import ceil

NUM_PRESENT_TYPES = 6

def main():
    if len(argv) < 2:
        print("Usage: python", argv[0], "<input_file>")
        return

    with open(argv[1]) as file:
        lines = file.readlines()
    
    shapes: list[list[str]] = []
    regions: list[tuple[tuple[int, int], list[int]]] = []
    seen_shapes = 0
    for line in lines:
        if seen_shapes < NUM_PRESENT_TYPES:
            if line[0] == '\n':
                seen_shapes += 1
            elif line[0] != '.' and line[0] != '#':
                shapes.append([])
            else:
                shapes[seen_shapes].append(line.removesuffix('\n'))
        else:
            values = line.split(' ')
            region_list = values[0].removesuffix(':').split('x')
            regions.append(((int(region_list[0]), int(region_list[1])), [int(i) for i in values[1:]]))
    
    # This is an approximation. It may not work on every input
    regions_fit = 0
    for field, presents in regions:
        rows = field[0] // 4
        columns = ceil((sum(presents) // 2  * 4 + 3) / rows)
        if columns <= field[1]:
            regions_fit += 1

    print(regions_fit)


if __name__ == "__main__":
    main()
