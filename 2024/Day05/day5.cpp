#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#define DELIMITER_RULES std::string("|")
#define DELIMITER_PRINT std::string(",")

/**
 * Checks if b should come before a in the print order
 */
bool should_come_before(
    const std::unordered_map<std::string, std::vector<std::string>>& rules,
    const std::string& a,
    const std::string& b
){
    if (a == b || rules.find(a) == rules.end()) {
        return false;
    }

    for (const auto& rule : rules.at(a)) {
        if (rule == b) {
            return true;
        }
    }

    return false;
}

/**
 * Checks if the given print order is valid and corrects the order if not valid
 */
bool check_and_correct_order(
    const std::unordered_map<std::string, std::vector<std::string>>& rules,
    std::vector<std::string>& order
) {
    bool was_correct = true;
    for (size_t i = 0; i < order.size(); i++) {
        for (size_t j = i + 1; j < order.size(); j++) {
            if (should_come_before(rules, order[i], order[j])) {
                was_correct = false;
                std::string temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }
    return was_correct;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    std::ifstream file(argv[1], std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    std::unordered_map<std::string, std::vector<std::string>> rules;
    std::vector<std::vector<std::string>> print_orders;

    // parsing input
    std::string line;
    bool parsing_rules = true;
    while (std::getline(file, line)) {
        if (line.empty()) {
            parsing_rules = false;
            continue;
        }
        if (parsing_rules) {
            rules[line.substr(line.find(DELIMITER_RULES) + DELIMITER_RULES.length())]
                .push_back(line.substr(0, line.find(DELIMITER_RULES)));
        } else {
            std::vector<std::string> order;
            size_t pos = 0;
            while ((pos = line.find(DELIMITER_PRINT)) != std::string::npos) {
                order.push_back(line.substr(0, pos));
                line.erase(0, pos + DELIMITER_PRINT.length());
            }
            order.push_back(line);
            print_orders.push_back(order);
        }
    }
    file.close();

    // solve the problem
    int sum_middle_correct_orders = 0;
    int sum_middle_incorrect_orders = 0;
    for (auto& order : print_orders) {
        if (check_and_correct_order(rules, order)) {
            sum_middle_correct_orders += std::stoi(order[(order.size() - 1) / 2]);
        } else {
            sum_middle_incorrect_orders += std::stoi(order[(order.size() - 1) / 2]);
        }
    }
    std::cout << "Sum of middles of correct orders: " << sum_middle_correct_orders << std::endl;
    std::cout << "Sum of middles of incorrect orders: " << sum_middle_incorrect_orders << std::endl;
    return 0;
}