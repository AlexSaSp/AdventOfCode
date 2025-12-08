import sys
import queue

def main():
    args = sys.argv

    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()
    
    splitter = queue.Queue()
    splitter.put((0, lines[0].find('S')))
    num_splits = 0

    last = (-1, -1)
    while not splitter.empty():
        l, p = splitter.get()
        if l == len(lines) - 1:
            continue
        if lines[l + 1][p] == '^':
            num_splits += 1
            if p > 0 and lines[l + 1][p - 1] != '^' and (l + 1, p - 1) != last:
                # print(l + 1, p - 1)
                splitter.put((l + 1, p - 1))
            if p < len(lines[0]) - 1 and lines[l + 1][p + 1] != '^':
                # print(l + 1, p + 1)
                splitter.put((l + 1, p + 1))
                last = (l + 1, p + 1)
        elif last != (l + 1, p):
            splitter.put((l + 1, p))
            last = (l + 1, p)
    
    print("Beam splits:", num_splits)

if __name__ == "__main__":
    main()
