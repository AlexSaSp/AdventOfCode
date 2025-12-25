from sys import argv
from queue import Queue

def toggle_light(state, button):
    new_state = list(state)
    for i in button:
        if new_state[i] == '.':
            new_state[i] = '#'
        else:
            new_state[i] = '.'
    return "".join(new_state)

def increase_joltage(state : list[int], button):
    new_state = state.copy()
    for i in button:
        new_state[i] = state[i] + 1
    return new_state

def main():
    if len(argv) < 2:
        print("Usage: python", argv[0], "<input_file>")
        return

    with open(argv[1]) as f:
        lines = f.readlines()
    
    lines_splitted = [line.split(' ') for line in lines]
    machines = [(
        line[0].removeprefix('[').removesuffix(']'),
        [list(map(lambda x: int(x), t.removeprefix('(').removesuffix(')').split(','))) for t in line[1:len(line)-1]],
        [int(i) for i in line[len(line)-1].removeprefix('{').removesuffix('}\n').split(',')]
        ) for line in lines_splitted]

    sum_fewest_presses = 0
    for machine in machines:
        seen = set()
        init_state, buttons, _ = machine
        q = Queue()
        q.put((0, "." * len(init_state)))
        # print("Searching:", init_state)
        while not q.empty():
            toggles, state = q.get()
            if state in seen:
                continue
            seen.add(state)
            for button in buttons:
                new_state = toggle_light(state, button)
                assert id(new_state) != id(state)
                #print(new_state)
                if new_state == init_state:
                    sum_fewest_presses += toggles + 1
                    # print("Found in", toggles + 1, "presses")
                    q = Queue()
                    break
                else:
                    q.put((toggles + 1, new_state))
    
    print("Sum of fewest button presses to reset all machines:", sum_fewest_presses)
    
if __name__ == "__main__":
    main()