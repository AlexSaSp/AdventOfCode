#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cassert>

#define DELIMITER '-'
#define CHIEF 't'

bool is_interconnected(const std::set<std::string>& network,
    const std::map<std::string, std::set<std::string>>& neighbours)
    {
    for (const auto& computer : network) {
        size_t degree = 0;
        for (const auto& neighbour : neighbours.at(computer)) {
            if (network.find(neighbour) != network.end()) {
                degree++;
            }
        }
        if (degree != network.size() - 1) {
            return false;
        }
    }
    return true;
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

    std::map<std::string, std::set<std::string>> neighbours;
    std::string line;
    while (getline(file, line)) {
        size_t del = line.find(DELIMITER);
        std::string first_el = line.substr(0, del);
        std::string second_el = line.substr(del + 1);
        neighbours[first_el].insert(second_el);
        neighbours[second_el].insert(first_el);
    }
    file.close();

    std::set<std::set<std::string>> three_interconnected;
    long three_interconnected_count = 0;

    std::set<std::set<std::string>> biggest_interconnected;

    std::set<std::string> seen_1st;
    for (const auto& [computer1, computer1_neighbours] : neighbours) {
        if (true || computer1[0] == CHIEF) { // remove true to get correct result for part 1
            seen_1st.insert(computer1);
            std::set<std::string> seen_2nd;
            for (const auto& computer2 : computer1_neighbours) {
                // remove true to get correct result for part 1
                if ((true || computer2[0] == CHIEF) && seen_1st.find(computer2) != seen_1st.end()) continue;
                seen_2nd.insert(computer2);

                std::set<std::string> intersection;
                std::set_intersection(computer1_neighbours.begin(), computer1_neighbours.end(),
                    neighbours[computer2].begin(), neighbours[computer2].end(),
                    std::inserter(intersection, intersection.begin()));

                std::set<std::string> intersect_seen_1st;
                std::set_intersection(intersection.begin(), intersection.end(),
                    seen_1st.begin(), seen_1st.end(),
                    std::inserter(intersect_seen_1st, intersect_seen_1st.begin()));

                std::set<std::string> intersect_seen_2nd;
                std::set_intersection(intersection.begin(), intersection.end(),
                    seen_2nd.begin(), seen_2nd.end(),
                    std::inserter(intersect_seen_2nd, intersect_seen_2nd.begin()));

                std::set<std::string> final_intersection;
                std::set_difference(intersection.begin(), intersection.end(),
                    intersect_seen_2nd.begin(), intersect_seen_2nd.end(),
                    std::inserter(final_intersection, final_intersection.begin()));

                std::set<std::string> final_intersection_2;
                std::set_difference(final_intersection.begin(), final_intersection.end(),
                    intersect_seen_1st.begin(), intersect_seen_1st.end(),
                    std::inserter(final_intersection_2, final_intersection_2.begin()));

                three_interconnected_count += final_intersection_2.size();

                final_intersection_2.insert(computer1);
                final_intersection_2.insert(computer2);
                if (final_intersection_2.size() > biggest_interconnected.begin()->size()
                    && is_interconnected(final_intersection_2, neighbours)) {
                    biggest_interconnected.clear();
                    biggest_interconnected.insert(final_intersection_2);
                } else if ((biggest_interconnected.empty()
                    || final_intersection_2.size() == biggest_interconnected.begin()->size())
                    && is_interconnected(final_intersection_2, neighbours)) {
                    biggest_interconnected.insert(final_intersection_2);
                }
            }
        }
    }

    std::cout << "Three-interconnected computers: " << three_interconnected_count << std::endl;
    std::cout << "Biggest interconnected network: ";
    for (const auto& network : biggest_interconnected) {
        for (const auto& computer : network) {
            std::cout << computer << ",";
        }
        std::cout << std::endl;
    }
}