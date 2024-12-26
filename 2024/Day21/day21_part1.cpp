#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include <cassert>

#define UP '^'
#define DOWN 'v'
#define LEFT '<'
#define RIGHT '>'
#define PUSH 'A'
#define INDIRECTIONS 2
#define EMPTY ' '

std::string keypad_numeric[] = {
    "789",
    "456",
    "123",
    " 0A"
};

std::string keypad_directional[] = {
    " ^A",
    "<v>"
};

std::map<char, std::pair<int, int>> get_indices_numeric() {
    std::map<char, std::pair<int, int>> indices;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 3; j++) {
            indices[keypad_numeric[i][j]] = std::make_pair(i, j);
        }
    }
    return indices;
}

std::map<char, std::pair<int, int>> get_indices_directional() {
    std::map<char, std::pair<int, int>> indices;
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 3; j++) {
            indices[keypad_directional[i][j]] = std::make_pair(i, j);
        }
    }
    return indices;
}

std::vector<std::string> get_directions_bfs(std::pair<int, int> start, std::pair<int, int> end,
    std::pair<int, int> forbidden) {
    std::vector<std::string> directions;
    std::queue<std::pair<std::pair<int, int>, std::string>> q;
    q.push(std::make_pair(start, ""));
    bool end_reached = false;
    while (!q.empty()) {
        std::pair<int, int> current = q.front().first;
        std::string path = q.front().second;
        q.pop();
        if (current == end) {
            directions.push_back(path + PUSH);
            end_reached = true;
        }
        if (end_reached) {
            continue;
        }
        if (current.first > 0 && std::make_pair(current.first - 1, current.second) != forbidden) {
            q.push(std::make_pair(std::make_pair(current.first - 1, current.second), path + UP));
        }
        if (current.first < 3 && std::make_pair(current.first + 1, current.second) != forbidden) {
            q.push(std::make_pair(std::make_pair(current.first + 1, current.second), path + DOWN));
        }
        if (current.second > 0 && std::make_pair(current.first, current.second - 1) != forbidden) {
            q.push(std::make_pair(std::make_pair(current.first, current.second - 1), path + LEFT));
        }
        if (current.second < 2 && std::make_pair(current.first, current.second + 1) != forbidden) {
            q.push(std::make_pair(std::make_pair(current.first, current.second + 1), path + RIGHT));
        }
    }
    auto it_min = std::min_element(directions.begin(), directions.end(),
        [](const std::string& a, const std::string& b) {
            return a.size() < b.size();
        });
    for (auto it = directions.begin(); it != directions.end(); ) {
        if (it->size() > it_min->size()) {
            it = directions.erase(it);
        } else {
            it++;
        }
    }
    return directions;
}

std::vector<std::string> get_directions(std::pair<int, int> start, std::pair<int, int> end,
    std::pair<int, int> forbidden) {
    int y_diff = end.first - start.first;
    int x_diff = end.second - start.second;

    std::vector<std::string> directions;
    std::string direction1 = "";
    if (x_diff >= 0) {
        direction1 += std::string(x_diff, RIGHT);
    } else {
        direction1 += std::string(-x_diff, LEFT);
    }
    if (y_diff < 0) {
        direction1 += std::string(-y_diff, UP);
    } else {
        direction1 += std::string(y_diff, DOWN);
    }
    if (start.first != forbidden.first) {
        directions.push_back(direction1 + PUSH);
    }

    std::string direction2 = "";
    if (y_diff < 0) {
        direction2 += std::string(-y_diff, UP);
    } else {
        direction2 += std::string(y_diff, DOWN);
    }
    if (x_diff >= 0) {
        direction2 += std::string(x_diff, RIGHT);
    } else {
        direction2 += std::string(-x_diff, LEFT);
    }
    if (start.second != forbidden.second) {
        directions.push_back(direction2 + PUSH);
    }

    if (direction1 == direction2) {
        directions.clear();
        directions.push_back(direction1 + PUSH);
    }

    return directions;
}

std::vector<std::string> get_paths(std::string code, std::map<char, std::pair<int, int>>& indices,
    bool bfs = false) {
    std::vector<std::string> paths;
    for (size_t i = 1; i < code.size(); i++) {
        std::pair<int, int> start = indices[code[i - 1]];
        std::pair<int, int> end = indices[code[i]];
        std::pair<int, int> forbidden = indices[EMPTY];
        std::vector<std::string> path =
            bfs
            ? get_directions_bfs(start, end, forbidden)
            : get_directions(start, end, forbidden);
        assert(!path.empty());
        if (paths.empty()) {
            paths.insert(paths.end(), path.begin(), path.end());
        } else {
            std::vector<std::string> new_paths;
            for (std::string old_path : paths) {
                for (std::string new_path : path) {
                    new_paths.push_back(old_path + new_path);
                }
            }
            paths = new_paths;
        }
    }
    return paths;
}

int get_num(std::string code) {
    int number = 0;
    for (size_t i = 0; i < code.size(); i++) {
        if (code[i] - '0' < 10 && code[i] - '0' >= 0) {
            number = number * 10 + code[i] - '0';;
        }
    }
    return number;
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
    
    std::vector<std::string> codes;
    std::string line;
    while (std::getline(file, line)) {
        codes.push_back(PUSH + line);
    }

    std::map<char, std::pair<int, int>> indices_numeric = get_indices_numeric();
    std::map<char, std::pair<int, int>> indices_directional = get_indices_directional();

    std::vector<std::string> keypad_num(keypad_numeric,
        keypad_numeric + sizeof(keypad_numeric) / sizeof(keypad_numeric[0]));
    std::vector<std::string> keypad_dir(keypad_directional,
        keypad_directional + sizeof(keypad_directional) / sizeof(keypad_directional[0]));

    std::vector<long> min_moves;
    
    for (auto code : codes) {
        std::vector<std::string> directions1 = get_paths(code, indices_numeric, true);
        for (size_t i = 0; i < INDIRECTIONS; i++) {
            std::vector<std::string> directions2;
            for (auto dir : directions1) {
                std::vector<std::string> directions = get_paths(PUSH + dir, indices_directional);
                directions2.insert(directions2.end(), directions.begin(), directions.end());
            }
            auto it_min = std::min_element(directions2.begin(), directions2.end(),
                [](const std::string& a, const std::string& b) {
                return a.size() < b.size();
                });
            directions1.clear();
            for (auto dir : directions2) {
                if (dir.size() == it_min->size()) {
                    directions1.push_back(dir);
                }
            }
        }
        
        min_moves.push_back(directions1[0].size());
    }

    assert(min_moves.size() == codes.size());
    size_t complexity = 0;
    for (size_t i = 0; i < min_moves.size(); i++) {
        complexity += min_moves[i] * get_num(codes[i]);
    }

    std::cout << "Complexity: " << complexity << std::endl;
    return 0;
}