from sys import argv

def find_paths(current: str, target: str, neighbours: dict[str, list[str]]) -> int:
    if current == target:
        return 1
    if current not in neighbours:
        return 0
    sum_paths = 0
    for neighbour in neighbours[current]:
        sum_paths += find_paths(neighbour, target, neighbours)
    return sum_paths


def find_paths_dac_fft(current: str, target: str, neighbours: dict[str, list[str]], dac: bool, fft: bool, cache: dict[tuple[str, bool, bool], int]) -> int:
    if current == target:
        return 1 if dac and fft else 0
    if current not in neighbours:
        return 0
    if (current, dac, fft) in cache:
        return cache[(current, dac, fft)]

    if current == "dac":
        dac = True
    if current == "fft":
        fft = True
    
    sum_paths = 0
    for neighbour in neighbours[current]:
        sum_paths += find_paths_dac_fft(neighbour, target, neighbours, dac, fft, cache)
    
    cache[(current, dac, fft)] = sum_paths
    return sum_paths


def main():
    if len(argv) < 2:
        print("Usage: python", argv[0], "<input_file>")
        return

    with open(argv[1]) as f:
        lines = f.readlines()
    
    neighbours = {}
    for line in lines:
        devices = line.split(': ')
        neighbours[devices[0]] = devices[1].split( )
    
    print("Paths you -> out", find_paths("you", "out", neighbours))
    print("Paths svr -> out via dac and fft", find_paths_dac_fft("svr", "out", neighbours, False, False, {}))


if __name__ == "__main__":
    main()