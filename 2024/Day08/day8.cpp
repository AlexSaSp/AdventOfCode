#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <regex>

#define ANTENNA_REGEX "[A-Za-z0-9]"

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

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

    //std::vector<std::string> map;
    std::unordered_map<char, std::vector<std::pair<size_t, size_t>>> antennas;
    std::unordered_set<std::pair<size_t, size_t>, pair_hash> antinodes_part1;
    std::unordered_set<std::pair<size_t, size_t>, pair_hash> antinodes_part2;
    std::string line;
    size_t line_number = 0;
    while (std::getline(file, line)) {
        //map.push_back(line);
        for (size_t i = 0; i < line.size(); i++) {
            if (std::regex_match(std::string(1, line[i]), std::regex(ANTENNA_REGEX))) {
                antennas[line[i]].push_back(std::make_pair(i, line_number));
            }
        }
        line_number++;
    }
    file.close();

    for (auto& antenna : antennas) {
        for (size_t i = 0; i < antenna.second.size(); i++) {
            for (size_t j = 0; j < antenna.second.size(); j++) {
                if (i == j) {
                    antinodes_part2.insert(antenna.second[i]);
                } else {
                    std::pair<size_t, size_t> distance
                        = std::make_pair(
                            antenna.second[j].first - antenna.second[i].first,
                            antenna.second[j].second - antenna.second[i].second
                        );
                    std::pair<size_t, size_t> antinode
                        = std::make_pair(
                            antenna.second[i].first - distance.first,
                            antenna.second[i].second - distance.second
                        );
                    if (antinode.first >= 0 && antinode.first < line.size()
                        && antinode.second >= 0 && antinode.second < line_number) {
                        antinodes_part1.insert(antinode);
                    }
                    while (antinode.first >= 0 && antinode.first < line.size()
                        && antinode.second >= 0 && antinode.second < line_number) {
                        //map[antinode.second][antinode.first] = '#';
                        antinodes_part2.insert(antinode);
                        antinode.first -= distance.first;
                        antinode.second -= distance.second;
                    }
                }
            }
        }
    }

    std::cout << "Number of antinodes part 1: " << antinodes_part1.size() << std::endl;
    std::cout << "Number of antinodes part 2: " << antinodes_part2.size() << std::endl;
    // for (auto& line : map) {
    //     std::cout << line << std::endl;
    // }
    return 0;
}