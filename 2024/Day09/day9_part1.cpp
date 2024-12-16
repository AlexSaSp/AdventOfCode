#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

#define FREE_BLOCK -1

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    std::ifstream file(argv[1], std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    std::vector<long> memory;
    std::string input;
    size_t line_number = 0;
    while (std::getline(file, input)) {
        line_number++;
    }
    file.close();
    assert(line_number == 1);

    for (size_t i = 0; i < input.size(); i++) {
        long number = input[i] - '0';
        for (long j = 0; j < number; j++) {
            if (i % 2 == 1) {
                memory.push_back(FREE_BLOCK);
            } else {
                memory.push_back(i / 2);
            }
        }
    }

    for (size_t i = memory.size() - 1; i >= 0; i--) {
        if (memory[i] == FREE_BLOCK) {
            continue;
        }
        size_t first_free_pos = i;
        for (size_t j = 0; j < i; j++) {
            if (memory[j] == FREE_BLOCK) {
                first_free_pos = j;
                break;
            }
        }
        if (first_free_pos == i) {
            break;
        }
        memory[first_free_pos] = memory[i];
        memory[i] = FREE_BLOCK;
    }

    // Better solution by Vulcagon
    // for (size_t i = 0; i < memory.size(); i++) {
    //     if (memory[i] == FREE_BLOCK) {
    //         memory[i] = memory[memory.size() - 1];
    //         memory.pop_back();
    //         while (memory[memory.size() - 1] == FREE_BLOCK) {
    //             memory.pop_back();
    //         }
    //     }
    // }

    long sum = 0;
    for (size_t i = 0; i < memory.size(); i++) {
        if (memory[i] != FREE_BLOCK) {
            sum += memory[i] * i;
        }
    }

    std::cout << "Sum of (ID * position in memory): " << sum << std::endl;

    return 0;
}