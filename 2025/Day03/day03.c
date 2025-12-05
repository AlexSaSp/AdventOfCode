#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "../../2024/DynamicStringArray.h"

#define BUFFER_SIZE 1024
#define NUM_DIGITS_P2 12

int findMax(char* bank, int* index, size_t start, size_t end) {
    // printf("String: %s, end: %zu, Strlen: %zu\n", bank, end, strlen(bank));
    assert(start >= 0 && end <= strlen(bank));
    char max = '0';
    for (size_t i = start; i < end; ++i) {
        if (bank[i] > max) {
            max = bank[i];
            if (index) *index = i;
        }
    }
    return max - '0';
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Unable to open file %s\n", argv[1]);
        return 1;
    }
    
    char buffer[BUFFER_SIZE];

    DynamicStringArray *banks = createDynamicStringArray();

    while(fgets(buffer, BUFFER_SIZE, file)) {
        size_t bankSize = 0;
        while (buffer[bankSize] != '\n' && bankSize < BUFFER_SIZE) {
            bankSize++;
        }
        buffer[bankSize] = '\0';
        char *bank = (char*)malloc((bankSize + 1) * sizeof(char));
        strcpy(bank, buffer);
        pushDynamicStringArray(banks, bank);
    }
    fclose(file);

    uint64_t sum_joltage_P1 = 0;
    uint64_t sum_joltage_P2 = 0;
    for (size_t i = 0; i < banks->size; ++i) {
        // Part 1
        int index_firstDigit;
        int firstDigit = findMax(banks->data[i], &index_firstDigit, 0, strlen(banks->data[i]) - 1);
        int secondDigit = findMax(banks->data[i], NULL, index_firstDigit + 1, strlen(banks->data[i]));
        sum_joltage_P1 += firstDigit * 10 + secondDigit;
        // Part 2
        int last_index = -1;
        uint64_t current_joltage = 0;
        for (int j = NUM_DIGITS_P2 - 1; j >= 0; --j) {
            int current_start = last_index + 1;
            current_joltage = current_joltage * 10 + findMax(banks->data[i], &last_index, current_start, strlen(banks->data[i]) - j);
            // printf("Current P2 sum: %" PRIu64 "\n", current_joltage);
        }
        sum_joltage_P2 += current_joltage;
    }
    printf("Sum of joltage ratings P1: %" PRIu64 "\n", sum_joltage_P1);
    printf("Sum of joltage ratings P2: %" PRIu64 "\n", sum_joltage_P2);
    for (size_t i = 0; i < banks->size; ++i) {
        free(banks->data[i]);
    }
    destroyDynamicStringArray(banks);
    return 0;
}