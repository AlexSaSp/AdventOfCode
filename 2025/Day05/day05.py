import sys

def main():
    args = sys.argv

    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()
    
    fresh_ingr : list[tuple[int, int]] = []
    sum_fresh_ingr = 0
    total_fresh_IDs = 0
    checkIDs = False
    for line in lines:
        if line == '\n':
            checkIDs = True
            continue
        if not checkIDs:
            id_range = line.split('-')
            assert(len(id_range) == 2)
            fresh_ingr.append((int(id_range[0]), int(id_range[1])))
        else:
            # Part 1
            for k, v in fresh_ingr:
                if int(line) >= k and int(line) <= v:
                    sum_fresh_ingr += 1
                    break
    
    # Part 2
    fresh_ingr = sorted(fresh_ingr)
    for i, d in enumerate(fresh_ingr):
        k, v = d
        if i > 0 and k <= fresh_ingr[i - 1][1]:
            total_fresh_IDs += max(0, v - fresh_ingr[i - 1][1])
        else:
            total_fresh_IDs += v - k + 1
    
    print("Number of fresh ingredients: ", sum_fresh_ingr)
    print("Number of fresh IDs ", total_fresh_IDs)

if __name__ == "__main__":
    main()
