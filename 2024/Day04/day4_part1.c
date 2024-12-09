#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "../DynamicStringArray.h"

#define BUFFER_SIZE 1024
#define REGEX "XMAS|SAMX"

int main(int argc, char** argv) {
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

    char buffer[BUFFER_SIZE];
    DynamicStringArray *lines = createDynamicArray();
    while (fgets(buffer, BUFFER_SIZE, file)) {
        char *line = (char*)malloc(strlen(buffer) + 1);
        if (!line) {
            fprintf(stderr, "Unable to allocate memory\n");
            return 1;
        }
        strcpy(line, buffer);
        pushDynamicArray(lines, line);
    }
    fclose(file);
    // horizontal appearances
    int appearances = 0;
    for (int i = 0; i < lines->size; i++) {
        regmatch_t matches[1];
        char *line = lines->data[i];
        while (regexec(&regex, line, 1, matches, 0) == 0) {
            appearances++;
            line += matches[0].rm_eo - 1;
        }
    }
    regfree(&regex);
    //printf("horizontal done: %d\n", appearances);
    // vertical appearances
    for (int j = 0; lines->data[1][j] != '\0'; j++) {
        for (int i = 0; i < lines->size - 3; i++) {
            if (lines->data[i][j] == 'X' && lines->data[i + 1][j] == 'M'
                && lines->data[i + 2][j] == 'A' && lines->data[i + 3][j] == 'S') {
                appearances++;
            } else if (lines->data[i][j] == 'S' && lines->data[i + 1][j] == 'A'
                && lines->data[i + 2][j] == 'M' && lines->data[i + 3][j] == 'X') {
                appearances++;
            }
        }
    }
    //printf("vertical done: %d\n", appearances);
    // diagonal appearances upper left to lower right
    for (int i = 0; i < lines->size - 3; i++) {
        for (int j = 0; lines->data[i][j + 3] != '\0'; j++) {
            if (lines->data[i][j] == 'X' && lines->data[i + 1][j + 1] == 'M'
                && lines->data[i + 2][j + 2] == 'A' && lines->data[i + 3][j + 3] == 'S') {
                appearances++;
            } else if (lines->data[i][j] == 'S' && lines->data[i + 1][j + 1] == 'A'
                && lines->data[i + 2][j + 2] == 'M' && lines->data[i + 3][j + 3] == 'X') {
                appearances++;
            }
        }
    }
    //printf("diagonal \\ done: %d\n", appearances);
    // diagonal appearances upper right to lower left
    for (int i = 0; i < lines->size - 3; i++) {
        for (int j = 3; lines->data[i][j] != '\0'; j++) {
            if (lines->data[i][j] == 'X' && lines->data[i + 1][j - 1] == 'M'
                && lines->data[i + 2][j - 2] == 'A' && lines->data[i + 3][j - 3] == 'S') {
                appearances++;
            } else if (lines->data[i][j] == 'S' && lines->data[i + 1][j - 1] == 'A'
                && lines->data[i + 2][j - 2] == 'M' && lines->data[i + 3][j - 3] == 'X') {
                appearances++;
            }
        }
    }
    //printf("diagonal / done\n");
    destroyDynamicArray(lines);
    for (int i = 0; i < lines->size; i++) {
        free(lines->data[i]);
    }
    printf("XMAS appearances: %d\n", appearances);
    return 0;
}