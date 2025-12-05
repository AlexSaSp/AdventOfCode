#ifndef DYNAMIC_STRING_ARRAY_H
#define DYNAMIC_STRING_ARRAY_H

#include <stdlib.h>

typedef struct _dynamicStringArray {
    char **data;
    size_t size;
    size_t capacity;
} DynamicStringArray;

DynamicStringArray *createDynamicStringArray() {
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

void destroyDynamicStringArray(DynamicStringArray *array) {
    free(array->data);
    free(array);
}

void pushDynamicStringArray(DynamicStringArray *array, char *value) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = (char**)realloc(array->data, sizeof(char*) * array->capacity);
    }
    array->data[array->size] = value;
    array->size++;
}

void popDynamicStringArray(DynamicStringArray *array) {
    if (array->size == 0) {
        return;
    }
    array->size--;
}

#endif