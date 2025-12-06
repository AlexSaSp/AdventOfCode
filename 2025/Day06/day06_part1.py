import sys

def main():
    args = sys.argv

    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()
    
    math_problems : list[list[str]] = []

    for line in lines:
        operands = line.split()
        math_problems.append(operands)
    
    grand_total = 0
    for i in range(0, len(math_problems[0])):
        result = 0 if math_problems[len(math_problems) - 1][i] == "+" else 1
        for j in range(0, len(math_problems) - 1):
            if math_problems[len(math_problems) - 1][i] == "+":
                result += int(math_problems[j][i])
            elif math_problems[len(math_problems) - 1][i] == "*":
                result *= int(math_problems[j][i])
            else:
                raise RuntimeError("Unknown Operand:", math_problems[len(math_problems) - 1][i])
        grand_total += result
        # print("Result: ", result)

    print("Grand Total:", grand_total)

if __name__ == "__main__":
    main()