#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#define REG_START 12 //std::string("Register A: ").length()
#define PROG_START 9 //std::string("Program: ").length()

u_int64_t reg_A;
u_int64_t reg_B;
u_int64_t reg_C;
size_t ip = 0;

u_int64_t get_combo(u_int8_t operand) {
    switch (operand) {
        case 0:
        case 1:
        case 2:
        case 3:
            return operand;
        case 4:
            return reg_A;
        case 5:
            return reg_B;
        case 6:
            return reg_C;
        default:
            throw std::runtime_error("Invalid operand");
    }
}

void adv(u_int8_t operand) {
    reg_A = reg_A / std::pow(2, get_combo(operand));
    ip += 2;
}

void bxl(u_int8_t operand) {
    reg_B = reg_B ^ operand;
    ip += 2;
}

void bst(u_int8_t operand) {
    reg_B = (get_combo(operand) % 8) & 7;
    ip += 2;
}

void jnz(u_int8_t operand) {
    if (reg_A == 0) {
        ip += 2;
    } else {
        ip = operand;
    }
}

void bxc() {
    reg_B = reg_B ^ reg_C;
    ip += 2;
}

u_int8_t out(u_int8_t operand) {
    ip += 2;
    return get_combo(operand) % 8;
}

void bdv(u_int8_t operand) {
    reg_B = reg_A / std::pow(2, get_combo(operand));
    ip += 2;
}

void cdv(u_int8_t operand) {
    reg_C = reg_A / std::pow(2, get_combo(operand));
    ip += 2;
}

std::vector<u_int8_t> run_program(std::vector<u_int8_t>& program) {
    std::vector<u_int8_t> output;
    while (ip < program.size() - 1) {
        u_int8_t opcode = program[ip];
        u_int8_t operand = program[ip + 1];
        switch (opcode) {
            case 0:
                adv(operand);
                break;
            case 1:
                bxl(operand);
                break;
            case 2:
                bst(operand);
                break;
            case 3:
                jnz(operand);
                break;
            case 4:
                bxc();
                break;
            case 5:
                output.push_back(out(operand));
                break;
            case 6:
                bdv(operand);
                break;
            case 7:
                cdv(operand);
                break;
        }
    }
    return output;
}

int main (int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    std::ifstream file(argv[1], std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    std::vector<u_int8_t> program;
    std::string line;
    u_int64_t adv_operands = 0;
    int line_number = 0;
    while (std::getline(file, line)) {
        if (line_number == 0) {
            reg_A = std::stoi(line.substr(REG_START, line.length() - REG_START));
        } else if (line_number == 1) {
            reg_B = std::stoi(line.substr(REG_START, line.length() - REG_START));
        } else if (line_number == 2) {
            reg_C = std::stoi(line.substr(REG_START, line.length() - REG_START));
        } else if (line_number == 4) {
            for (char c : line.substr(PROG_START, line.length() - PROG_START)) {
                if (c != ',') {
                    program.push_back(c - '0');
                }
            }
        }
        line_number++;
    }
    file.close();
    
    for (size_t i = 0; i < program.size() - 1; i += 2) {
        if (program[i] == 0) {
            adv_operands += program[i + 1];
        }
    }
    std::vector<u_int8_t> output = run_program(program);
    
    std::string output_string;
    for (u_int8_t i : output) {
        output_string += std::to_string(i) + ",";
    }
    std::cout << "Program output: " << output_string.substr(0, output_string.length() - 1) << std::endl;


    u_int64_t reg_A_init = 0;
    for (size_t i = 0; i < program.size(); i++) {
        reg_A_init = reg_A_init << adv_operands;
        std::vector<u_int8_t> output_ref;
        for (size_t j = program.size() - 1 - i; j < program.size(); j++) {
            output_ref.push_back(program[j]);
        }
        do {
            //std::cout << "Trying: " << i << ": " << reg_A_init << std::endl;
            reg_A = reg_A_init;
            ip = 0;
            output = run_program(program);
            // std::string output_string;
            // for (u_int8_t i : output) {
            //     output_string += std::to_string(i) + ",";
            // }
            // std::cout << output_string.substr(0, output_string.length() - 1) << std::endl;
            reg_A_init++;
        } while (output.size() != i + 1 || output != output_ref);
        reg_A_init--;
    }

    std::cout << "Register A: " << reg_A_init << std::endl;
}
