#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <unordered_set>
#include <algorithm>

#define N 2000
#define MOD_OP 16777216

std::vector<u_int64_t> calculate_secret_list(u_int64_t secret_init) {
    std::vector<u_int64_t> secret_list(N + 1);
    u_int64_t secret = secret_init;
    secret_list[0] = secret % 10;
    for (int i = 1; i <= N; i++) {
        secret = (secret ^ (secret * 64)) % MOD_OP;
        secret = (secret ^ (secret / 32)) % MOD_OP;
        secret = (secret ^ (secret * 2048)) % MOD_OP;
        secret_list[i] = secret % 10;
    }
    return secret_list;
}

std::vector<int> get_differences(const std::vector<u_int64_t>& secret_list) {
    std::vector<int> differences(secret_list.size() - 1);
    for (int i = 1; i <= N; i++) {
        differences[i - 1] = secret_list[i] - secret_list[i - 1];
    }
    return differences;
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

    std::vector<u_int64_t> secrect_inits;
    std::string line;
    while (std::getline(file, line)) {
        secrect_inits.push_back(std::stoull(line));
    }
    file.close();

    std::unordered_map<std::string, u_int64_t> global_secret_map;

    for (u_int64_t& secret_init : secrect_inits) {
        std::vector<u_int64_t> secret_list = calculate_secret_list(secret_init);
        std::vector<int> differences = get_differences(secret_list);
        assert(secret_list.size() == differences.size() + 1);
        std::unordered_map<std::string, u_int64_t> prices;
        for (size_t i = 0; i < differences.size() - 3; i++) {
            std::string quadruple = std::to_string(differences[i]) + std::to_string(differences[i + 1])
                + std::to_string(differences[i + 2]) + std::to_string(differences[i + 3]);
            if (prices.find(quadruple) == prices.end()) {
                prices[quadruple] = secret_list[i + 4];
                auto it = global_secret_map.find(quadruple);
                if (it == global_secret_map.end()) {
                    global_secret_map.insert({quadruple, secret_list[i + 4]});
                } else {
                    it->second += secret_list[i + 4];
                }
            }
        }
    }

    u_int64_t max_price = 0;
    std::string max_quadruple;
    for (const auto& [key, value] : global_secret_map) {
        if (value > max_price) {
            max_price = value;
            max_quadruple = key;
        }
    }

    std::cout << "Max price: " << max_price << std::endl;
    return 0;
}