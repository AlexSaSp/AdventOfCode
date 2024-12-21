#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define delimiter std::string(", ")

long get_possible_designs(
    const std::string& design, const std::vector<std::string>& towels,
    std::map<std::string, long>& cache
    ) {
    if (cache.find(design) != cache.end()) {
        return cache[design];
    }
    long num_possible = 0;
    for (const std::string& towel : towels) {
        if (towel == design) {
            num_possible += 1;
        } else if (towel == design.substr(0, towel.length())) {
            num_possible = num_possible + get_possible_designs(design.substr(towel.length()), towels, cache);
        }
    }
    cache[design] = num_possible;
    return num_possible;
}

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

    std::vector<std::string> towels;
    std::vector<std::string> designs;

    std::string line;
    bool parsing_towels = true;
    while (std::getline(file, line)) {
        if (line.empty()) {
            parsing_towels = false;
            continue;
        }
        if (parsing_towels) {
            size_t pos_del = line.find(delimiter);
            while (pos_del != std::string::npos) {
                towels.push_back(line.substr(0, pos_del));
                line.erase(0, pos_del + delimiter.length());
                pos_del = line.find(delimiter);
            }
            towels.push_back(line);
        } else {
            designs.push_back(line);
        }
    }
    file.close();

    int possible_designs = 0;
    long design_variants = 0;
    std::map<std::string, long> cache;
    for (const std::string& design : designs) {
        long variants = get_possible_designs(design, towels, cache);
        if (variants > 0) {
            possible_designs++;
        }
        design_variants += variants;
    }

    std::cout << "Possible designs: " << possible_designs << std::endl;
    std::cout << "Design variants: " << design_variants << std::endl;
    return 0;
}