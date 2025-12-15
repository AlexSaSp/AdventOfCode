import sys
import heapq

def main():
    args = sys.argv

    CONNECTIONS = 1000
    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()
    
    distances = []
    for i in range(len(lines)):
        for j in range(i, len(lines)):
            if i != j:
                num_i = lines[i].split(',')
                assert len(num_i) == 3
                num_j = lines[j].split(',')
                assert len(num_j) == 3
                heapq.heappush(distances, (sum((int(num_i[k]) - int(num_j[k]))**2 for k in range(len(num_i))), i, j))
    
    circuits = [{i} for i in range(len(lines))]
    # for _ in range(CONNECTIONS): # Uncomment for Part 1
    while True:
        min_dist, i, j = heapq.heappop(distances) 
        new_set = circuits[i].union(circuits[j])
        if len(new_set) == len(lines):
            print("Product last 2 boxes:", int(lines[i].split(',')[0]) * int(lines[j].split(',')[0]))
            break
        for k in new_set:
            circuits[k] = new_set
    
    circuits.sort(key=lambda x: len(x), reverse=True)
    # print("Product 3 largest circuits:", len(circuits[0]) * len(circuits[len(circuits[0])]) * len(circuits[len(circuits[0]) + len(circuits[1])]))


if __name__ == "__main__":
    main()
