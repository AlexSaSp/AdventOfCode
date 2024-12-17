#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

#define OBS 'O'
#define EMPTY '.'
#define WALL '#'
#define POS '@'
#define BOX_L '['
#define BOX_R ']'

typedef enum _direction {
    UP = '^',
    DOWN = 'v',
    LEFT = '<',
    RIGHT = '>'
} Direction;

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

bool is_move_valid(const std::pair<size_t, size_t>& pos, Direction dir, const std::vector<std::string>& map) {
    if (!is_next_move_on_map(map, pos, dir)) {
        return false;
    }
    std::pair<size_t, size_t> next_pos = move(pos, dir);
    if (map[next_pos.first][next_pos.second] == WALL) {
        return false;
    }
    if (map[next_pos.first][next_pos.second] == OBS) {
        return is_move_valid(next_pos, dir, map);
    }
    if (map[next_pos.first][next_pos.second] == BOX_L) {
        return is_move_valid(next_pos, dir, map)
            && ((next_pos.second + 1 != pos.second)
                ? is_move_valid(std::make_pair(next_pos.first, next_pos.second + 1), dir, map)
                : true);
    }
    if (map[next_pos.first][next_pos.second] == BOX_R) {
        return is_move_valid(next_pos, dir, map)
            && ((next_pos.second - 1 != pos.second)
                ? is_move_valid(std::make_pair(next_pos.first, next_pos.second - 1), dir, map)
                : true);
    }
    return true;
}

void move_obstacles(const std::pair<size_t, size_t>& pos, Direction dir, std::vector<std::string>& map) {
    if (map[pos.first][pos.second] == OBS) {
        map[pos.first][pos.second] = EMPTY;
        if (is_next_move_on_map(map, pos, dir)) {
            std::pair<size_t, size_t> next_pos = move(pos, dir);
            move_obstacles(next_pos, dir, map);
            map[next_pos.first][next_pos.second] = OBS;
        }
    } else if (map[pos.first][pos.second] == BOX_L && map[pos.first][pos.second + 1] == BOX_R) {
        map[pos.first][pos.second] = EMPTY;
        map[pos.first][pos.second + 1] = EMPTY;
        if (is_next_move_on_map(map, pos, dir)) {
            std::pair<size_t, size_t> next_pos = move(pos, dir);
            std::pair<size_t, size_t> next_pos_r = std::make_pair(next_pos.first, next_pos.second + 1);
            move_obstacles(next_pos, dir, map);
            move_obstacles(next_pos_r, dir, map);
            map[next_pos.first][next_pos.second] = BOX_L;
            map[next_pos_r.first][next_pos_r.second] = BOX_R;
        }
    } else if (map[pos.first][pos.second] == BOX_R) {
        move_obstacles(std::make_pair(pos.first, pos.second - 1), dir, map);
    }
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
    std::queue<Direction> directions;

    std::string line;
    bool parsing_map = true;
    size_t line_number = 0;
    while (std::getline(file, line)) {
        if (line.empty()) {
            parsing_map = false;
            continue;
        }
        if (parsing_map) {
            std::string oldLine = line;
            line = "";
            for (char c : oldLine) {
                switch (c) {
                    case OBS:
                        line.push_back(BOX_L);
                        line.push_back(BOX_R);
                        break;
                    case EMPTY:
                    case WALL:
                        line.append(2, c);
                        break;
                    case POS:
                        line.push_back(POS);
                        line.push_back(EMPTY);
                        break;
                }
            } 
            size_t start_x = line.find_first_of(POS);
            if (start_x != std::string::npos) {
                start_pos = std::make_pair(line_number, start_x);
            }
            map.push_back(line);
            line_number++;
        } else {
            for (char c : line) {
                directions.push(static_cast<Direction>(c));
            }
        }
    }
    file.close();

    // std::cout << "file parsed" << std::endl;
    // for (size_t i = 0; i < map.size(); i++) {
    //     std::cout << map[i] << std::endl;
    // }
    std::pair<size_t, size_t> current_pos = start_pos;
    while (!directions.empty()) {
        Direction current_direction = directions.front();
        directions.pop();
        if (is_move_valid(current_pos, current_direction, map)) {
            map[current_pos.first][current_pos.second] = EMPTY;
            current_pos = move(current_pos, current_direction);
            move_obstacles(current_pos, current_direction, map);
            map[current_pos.first][current_pos.second] = POS;
        }
        // std::cout << "After 1 move" << std::endl;
        // for (size_t i = 0; i < map.size(); i++) {
        //     std::cout << map[i] << std::endl;
        // } 
    }

    long sum_gps = 0;
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            if (map[i][j] == BOX_L) {
                sum_gps += 100 * i + j;
            }
        }
    }

    std::cout << "Sum of boxes' gps: " << sum_gps << std::endl;
    return 0;
}