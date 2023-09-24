#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "array.h"


vector *init(int initial_capacity) {
    vector *arr = malloc(sizeof(vector));
    if (arr == NULL) {
        exit(EXIT_FAILURE);
    }
    arr->array = malloc(initial_capacity * sizeof(const char *));
    if (arr->array == NULL) {
        free(arr);
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

void add(vector *arr, const char *element) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->array = realloc(arr->array, arr->capacity * sizeof(const char *));
        if (arr->array == NULL) {
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < arr->size; i++) {
        if (strcmp(element, arr->array[i]) == 0) {
            return;
        }
    }
    arr->array[arr->size] = strdup(element);
    arr->size++;
}

void delete_all(vector *arr) {
    for (int i = 0; i < arr->size; i++) {
        free((void*)arr->array[i]);
    }
    arr->size = 0;
}

void terminate(vector *arr) {
    delete_all(arr);
    free(arr->array);
    free(arr);
}