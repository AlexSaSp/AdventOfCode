#ifndef DYNAMIC_STRING_ARRAY_H
#define DYNAMIC_STRING_ARRAY_H

#include <stdlib.h>

typedef struct _dynamicStringArray {
    char **data;
    int size;
    int capacity;
} DynamicStringArray;

DynamicStringArray *createDynamicArray() {
    DynamicStringArray *array = (DynamicStringArray*)malloc(sizeof(DynamicStringArray));
    if (!array) {
        return NULL;
    }
    array->data = (char**)malloc(sizeof(char*) * 2);
    if (!array->data) {
        free(array);
        return NULL;
    }
    array->size = 0;
    array->capacity = 2;
    return array;
}

void destroyDynamicArray(DynamicStringArray *array) {
    free(array->data);
    free(array);
}

void pushDynamicArray(DynamicStringArray *array, char *value) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = (char**)realloc(array->data, sizeof(char*) * array->capacity);
    }
    array->data[array->size] = value;
    array->size++;
}

void popDynamicArray(DynamicStringArray *array) {
    if (array->size == 0) {
        return;
    }
    array->size--;
}

#endif