#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

typedef struct _node {
    char plant;
    std::vector<struct _node*> neighbors;
    std::pair<long, long> position;
} Node;

std::vector<Node> getRegion(Node& node) {
    std::vector<Node> region;
    node.plant = '#';
    region.push_back(node);
    for (Node* neighbor : node.neighbors) {
        if (neighbor->plant != '#') {
            std::vector<Node> neighbor_region = getRegion(*neighbor);
            region.insert(region.end(), neighbor_region.begin(), neighbor_region.end());
        }
    }
    return region;
}

bool isPositionInRegion(std::pair<long, long> position, std::vector<Node>& region) {
    for (Node& node : region) {
        if (node.position == position) {
            return true;
        }
    }
    return false;
}

int innerCorner(Node& node, std::vector<Node>& region) {
    int corners = 0;
    if (isPositionInRegion(std::make_pair(node.position.first - 1, node.position.second), region)
        && isPositionInRegion(std::make_pair(node.position.first, node.position.second - 1), region)
        && !isPositionInRegion(std::make_pair(node.position.first - 1, node.position.second - 1), region)) {
            corners++;
    }
    if (isPositionInRegion(std::make_pair(node.position.first - 1, node.position.second), region)
        && isPositionInRegion(std::make_pair(node.position.first, node.position.second + 1), region)
        && !isPositionInRegion(std::make_pair(node.position.first - 1, node.position.second + 1), region)) {
            corners++;
    }
    if (isPositionInRegion(std::make_pair(node.position.first + 1, node.position.second), region)
        && isPositionInRegion(std::make_pair(node.position.first, node.position.second - 1), region)
        && !isPositionInRegion(std::make_pair(node.position.first + 1, node.position.second - 1), region)) {
            corners++;
    }
    if (isPositionInRegion(std::make_pair(node.position.first + 1, node.position.second), region)
        && isPositionInRegion(std::make_pair(node.position.first, node.position.second + 1), region)
        && !isPositionInRegion(std::make_pair(node.position.first + 1, node.position.second + 1), region)) {
            corners++;
    }
    return corners;
}

int outerCorner(Node& node, std::vector<Node>& region) {
    int corners = 0;
    if (!isPositionInRegion(std::make_pair(node.position.first - 1, node.position.second), region)
        && !isPositionInRegion(std::make_pair(node.position.first, node.position.second - 1), region)) {
            corners++;
    }
    if (!isPositionInRegion(std::make_pair(node.position.first - 1, node.position.second), region)
        && !isPositionInRegion(std::make_pair(node.position.first, node.position.second + 1), region)) {
            corners++;
    }
    if (!isPositionInRegion(std::make_pair(node.position.first + 1, node.position.second), region)
        && !isPositionInRegion(std::make_pair(node.position.first, node.position.second - 1), region)) {
            corners++;
    }
    if (!isPositionInRegion(std::make_pair(node.position.first + 1, node.position.second), region)
        && !isPositionInRegion(std::make_pair(node.position.first, node.position.second + 1), region)) {
            corners++;
    }
    return corners;
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

    std::vector<std::vector<Node>> input;
    std::string line;
    size_t line_number = 0;
    while (std::getline(file, line)) {
        std::vector<Node> row;
        for (size_t i = 0; i < line.size(); i++) {
            Node node;
            node.plant = line[i];
            node.position = std::make_pair(line_number, i);
            row.push_back(node);
        }
        input.push_back(row);
        line_number++;
    }
    file.close();

    // get neighbours of each node
    for (size_t i = 0; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            if (i > 0 && input[i - 1][j].plant == input[i][j].plant) {
                input[i][j].neighbors.push_back(&input[i - 1][j]);
            }
            if (i < input.size() - 1 && input[i + 1][j].plant == input[i][j].plant) {
                input[i][j].neighbors.push_back(&input[i + 1][j]);
            }
            if (j > 0 && input[i][j - 1].plant == input[i][j].plant) {
                input[i][j].neighbors.push_back(&input[i][j - 1]);
            }
            if (j < input[i].size() - 1 && input[i][j + 1].plant == input[i][j].plant) {
                input[i][j].neighbors.push_back(&input[i][j + 1]);
            }
        }
    }

    // get regions
    std::vector<std::vector<Node>> regions;
    for (size_t i = 0; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            if (input[i][j].plant != '#') {
                std::vector<Node> region = getRegion(input[i][j]);
                regions.push_back(region);
            }
        }
    }

    // solve part 1
    int sum_area_perimeter = 0;
    for (auto& region : regions) {
        int area = region.size();
        int perimeter = 0;
        for (auto& node : region) {
            perimeter += 4 - node.neighbors.size();
        }
        sum_area_perimeter += area * perimeter;
    }

    // solve part 2
    int sum_area_sides = 0;
    for (auto& region : regions) {
        int area = region.size();
        int sides = 0;
        for (auto& node : region) {
            sides += innerCorner(node, region);
            sides += outerCorner(node, region);
        }
        sum_area_sides += area * sides;
    }

    std::cout << "Sum of area * perimeter: " << sum_area_perimeter << std::endl;
    std::cout << "Sum of area * side: " << sum_area_sides << std::endl;
}