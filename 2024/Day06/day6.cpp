#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

#define START_CHAR '^'
#define START_DIRECTION UP
#define OBSTRUCTION_CHAR '#'

typedef enum _direction {
    UP = '^',
    DOWN = 'v',
    LEFT = '<',
    RIGHT = '>'
} Direction;

bool is_next_move_on_map(const std::vector<std::string>& map,
    const std::pair<size_t, size_t>& pos, Direction dir) {
    switch (dir) {
        case UP:
            return pos.first > 0;
        case DOWN:
            return pos.first < map.size() - 1;
        case LEFT:
            return pos.second > 0;
        case RIGHT:
            return pos.second < map[pos.first].size() - 1;
        default:
            throw std::invalid_argument("Invalid direction");
    }
}

bool is_next_move_invalid(const std::vector<std::string>& map,
    const std::pair<size_t, size_t>& pos, Direction dir) {
    switch (dir) {
        case UP:
            return map[pos.first - 1][pos.second] == OBSTRUCTION_CHAR;
        case DOWN:
            return map[pos.first + 1][pos.second] == OBSTRUCTION_CHAR;
        case LEFT:
            return map[pos.first][pos.second - 1] == OBSTRUCTION_CHAR;
        case RIGHT:
            return map[pos.first][pos.second + 1] == OBSTRUCTION_CHAR;
        default:
            throw std::invalid_argument("Invalid direction");
    }
}

Direction rotate(Direction dir) {
    switch (dir) {
        case UP:
            return RIGHT;
        case DOWN:
            return LEFT;
        case LEFT:
            return UP;
        case RIGHT:
            return DOWN;
        default:
            throw std::invalid_argument("Invalid direction");
    }
}

std::pair<size_t, size_t> move(const std::pair<size_t, size_t>& pos, Direction dir) {
    switch (dir) {
        case UP:
            return std::make_pair(pos.first - 1, pos.second);
        case DOWN:
            return std::make_pair(pos.first + 1, pos.second);
        case LEFT:
            return std::make_pair(pos.first, pos.second - 1);
        case RIGHT:
            return std::make_pair(pos.first, pos.second + 1);
        default:
            throw std::invalid_argument("Invalid direction");
    }
}

bool has_cycle(
    std::vector<std::string> map,
    const std::pair<size_t, size_t>& start_pos,
    int& unique_visits
) {
    std::pair<size_t, size_t> current_pos = start_pos;
    Direction current_direction = START_DIRECTION;
    map[current_pos.first][current_pos.second] = START_DIRECTION;
    unique_visits = 1;

    while (is_next_move_on_map(map, current_pos, current_direction)) {
        if (is_next_move_invalid(map, current_pos, current_direction)) {
            current_direction = rotate(current_direction);
        } else {
            current_pos = move(current_pos, current_direction);
            if (map[current_pos.first][current_pos.second] != UP
                && map[current_pos.first][current_pos.second] != DOWN
                && map[current_pos.first][current_pos.second] != LEFT
                && map[current_pos.first][current_pos.second] != RIGHT) {
                map[current_pos.first][current_pos.second] = current_direction;
                unique_visits++;
            } else if (map[current_pos.first][current_pos.second] == current_direction) {
                return true;
            }
        }
    }
    return false;
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

    std::pair<size_t, size_t> start_pos;
    std::vector<std::string> map;
    std::string line;
    int line_pos = 0;
    while (std::getline(file, line)) {
        map.push_back(line);
        size_t column;
        if ((column = line.find(START_CHAR)) != std::string::npos) {
            start_pos = std::make_pair(line_pos, column);
        }
        line_pos++;
    }
    file.close();
    
    int unique_visits;
    bool hc = has_cycle(map, start_pos, unique_visits);
    assert(!hc);

    int positions_for_cycle = 0;
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            if (map[i][j] == OBSTRUCTION_CHAR || std::make_pair(i, j) == start_pos) {
                continue;
            }
            char orig_char = map[i][j];
            map[i][j] = OBSTRUCTION_CHAR;
            int not_used;
            if (has_cycle(map, start_pos, not_used)) {
                positions_for_cycle++;
            }
            map[i][j] = orig_char;
        }
    }

    std::cout << "Unique visits: " << unique_visits << std::endl;
    std::cout << "Positions for cycle: " << positions_for_cycle << std::endl;
    return 0;
}
