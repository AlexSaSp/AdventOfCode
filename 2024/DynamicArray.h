#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <stdlib.h>

typedef struct _dynamicArray {
    int *data;
    int size;
    int capacity;
} DynamicArray;

DynamicArray *createDynamicArray() {
    DynamicArray *array = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!array) {
        return NULL;
    }
    array->data = (int*)malloc(sizeof(int) * 2);
    if (!array->data) {
        free(array);
        return NULL;
    }
    array->size = 0;
    array->capacity = 2;
    return array;
}

void destroyDynamicArray(DynamicArray *array) {
    free(array->data);
    free(array);
}

void pushDynamicArray(DynamicArray *array, int value) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = (int*)realloc(array->data, sizeof(int) * array->capacity);
    }
    array->data[array->size] = value;
    array->size++;
}

void popDynamicArray(DynamicArray *array) {
    if (array->size == 0) {
        return;
    }
    array->size--;
}

#endif