#include "../DynamicArray.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 1024
#define DELIMITER " "

int testSafe(DynamicArray *report) {
    for (size_t i = 1; i < report->size; i++) {
        if (report->data[0] < report->data[1] && report->data[i] < report->data[i - 1]) {
            return 0;
        } else if (report->data[0] > report->data[1] && report->data[i] > report->data[i - 1]) {
            return 0;
        } else if (report->data[i] - report->data[i - 1] > 3
            || report->data[i] - report->data[i - 1] < -3) {
            return 0;
        } else if (report->data[i] == report->data[i - 1]) {
            return 0;
        } 
    }
    return 1;
}

DynamicArray *removeFromDynArr(DynamicArray *array, size_t index) {
    assert(index < array->size);
    DynamicArray *newArray = createDynamicArray();
    if(!newArray) {
        return NULL;
    }
    for(size_t i = 0; i < array->size; i++) {
        if (i != index) {
            pushDynamicArray(newArray, array->data[i]);
        }
    }
    return newArray;
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
    int safeReports = 0;
    int safeReportsPart2 = 0;
    while (fgets(buffer, BUFFER_SIZE, file)) {
        DynamicArray *report = createDynamicArray();
        if (!report) {
            fprintf(stderr, "Unable to allocate memory\n");
            return 1;
        }
        char *level = strtok(buffer, DELIMITER);
        while (level) {
            pushDynamicArray(report, atoi(level));
            level = strtok(NULL, DELIMITER);
        }

        if (testSafe(report)) {
            safeReports++;
            safeReportsPart2++;
        } else {
            for (size_t i = 0; i < report->size; i++) {
                DynamicArray *newReport = removeFromDynArr(report, i);
                if (!newReport) {
                    fprintf(stderr, "Unable to allocate memory\n");
                    return 1;
                }
                if (testSafe(newReport)) {
                    safeReportsPart2++;
                    destroyDynamicArray(newReport);
                    break;
                }
                destroyDynamicArray(newReport);
            }
        }
        destroyDynamicArray(report);
    }
    fclose(file);
    printf("Safe reports part 1: %d\n", safeReports);
    printf("Safe reports part 2: %d\n", safeReportsPart2);
    return 0;
} 