#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "../../2024/DynamicStringArray.h"
#include "../../2024/DynamicArray.h"

#define BUFFER_SIZE 1024
#define ROLL '@'

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
    
    char buffer[BUFFER_SIZE + 1];

    DynamicStringArray *diagram = createDynamicStringArray();
    pushDynamicStringArray(diagram, NULL);

    while(fgets(buffer + 1, BUFFER_SIZE, file)) {
        buffer[0] = 'i';
        size_t lineSize = 0;
        while (buffer[lineSize] != '\n' && lineSize < BUFFER_SIZE) {
            lineSize++;
        }
        if (!diagram->data[0]) {
            char *lineM1 = (char*)malloc((lineSize + 1) * sizeof(char));
            memset(lineM1, 'i', lineSize);
            lineM1[lineSize] = '\0';
            diagram->data[0] = lineM1;
        }
        buffer[lineSize] = '\0';
        char *line = (char*)malloc((lineSize + 1) * sizeof(char));
        strcpy(line, buffer);
        pushDynamicStringArray(diagram, line);
    }
    fclose(file);
    pushDynamicStringArray(diagram, diagram->data[0]);

    uint64_t sum_accesible_rolls = 0;
    int rolls_accesible = 1;
    // Part 1: Ein Schleifendurchlauf
    do {
        DynamicArray *currentColsToDelete = createDynamicArray();
        DynamicArray *currentLinesToDelete = createDynamicArray();
        for (long line = 1; line < (long)diagram->size - 1; ++line) {
            for (long col = 1; col < (long)strlen(diagram->data[line]); ++col) {
                if (diagram->data[line][col] != ROLL) continue;
                int num_rolls_adjacent = 0;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        if ((i != 0 || j != 0) && diagram->data[line + i][col + j] == ROLL) {
                            // printf("D(%ld, %ld)", line + i, col + j);
                            num_rolls_adjacent++;
                        } else {
                            // printf("(%ld, %ld)", line + i, col + j);
                        }
                    }
                }
                if (num_rolls_adjacent < 4) {
                    ++sum_accesible_rolls;
                    pushDynamicArray(currentLinesToDelete, (int)line);
                    pushDynamicArray(currentColsToDelete, (int)col);
                    // printf("Roll at %ld, %ld accesible with %d adjacent rolls", line, col, num_rolls_adjacent);
                }
                // printf("\n");
            }
        }
        if (currentLinesToDelete->size == 0) rolls_accesible = 0;
        for (size_t i = 0; i < currentLinesToDelete->size; ++i) {
            diagram->data[currentLinesToDelete->data[i]][currentColsToDelete->data[i]] = '.';
        }
        destroyDynamicArray(currentLinesToDelete);
        destroyDynamicArray(currentColsToDelete);
    } while (rolls_accesible);
    printf("Accesible rolls: %" PRId64 "\n", sum_accesible_rolls);
    for (size_t i = 0; i < diagram->size - 1; ++i) {
        free(diagram->data[i]);
    }
    destroyDynamicStringArray(diagram);
    return 0;
}