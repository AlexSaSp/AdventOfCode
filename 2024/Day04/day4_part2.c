#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../DynamicStringArray.h"

#define BUFFER_SIZE 1024

int match_diag_ul_lr(char **data, int i, int j) {
    if (data[i][j] == 'M' && data[i + 1][j + 1] == 'A'
        && data[i + 2][j + 2] == 'S') {
        return 1;
    } else if (data[i][j] == 'S' && data[i + 1][j + 1] == 'A'
        && data[i + 2][j + 2] == 'M') {
        return 1;
    } else {
        return 0;
    }
}

int match_diag_ur_ll(char **data, int i, int j) {
    if (data[i][j] == 'M' && data[i + 1][j - 1] == 'A'
        && data[i + 2][j - 2] == 'S') {
        return 1;
    } else if (data[i][j] == 'S' && data[i + 1][j - 1] == 'A'
        && data[i + 2][j - 2] == 'M') {
        return 1;
    } else {
        return 0;
    }
}

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

    char buffer[BUFFER_SIZE];
    DynamicStringArray *lines = createDynamicStringArray();
    while (fgets(buffer, BUFFER_SIZE, file)) {
        char *line = (char*)malloc(strlen(buffer) + 1);
        if (!line) {
            fprintf(stderr, "Unable to allocate memory\n");
            return 1;
        }
        strcpy(line, buffer);
        pushDynamicStringArray(lines, line);
    }
    fclose(file);
    
    int appearances = 0;
    for (size_t i = 0; i < lines->size - 2; i++) {
        for (size_t j = 0; lines->data[i][j + 2] != '\0'; j++) {
            if (match_diag_ul_lr(lines->data, i, j) && match_diag_ur_ll(lines->data, i, j + 2)) {
                appearances++;
            }
        }
    }

    destroyDynamicStringArray(lines);
    for (size_t i = 0; i < lines->size; i++) {
        free(lines->data[i]);
    }
    printf("X-MAS appearances: %d\n", appearances);
    return 0;
}