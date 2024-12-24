#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cassert>

#define START 'S'
#define END 'E'
#define FREE '.'
#define COST 1
#define COST_TURN 1000

typedef enum _direction {
    UP = 0,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 1
} Direction;

char printDir(Direction dir) {
    switch (dir) {
        case UP:
            return '^';
        case DOWN:
            return 'v';
        case LEFT:
            return '<';
        case RIGHT:
            return '>';
    }
    return ' ';
}

struct Node {
    std::pair<size_t, size_t> pos;
    Direction direction;
    std::vector<Node*> neighbors;
    std::vector<Node*> before;

    Node() : pos(std::make_pair(0, 0)), direction(UP) {}
    Node(std::pair<size_t, size_t> p, Direction dir)
        : pos(p), direction(dir) {}
};

struct pairLess {
    bool operator()(const std::pair<size_t, Node*>& a, const std::pair<size_t, Node*>& b) {
        return a.first > b.first;
    }
};

void get_neighbors(Node& node, const std::vector<std::string>& map, 
        std::map<std::pair<Direction, std::pair<size_t, size_t>>, Node>& allNodes) {
    std::vector<Node*> neighbors;
    if (node.pos.first > 0 && map[node.pos.first - 1][node.pos.second] == FREE) {
        neighbors.push_back(
            &allNodes[std::make_pair(UP,std::make_pair(node.pos.first - 1, node.pos.second))]);
    }
    if (node.pos.first < map.size() - 1 && map[node.pos.first + 1][node.pos.second] == FREE) {
        neighbors.push_back(
            &allNodes[std::make_pair(DOWN, std::make_pair(node.pos.first + 1, node.pos.second))]);
    }
    if (node.pos.second > 0 && map[node.pos.first][node.pos.second - 1] == FREE) {
        neighbors.push_back(
            &allNodes[std::make_pair(LEFT, std::make_pair(node.pos.first, node.pos.second - 1))]);
    }
    if (node.pos.second < map[node.pos.first].size()
        && map[node.pos.first][node.pos.second + 1] == FREE) {
        neighbors.push_back(
            &allNodes[std::make_pair(RIGHT, std::make_pair(node.pos.first, node.pos.second + 1))]);
    }
    node.neighbors = neighbors;
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
    std::map<std::pair<Direction, std::pair<size_t, size_t>>, Node> nodes;
    std::map<std::pair<Direction, std::pair<size_t, size_t>>, size_t> distances;
    std::priority_queue<std::pair<size_t, Node*>, std::vector<std::pair<size_t, Node*>>, pairLess> q;
    std::pair<size_t, size_t> start;
    std::pair<size_t, size_t> end;
    
    std::string line;
    size_t line_number = 0;
    while (std::getline(file, line)) {
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == START) {
                line[i] = FREE;
                start = std::make_pair(line_number, i);
            } else if (line[i] == END) {
                line[i] = FREE;
                end = std::make_pair(line_number, i);
            }
            if (line[i] == FREE) {
                nodes[std::make_pair(UP, std::make_pair(line_number, i))]
                    = Node(std::make_pair(line_number, i), UP);
                nodes[std::make_pair(DOWN, std::make_pair(line_number, i))]
                    = Node(std::make_pair(line_number, i), DOWN);
                nodes[std::make_pair(LEFT, std::make_pair(line_number, i))]
                    = Node(std::make_pair(line_number, i), LEFT);
                nodes[std::make_pair(RIGHT, std::make_pair(line_number, i))]
                    = Node(std::make_pair(line_number, i), RIGHT);
            }
        }
        map.push_back(line);
        line_number++;
    }
    file.close();

    for (auto& node : nodes) {
        get_neighbors(node.second, map, nodes);
    }

    for (auto& node : nodes) {
        if (node.first == std::make_pair(RIGHT, start)) {
            q.push(std::make_pair(0, &node.second));
            distances[node.first] = 0;
        } else {
            q.push(std::make_pair(__LONG_MAX__, &node.second));
            distances[node.first] = __LONG_MAX__;
        }
    }
    
    Node* current_node = q.top().second;
    while (!q.empty()) {
        current_node = q.top().second;
        size_t current_distance = q.top().first;
        q.pop();
        if (current_node->pos == end) {
            break;
        }
        if (current_distance > distances[std::make_pair(current_node->direction, current_node->pos)]) {
            continue;
        }
        for (auto& neighbour : current_node->neighbors) {
            size_t new_distance = current_distance + COST
                + COST_TURN * (
                    (std::abs(current_node->direction - neighbour->direction) == 3)
                    ? 1 : std::abs(current_node->direction - neighbour->direction));
            if (distances[std::make_pair(neighbour->direction, neighbour->pos)] > new_distance) {
                distances[std::make_pair(neighbour->direction, neighbour->pos)] = new_distance;
                neighbour->before.clear();
                neighbour->before.push_back(current_node);
                q.push(std::make_pair(new_distance, neighbour));
            } else if (distances[std::make_pair(neighbour->direction, neighbour->pos)] == new_distance) {
                neighbour->before.push_back(current_node);
            }
        }
    }

    size_t score = distances[std::make_pair(current_node->direction, current_node->pos)];
    std::cout << "The minimum score is: " << score << std::endl;

    std::queue<Node*> q_path;
    std::set<std::pair<size_t, size_t>> visited;
    q_path.push(current_node);
    Node* current_path = current_node;
    while (!q_path.empty()) {
        current_path = q_path.front();
        q_path.pop();
        for (auto& before : current_path->before) {
            q_path.push(before);
        }
        visited.insert(current_path->pos);
    }

    std::cout << "Tiles at best paths: " << visited.size() << std::endl;
    
    // for (size_t i = 0; i < map.size(); i++) {
    //     for (size_t j = 0; j < map[i].size(); j++) {
    //         if (visited.find(std::make_pair(i, j)) != visited.end()) {
    //             std::cout << 'o';
    //         } else {
    //             std::cout << map[i][j];
    //         }
    //     }
    //     std::cout << std::endl;
    // }
    return 0;
}