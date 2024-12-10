#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int get_score(std::vector<std::vector<int>>& map, const std::pair<size_t, size_t>& start) {
    if (map[start.first][start.second] == 9) {
        // map[start.first][start.second] = -1; // uncomment to get solution for part 1
        return 1;
        }
    int score = 0;
    if (start.first > 0 && map[start.first - 1][start.second] == map[start.first][start.second] + 1) {
        score += get_score(map, std::make_pair(start.first - 1, start.second));
    }
    if (start.first < map.size() - 1 && map[start.first + 1][start.second] == map[start.first][start.second] + 1) {
        score += get_score(map, std::make_pair(start.first + 1, start.second));
    }
    if (start.second > 0 && map[start.first][start.second - 1] == map[start.first][start.second] + 1) {
        score += get_score(map, std::make_pair(start.first, start.second - 1));
    }
    if (start.second < map[0].size() - 1 && map[start.first][start.second + 1] == map[start.first][start.second] + 1) {
        score += get_score(map, std::make_pair(start.first, start.second + 1));
    }
    return score;
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

    std::vector<std::pair<size_t, size_t>> trailheads;
    std::vector<std::vector<int>> map;
    std::string line;
    size_t line_number = 0;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == '0') trailheads.push_back(std::make_pair(line_number, i));
            row.push_back(line[i] - '0');
        }
        line_number++;
        map.push_back(row);
    }
    file.close();

    int sum_scores = 0;
    for (const auto& trailhead : trailheads) {
        std::vector<std::vector<int>> map_copy = map;
        int score = get_score(map_copy, trailhead);
        sum_scores += score;
    }
    std::cout << "Sum of scores: " << sum_scores << std::endl;
    return 0;
}