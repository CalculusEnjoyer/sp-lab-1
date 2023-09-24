#include <stdio.h>


typedef struct {
    const char **array;
    int size;
    int capacity;
} DynamicCStringArray;

DynamicCStringArray *init_dynamic_array(int initial_capacity);

void append(DynamicCStringArray *arr, const char *element);

void free_dynamic_array(DynamicCStringArray *arr);

void clear(DynamicCStringArray *arr);