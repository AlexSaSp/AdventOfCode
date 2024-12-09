#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../DynamicArray.h"

#define BUFFER_SIZE 1024
#define DELIMITER "   "

void sort(int *array, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (array[i] > array[j]) {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
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
    DynamicArray *array1 = createDynamicArray();
    DynamicArray *array2 = createDynamicArray();
    if (!array1 || !array2) {
        fprintf(stderr, "Unable to allocate memory\n");
        return 1;
    }

    while (fgets(buffer, BUFFER_SIZE, file)) {
        pushDynamicArray(array1, atoi(strtok(buffer, DELIMITER)));
        pushDynamicArray(array2, atoi(strtok(NULL, DELIMITER)));
        assert(strtok(NULL, DELIMITER) == NULL);
    }

    fclose(file);

    assert(array1->size == array2->size);

    // Calculate the total distance
    sort(array1->data, array1->size);
    sort(array2->data, array2->size);
    int totalDistance = 0;
    for (int i = 0; i < array1->size; i++) {
        if (array1->data[i] < array2->data[i]) {
            totalDistance += array2->data[i] - array1->data[i];
        } else {
            totalDistance += array1->data[i] - array2->data[i];
        }
    }
    printf("Total distance: %d\n", totalDistance);

    // Calculate similarity score
    DynamicArray *numbers = createDynamicArray();
    DynamicArray *appearences = createDynamicArray();
    pushDynamicArray(numbers, array1->data[0]);
    pushDynamicArray(appearences, 1);
    for (int i = 0; i < array1->size; i++) {
        if (array1->data[i] != numbers->data[numbers->size - 1]) {
            pushDynamicArray(numbers, array1->data[i]);
            pushDynamicArray(appearences, 1);
        } else {
            appearences->data[appearences->size - 1]++;
        }
    }
    int posInAppearences = 0;
    int similarityScore = 0;
    for (int i = 0; i < array2->size; i++) {
        while (array2->data[i] > numbers->data[posInAppearences]) {
            posInAppearences++;
        }
        if (array2->data[i] == numbers->data[posInAppearences]) {
            similarityScore += (appearences->data[posInAppearences] * array2->data[i]);
        }
    }
    printf("Similarity score: %d\n", similarityScore);
    destroyDynamicArray(array1);
    destroyDynamicArray(array2);
    destroyDynamicArray(numbers);
    destroyDynamicArray(appearences);
    return 0;
}