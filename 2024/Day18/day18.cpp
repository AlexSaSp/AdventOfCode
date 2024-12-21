#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <cassert>

#define delimiter ","
#define START std::make_pair(0, 0)
#define END std::make_pair(70, 70)
#define NUM_CORRUPTED 1024

std::vector<std::pair<int, int>> get_neighbors(
    std::pair<int, int> current,
    std::set<std::pair<int, int>>& corrupted
    ) {
    std::vector<std::pair<int, int>> neighbors;
    if (current.first > 0
        && corrupted.find(std::make_pair(current.first - 1, current.second)) == corrupted.end()) {
        neighbors.push_back(std::make_pair(current.first - 1, current.second));
    }
    if (current.first < END.first
        && corrupted.find(std::make_pair(current.first + 1, current.second)) == corrupted.end()) {
        neighbors.push_back(std::make_pair(current.first + 1, current.second));
    }
    if (current.second > 0
        && corrupted.find(std::make_pair(current.first, current.second - 1)) == corrupted.end()) {
        neighbors.push_back(std::make_pair(current.first, current.second - 1));
    }
    if (current.second < END.second
        && corrupted.find(std::make_pair(current.first, current.second + 1)) == corrupted.end()) {
        neighbors.push_back(std::make_pair(current.first, current.second + 1));
    }
    return neighbors;
}

int find_shortest(std::set<std::pair<int, int>>& corrupted) {
    int distances[END.first + 1][END.second + 1];
    for (int i = 0; i <= END.first; i++) {
        for (int j = 0; j <= END.second; j++) {
            distances[i][j] = -1;
        }
    }
    distances[START.first][START.second] = 0;

    std::queue<std::pair<int, int>> q;
    q.push(START);

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();
        for (std::pair<int, int> neighbor : get_neighbors(current, corrupted)) {
            if (distances[neighbor.first][neighbor.second] == -1) {
                distances[neighbor.first][neighbor.second] = distances[current.first][current.second] + 1;
                q.push(neighbor);
            }
            if (neighbor == END) {
                return distances[neighbor.first][neighbor.second];
            }
        }
    }
    return -1;
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

    std::vector<std::pair<int, int>> all_corrupted;
    std::set<std::pair<int, int>> corrupted;

    std::string line;
    while (std::getline(file, line)) {
        int pos_delimiter = line.find(delimiter);
        int x = std::stoi(line.substr(0, pos_delimiter));
        int y = std::stoi(line.substr(pos_delimiter + 1, line.length() - pos_delimiter - 1));
        all_corrupted.push_back(std::make_pair(x, y));
    }
    file.close();
    for (int i = 0; i < NUM_CORRUPTED; i++) {
        corrupted.insert(all_corrupted[i]);
    }

    int steps = find_shortest(corrupted);
    assert(steps != -1);
    std::cout << "Shortest path: " << steps << std::endl;

    for (size_t i = NUM_CORRUPTED; i < all_corrupted.size(); i++) {
        corrupted.insert(all_corrupted[i]);
        steps = find_shortest(corrupted);
        if (steps == -1) {
            std::cout << "First byte that cuts off exit: " 
                << all_corrupted[i].first << "," << all_corrupted[i].second << std::endl;
            break;
        }
    }
    return 0;
}