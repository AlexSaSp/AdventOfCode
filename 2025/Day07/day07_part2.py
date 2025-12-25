import sys

def get_num_timelines(line: int, pos: int, lines: list[str], cache: dict[tuple[int, int], int]) -> int:
    if line == len(lines) - 1:
        return 0
    if (line, pos) in cache:
        return cache[(line, pos)]
    num_timelines = 0
    if lines[line + 1][pos] == '^':
        num_timelines = 1
        if pos > 0 and lines[line + 1][pos - 1] != '^':
            num_timelines += get_num_timelines(line + 1, pos - 1, lines, cache)
        if pos < len(lines[0]) - 1 and lines[line + 1][pos + 1] != '^':
            num_timelines += get_num_timelines(line + 1, pos + 1, lines, cache)
    else:
        num_timelines += get_num_timelines(line + 1, pos, lines, cache)
    cache[(line, pos)] = num_timelines
    return num_timelines

def main():
    args = sys.argv

    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()
    
    print("Beam splits:", get_num_timelines(0, lines[0].find('S'), lines, {}) + 1)

if __name__ == "__main__":
    main()
