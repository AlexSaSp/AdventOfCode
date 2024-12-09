#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "../DynamicStringArray.h"

#define BUFFER_SIZE 4096
#define REGEX "(mul\\([0-9][0-9]?[0-9]?,[0-9][0-9]?[0-9]?\\))|do\\(\\)|don't\\(\\)"

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

    regex_t regex;
    if (regcomp(&regex, REGEX, REG_EXTENDED)) {
        fprintf(stderr, "Unable to compile regex\n");
        return 1;
    }

    DynamicStringArray *muls = createDynamicArray();
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file)) {
        const char *line = buffer;
        regmatch_t matches[1];
        while (regexec(&regex, line, 1, matches, 0) == 0) {
            char *mul = (char*)malloc(matches[0].rm_eo - matches[0].rm_so + 1);
            if (!mul) {
                fprintf(stderr, "Unable to allocate memory\n");
                return 1;
            }
            strncpy(mul, line + matches[0].rm_so, matches[0].rm_eo - matches[0].rm_so);
            mul[matches[0].rm_eo - matches[0].rm_so] = '\0';
            pushDynamicArray(muls, mul);
            line += matches[0].rm_eo;
        }
    }
    regfree(&regex);
    fclose(file);
    long total_mul = 0;
    long total_mul_with_do_dont = 0;
    int mul_enabled = 1;
    for (int i = 0; i < muls->size; i++) {
        if (strcmp(muls->data[i], "do()") == 0) {
            mul_enabled = 1;
            continue;
        }
        if (strcmp(muls->data[i], "don't()") == 0) {
            mul_enabled = 0;
            continue;
        }
        long val1 = atol(strtok(muls->data[i] + 4, ",")); 
        long val2 = atol(strtok(strtok(NULL, ","), ")"));
        total_mul += val1 * val2;
        if (mul_enabled) {
            total_mul_with_do_dont += val1 * val2;
        }
    }
    printf("Sum of all multuplication: %ld\n", total_mul);
    printf("Sum of all multuplication with do()/don't(): %ld\n", total_mul_with_do_dont);
    for (int i = 0; i < muls->size; i++) {
        free(muls->data[i]);
    }
    destroyDynamicArray(muls);
    return 0;
}
