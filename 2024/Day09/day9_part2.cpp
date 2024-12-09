#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <list>

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

    std::list<std::pair<long, size_t>> memory;
    std::string input;
    size_t line_number = 0;
    while (std::getline(file, input)) {
        line_number++;
    }
    file.close();
    assert(line_number == 1);

    for (size_t i = 0; i < input.size(); i++) {
        size_t length = input[i] - '0';
        if (i % 2 == 1) {
            memory.push_back(std::make_pair(FREE_BLOCK, length));
        } else {
            memory.push_back(std::make_pair(i / 2, length));
        }
    }

    // for (auto& [id, size] : memory) {
    //     for (size_t i = 0; i < size; i++) {
    //         if (id == FREE_BLOCK) {
    //             std::cout << ". ";
    //         } else {
    //             std::cout << id << " ";
    //         }
    //     }
    // }

    for (auto it = memory.end(); it != memory.begin(); it--) {
        if (it->first == FREE_BLOCK) {
            continue;
        }
        for (auto it2 = memory.begin(); it2 != it; it2++) {
            if (it2->first == FREE_BLOCK && it2->second >= it->second) {
                memory.insert(it2, *it);
                if (it2->second > it->second) {
                    it2->second -= it->second;
                } else {
                    memory.erase(it2);
                }
                it->first = FREE_BLOCK;
                break;
            }
        }
    }

    // for (auto& [id, size] : memory) {
    //     for (size_t i = 0; i < size; i++) {
    //         if (id == FREE_BLOCK) {
    //             std::cout << ". ";
    //         } else {
    //             std::cout << id << " ";
    //         }
    //     }
    // }
    // std::cout << std::endl;

    long sum = 0;
    size_t pos = 0;
    for (auto& [id, size] : memory) {
        if (id != FREE_BLOCK) {
            for (size_t i = 0; i < size; i++) {
                sum += id * pos;
                pos++;
            }
        } else {
            pos += size;
        }
    }

    std::cout << "Sum of (ID * position in memory): " << sum << std::endl;

    return 0;
}