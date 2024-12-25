#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <map>

namespace std {
    template <>
    struct hash<std::pair<size_t, size_t>> {
        size_t operator()(const std::pair<size_t, size_t>& p) const {
            auto hash1 = std::hash<size_t>{}(p.first);
            auto hash2 = std::hash<size_t>{}(p.second);
            return hash1 ^ (hash2 << 1);
        }
    };
}

#define START_CHAR 'S'
#define END_CHAR 'E'
#define WALL_CHAR '#'
#define FREE_CHAR '.'
#define MIN_TIME_SAFES 100
#define CHEAT_LENGTH 20

size_t getCheats(std::pair<size_t, size_t> start, std::vector<std::string>& map,
    size_t index, std::map<std::pair<size_t, size_t>, size_t>& all_indices) {
    std::vector<std::vector<long>> distances(map.size(), std::vector<long>(map[0].size()));
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[0].size(); j++) {
            distances[i][j] = -1;
        }
    }
    distances[start.first][start.second] = 0;

    size_t cheats = 0;

    std::queue<std::pair<size_t, size_t>> q;
    q.push(start);

    while (!q.empty()) {
        std::pair<size_t, size_t> current = q.front();
        q.pop();
        if (map[current.first][current.second] == FREE_CHAR && current != start) {
            if (all_indices[current] >= index + MIN_TIME_SAFES + distances[current.first][current.second]) {
                cheats++;
            }
        }
        if (distances[current.first][current.second] >= CHEAT_LENGTH) {
            continue;
        }
        if (current.first > 0) {
            if (distances[current.first - 1][current.second] == -1) {
                distances[current.first - 1][current.second] = distances[current.first][current.second] + 1;
                q.push(std::make_pair(current.first - 1, current.second));
            }
        }
        if (current.second > 0) {
            if (distances[current.first][current.second - 1] == -1) {
                distances[current.first][current.second - 1] = distances[current.first][current.second] + 1;
                q.push(std::make_pair(current.first, current.second - 1));
            }
        }
        if (current.first < map.size() - 1) {
            if (distances[current.first + 1][current.second] == -1) {
                distances[current.first + 1][current.second] = distances[current.first][current.second] + 1;
                q.push(std::make_pair(current.first + 1, current.second));
            }
        }
        if (current.second < map[0].size() - 1) {
            if (distances[current.first][current.second + 1] == -1) {
                distances[current.first][current.second + 1] = distances[current.first][current.second] + 1;
                q.push(std::make_pair(current.first, current.second + 1));
            }
        }
    }
    return cheats;
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

    std::vector<std::string> map;
    std::pair<size_t, size_t> start;
    std::pair<size_t, size_t> end;

    std::string line;
    size_t line_number = 0;
    while (std::getline(file, line)) {
        map.push_back(line);
        size_t index_start = line.find(START_CHAR);
        if (index_start != std::string::npos) {
            start = std::make_pair(line_number, index_start);
        }
        size_t index_end = line.find(END_CHAR);
        if (index_end != std::string::npos) {
            end = std::make_pair(line_number, index_end);
        }
        line_number++;
    }
    file.close();

    std::vector<std::pair<size_t, size_t>> race_path;
    std::pair<size_t, size_t> current = start;
    std::vector<std::string> map_copy = map;
    map_copy[end.first][end.second] = FREE_CHAR;
    race_path.push_back(current);
    while (current != end) {
        map_copy[current.first][current.second] = WALL_CHAR;
        if (current.first < map_copy.size() - 1 && map_copy[current.first + 1][current.second] == FREE_CHAR) {
            current.first++;
        } else if (current.second < map_copy[current.first].size() - 1
            && map_copy[current.first][current.second + 1] == FREE_CHAR) {
            current.second++;
        } else if (current.first > 0 && map_copy[current.first - 1][current.second] == FREE_CHAR) {
            current.first--;
        } else if (current.second > 0 && map_copy[current.first][current.second - 1] == FREE_CHAR) {
            current.second--;
        } else {
            throw std::runtime_error("Race path could not get constructed");
        }
        race_path.push_back(current);
    }

    size_t cheats = 0;
    map[end.first][end.second] = FREE_CHAR;

    std::map<std::pair<size_t, size_t>, size_t> all_indices;
    for (size_t i = 0; i < race_path.size(); i++) {
        all_indices[race_path[i]] = i;
    }

    for (size_t i = 0; i < race_path.size(); i++) {
        cheats += getCheats(race_path[i], map, i, all_indices);
    }

    std::cout << "Cheats: " << cheats << std::endl;
    // for (std::string line : map_copy) {
    //     std::cout << line << std::endl;
    // }
    return 0;
}