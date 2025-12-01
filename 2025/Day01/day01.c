#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../../2024/DynamicArray.h"

#define BUFFER_SIZE 1024
#define START_POS 50
#define DIAL_CIRCLE_SIZE 100

//Part 2 doesn't work correctly!
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
    DynamicArray *array = createDynamicArray();

    while(fgets(buffer, BUFFER_SIZE, file)) {
        assert(buffer);
        if (buffer[0] == 'R') {
            pushDynamicArray(array, atoi(buffer + 1));
        } else if (buffer[0] == 'L') {
            pushDynamicArray(array, -atoi(buffer + 1));
        } else {
            fprintf(stderr, "Invalid direction in input: %s\n", buffer);
            return 1;
        }
    }

    long pos = START_POS;
    long num_zeros = 0;
    long num_zeros_b = 0;
    for (int i = 0; i < array->size; ++i) {
        long new_pos = pos + array->data[i];

        while (new_pos < 0) {
            new_pos += DIAL_CIRCLE_SIZE;
            num_zeros_b++;
            if (pos == 0) {
                num_zeros_b--;
            }
        }
        while (new_pos > DIAL_CIRCLE_SIZE) {
            new_pos -= DIAL_CIRCLE_SIZE;
            num_zeros_b++;
        }
        if (new_pos == DIAL_CIRCLE_SIZE) {
            new_pos = 0;
        }
        assert(new_pos >= 0 && new_pos < DIAL_CIRCLE_SIZE);
        if (new_pos == 0) {
            num_zeros++;
            num_zeros_b++;
        }
        printf("old: %ld, move: %d, new: %ld, zeros: %ld\n", pos, array->data[i], new_pos, num_zeros_b);
        pos = new_pos;
    }

    fclose(file);
    fprintf(stdout, "Password: %ld\n", num_zeros);
    fprintf(stdout, "Part 2 Password: %ld\n", num_zeros_b);
    fprintf(stdout, "Final Position: %ld\n", pos);

    destroyDynamicArray(array);
    return 0;

}