import sys

def main():
    args = sys.argv

    CONNECTIONS = 1000
    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()
    
    distances = [[0 for _ in range(len(lines))] for _ in range(len(lines))]
    for i in range(len(lines)):
        for j in range(len(lines)):
            num_i = lines[i].split(',')
            assert len(num_i) == 3
            num_j = lines[j].split(',')
            assert len(num_j) == 3
            distances[i][j] = sum((int(num_i[k]) - int(num_j[k]))**2 for k in range(len(num_i)))
    
    circuits = [{i} for i in range(len(lines))]
    # for _ in range(CONNECTIONS): # Uncomment for Part 1
    while True:
        min_dist = 100000000000000
        min_index = (-1, -1)
        for i in range(len(lines)):
            for j in range(i, len(lines)):
                if i != j and distances[i][j] < min_dist:
                    min_dist = distances[i][j]
                    min_index = (i, j)
        
        distances[min_index[0]][min_index[1]] = 100000000000000
        distances[min_index[1]][min_index[0]] = 100000000000000
        new_set = circuits[min_index[0]].union(circuits[min_index[1]])
        if len(new_set) == len(lines):
            print("Product last 2 boxes:", int(lines[min_index[0]].split(',')[0]) * int(lines[min_index[1]].split(',')[0]))
            break
        for i in new_set:
            circuits[i] = new_set
    
    circuits.sort(key=lambda x: len(x), reverse=True)
    # print("Product 3 largest circuits:", len(circuits[0]) * len(circuits[len(circuits[0])]) * len(circuits[len(circuits[0]) + len(circuits[1])]))


if __name__ == "__main__":
    main()
