import sys

def main():
    args = sys.argv

    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()

    array = []
    for line in lines:
        if line[0] == 'R':
            array.append(int(line[1:]))
        elif line[0] == 'L':
            array.append(-int(line[1:]))
    
    pos = 50
    zeros_a = 0
    zeros_b = 0
    for move in array:
        for i in range(0, abs(move)):
            if move > 0:
                pos = (pos + 1) % 100
            else:
                pos = (pos - 1) % 100
            
            if pos == 0:
                zeros_b += 1
        if pos == 0:
            zeros_a += 1 
    
    print("Password A:", zeros_a)
    print("Password B:", zeros_b)

if __name__ == "__main__":
    main()