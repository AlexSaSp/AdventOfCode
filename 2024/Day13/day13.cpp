#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <regex>
#include <cassert>
#include <cmath>

#define DIM 2
#define COST_A 3
#define COST_B 1
#define EPS 0.001
#define PART_2_ADD 10000000000000


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

    std::vector<std::pair<std::array<std::array<double, DIM>, DIM>, std::array<double, DIM>>> input;
    std::string line;
    int line_number = 0;
    double cache[DIM + 1][DIM];
    while (std::getline(file, line)) {
        std::smatch match;
        std::regex_match(line, match, std::regex("(.*)X[\\+|=](\\d+)(.*)Y[\\+|=](\\d+)"));
        int i = 0;
        for (std::smatch::iterator it = match.begin(); it != match.end(); ++it) {
            switch (i) {
                case 2:
                    cache[line_number % (DIM + 2)][0] = std::stod(*it);
                    break;
                case 4:
                    cache[line_number % (DIM + 2)][1] = std::stod(*it);
                    break;
                default:
                    break;
            }
            i++;
        }
        if (line_number % (DIM + 2) == DIM) {
            std::array<std::array<double, DIM>, DIM> matrix;
            std::array<double, DIM> vector;

            for (int i = 0; i < DIM; i++) {
                for (int j = 0; j < DIM; j++) {
                    matrix[j][i] = cache[i][j];
                }
                vector[i] = cache[DIM][i] + PART_2_ADD; // Remove PART_2_ADD for part 1
            }

            input.push_back(std::make_pair(matrix, vector));
        }
        line_number++;
    }
    file.close();

    assert(DIM == 2);
    long total_costs = 0;
    for (auto& mashine : input) {
        std::array<std::array<double, DIM>, DIM> matrix = mashine.first;
        std::array<double, DIM> vector = mashine.second;

        matrix[0][1] /= matrix[0][0];
        vector[0] /= matrix[0][0];
        matrix[0][0] = 1;

        matrix[1][1] -= matrix[0][1] * matrix[1][0];
        vector[1] -= vector[0] * matrix[1][0];
        matrix[1][0] = 0;

        if (std::abs(matrix[1][1]) < EPS && std::abs(vector[1]) > EPS) {
            continue;
        }
        if (std::abs(matrix[1][1]) < EPS && std::abs(vector[1]) < EPS) {
            std::cerr << "Uncovered case" << std::endl;
            return 1;
        } else {
            double b = vector[1] / matrix[1][1];
            double a = (vector[0] - matrix[0][1] * b) / matrix[0][0];
            if (a >= 0 && b >= 0 && std::abs(a - std::round(a)) < EPS
                && std::abs(b - std::round(b)) < EPS) {
                total_costs += a * COST_A + b * COST_B;
            }
        }
    }
    std::cout << "Total costs: " << total_costs << std::endl;
    return 0;
}