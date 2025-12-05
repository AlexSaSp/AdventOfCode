#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <inttypes.h>

#include "../../2024/DynamicStringArray.h"

#define BUFFER_SIZE 1024
#define DELIMITER ","

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
    fgets(buffer, BUFFER_SIZE, file);
    fclose(file);

    DynamicStringArray *intervals = createDynamicStringArray();
    char *interval = strtok(buffer, DELIMITER);
    do {
        pushDynamicStringArray(intervals, interval);
    } while ((interval = strtok(NULL, DELIMITER)) != NULL);
    
    uint64_t count = 0;
    uint64_t count_p2 = 0;
    for (size_t i = 0; i < intervals->size; ++i) {
        uint64_t rangeStart = atoll(strtok(intervals->data[i], "-"));
        uint64_t rangeEnd = atoll(strtok(NULL, "-"));
        
        for (uint64_t j = rangeStart; j <= rangeEnd; ++j) {
            char* numberStr = (char*)malloc((int)((ceil(log10(j))+1)*sizeof(char)));
            sprintf(numberStr, "%" PRIu64, j);
            size_t len = strlen(numberStr);
            //Part 1
            if (len % 2 == 0 
                && strncmp(numberStr, numberStr + len/2, len/2) == 0) {
                count += j;
                //printf("invalid: ");
            }
            // Part 2
            for (size_t k = 1;  k <= len/2; ++k) {
                if (len % k == 0) {
                    char *newStr = (char*)malloc((len + 1) * sizeof(char));
                    newStr[0] = '\0';
                    for (size_t l = 0; l < len / k; ++l) {
                        assert(strlen(newStr) + k <= len);
                        strncat(newStr, numberStr, k);
                        // printf("String: %s\n", newStr);
                    }
                    if (strcmp(newStr, numberStr) == 0) {
                        count_p2 += j;
                        //printf("invalid: ");
                        free(newStr);
                        break;
                    }
                    free(newStr);
                }
            }
            //printf("Checking number: %s\n", numberStr);
            free(numberStr);
        }
    }
    printf("Sum of invalid IDs %" PRIu64 "\n", count);
    printf("Sum of invalid IDs Part 2 %" PRIu64 "\n", count_p2);
    destroyDynamicStringArray(intervals);
    return 0;
}