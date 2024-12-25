#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define START_CHAR 'S'
#define END_CHAR 'E'
#define WALL_CHAR '#'
#define FREE_CHAR '.'
#define MIN_TIME_SAFES 100

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
    // map_copy = map;
    std::vector<std::pair<size_t, size_t>>::iterator it2 = race_path.begin();
    for (int i = 0; i < MIN_TIME_SAFES + 2; i++) {
        it2++;
    }
    for (auto it = race_path.begin(); it != race_path.end(); it++) {
        std::pair<size_t, size_t> step = *it;
        if (step.first < map.size() - 2 && map[step.first + 1][step.second] == WALL_CHAR
            && (map[step.first + 2][step.second] == FREE_CHAR 
            || (step.first < map.size() - 3 && map[step.first + 3][step.second] == FREE_CHAR))
            && std::find(it2, race_path.end(), std::make_pair(step.first + 2, step.second)) != race_path.end()) {
            cheats++;
            // map_copy[step.first + 1][step.second] = '1';
            // map_copy[step.first + 2][step.second] = '2';
        }
        if (step.second < map[step.first].size() - 2
            && map[step.first][step.second + 1] == WALL_CHAR
            && (map[step.first][step.second + 2] == FREE_CHAR
            || (step.second < map[step.first].size() - 3 && map[step.first][step.second + 3] == FREE_CHAR))
            && std::find(it2, race_path.end(), std::make_pair(step.first, step.second + 2)) != race_path.end()) {
            cheats++;
            // map_copy[step.first][step.second + 1] = '1';
            // map_copy[step.first][step.second + 2] = '2';
        }
        if (step.first > 1 && map[step.first - 1][step.second] == WALL_CHAR
            && (map[step.first - 2][step.second] == FREE_CHAR
            || (step.first > 2 && map[step.first - 3][step.second] == FREE_CHAR))
            && std::find(it2, race_path.end(), std::make_pair(step.first - 2, step.second)) != race_path.end()) {
            cheats++;
            // map_copy[step.first - 1][step.second] = '1';
            // map_copy[step.first - 2][step.second] = '2';
        }
        if (step.second > 1 && map[step.first][step.second - 1] == WALL_CHAR
            && (map[step.first][step.second - 2] == FREE_CHAR
            || (step.second > 2 && map[step.first][step.second - 3] == FREE_CHAR))
            && std::find(it2, race_path.end(), std::make_pair(step.first, step.second - 2)) != race_path.end()) {
            cheats++;
            // map_copy[step.first][step.second - 1] = '1';
            // map_copy[step.first][step.second - 2] = '2';
        }
        it2++;
    }

    std::cout << "Cheats: " << cheats << std::endl;
    // for (std::string line : map_copy) {
    //     std::cout << line << std::endl;
    // }
    return 0;
}