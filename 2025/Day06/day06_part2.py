import sys

def main():
    args = sys.argv

    if len(args) < 2:
        print("Usage: python", args[0], "<input_file>")
        return

    with open(args[1]) as f:
        lines = f.readlines()
    
    grand_total = 0

    result = 0
    operand = lines[len(lines) - 1][0]
    assert(operand == "+" or operand == "*")
    for i in range(len(lines[0])):
        number = ""
        operand = lines[len(lines) - 1][i] if lines[len(lines) - 1][i] == "+" or lines[len(lines) - 1][i] == "*" else operand
        if operand == "*":
            result = max(1, result)
        for j in range(len(lines) - 1):
            number += lines[j][i]
        if number.strip().isdigit() and operand == "+":
            result += int(number)
            # print("+", number, end="")
        elif number.strip().isdigit() and operand == "*":
            result *= int(number)
            # print("*", number, end="")
        else:
            #print ("=", result)
            grand_total += result
            result = 0
    
    grand_total += result
    print("Grand Total:", grand_total)

if __name__ == "__main__":
    main()