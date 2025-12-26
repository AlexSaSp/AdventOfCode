from sys import argv
from itertools import product
from fractions import Fraction


def make_matrix(buttons: list[list[int]], joltage: tuple[int, ...]) -> list[list[Fraction]]:
    matrix = [[Fraction(0)] * (len(buttons) + 1) for _ in range(len(joltage))]
    for i in range(len(joltage)):
        matrix[i][-1] = Fraction(joltage[i])
    for j in range(len(buttons)):
        button = buttons[j]
        for i in button:
            matrix[i][j] = Fraction(1)
    return matrix


def subtract(l1: list[Fraction], l2: list[Fraction]) -> None:
    assert len(l1) == len(l2)
    for i in range(len(l1)):
        l2[i] = l2[i] - l1[i]


def divide(line: list[Fraction], divisor: Fraction):
    for i in range(len(line)):
        line[i] = line[i] / divisor


def normalize(line: list[Fraction]):
    first_el = next((element for element in line if element != 0), None)
    if first_el == None:
        return
    divide(line, first_el)


def eliminate(idx: int, matrix: list[list[Fraction]]) -> None:
    matrix.sort(reverse=True)
    line_for_elim = next(((l_idx, l) for l_idx, l in enumerate(matrix) if l[idx] == 1 and l[:idx] == [0] * idx), (len(matrix), None))
    for i in range(line_for_elim[0] + 1, len(matrix)):
        if matrix[i][idx] == 0:
            break
        for j in range(len(matrix[i])):
            matrix[i][j] -= matrix[line_for_elim[0]][j]
        normalize(matrix[i])


def gauss(matrix: list[list[Fraction]]) -> None:
    for i in range(len(matrix[0]) - 1):
        eliminate(i, matrix)

def init_amount(button: list[int], joltage: tuple[int, ...]) -> int:
    min_el = joltage[button[0]]
    for i in button:
        min_el = min(joltage[i], min_el)
    return min_el

def find_smallest_sol(matrix: list[list[Fraction]], buttons: list[list[int]], joltage: tuple[int, ...]):
    unique_sol = [False for _ in buttons]
    assert len(buttons) == len(matrix[0]) - 1
    for line in matrix:
        if line[:-1] != [0] * (len(line) - 1):
            unique_sol[line.index(Fraction(1))] = True

    smallest_sol = 10000000
    solutions = [Fraction(0)] * len(buttons)
    not_unique_idx = [i for i, j in enumerate(unique_sol) if not j]
    ranges = [[j for j in range(0, init_amount(buttons[i], joltage) + 1)] for i in not_unique_idx]
    if len(not_unique_idx) == 0:
        not_unique_idx.append(0)
        ranges.append([0])
    for k in product(*ranges):
        for i_u, u in enumerate(not_unique_idx):
            solutions[u] = Fraction(k[i_u])

            negative = False
            for i in range(len(matrix) - 1, -1, -1):
                if matrix[i][:-1] == [0] * (len(matrix[i]) - 1):
                    continue
                sol_idx = matrix[i].index(Fraction(1))
                sol = matrix[i][-1]
                for j in range(sol_idx + 1, len(matrix[i]) - 1):
                    sol -= matrix[i][j] * solutions[j]
                solutions[sol_idx] = sol
                if sol < 0 or not sol.is_integer():
                    negative = True
                    break
            
            if not negative:
                smallest_sol = min(smallest_sol, sum(solutions))
    
    assert smallest_sol < 10000000, matrix
    return smallest_sol

def main() -> None:
    if len(argv) < 2:
        print("Usage: python", argv[0], "<input_file>")
        return

    with open(argv[1]) as f:
        lines = f.readlines()
    
    lines_splitted = [line.split(' ') for line in lines]
    machines = [(
        line[0].removeprefix('[').removesuffix(']'),
        [list(map(lambda x: int(x), t.removeprefix('(').removesuffix(')').split(','))) for t in line[1:len(line)-1]],
        tuple(int(i) for i in line[len(line)-1].removeprefix('{').removesuffix('}\n').split(','))
        ) for line in lines_splitted]

    sum_fewest_presses = 0
    for machine in machines:
        _, buttons, joltage = machine
        matrix = make_matrix(buttons, joltage)
        gauss(matrix)    
        smallest = find_smallest_sol(matrix, buttons, joltage)
        sum_fewest_presses += smallest
    
    print("Sum of fewest button presses to reset all machines:", sum_fewest_presses)
    
if __name__ == "__main__":
    main()