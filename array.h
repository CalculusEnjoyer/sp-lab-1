#include <stdio.h>


typedef struct {
    const char **array;
    int size;
    int capacity;
} vector;

vector *init(int initial_capacity);

void add(vector *arr, const char *element);

void terminate(vector *arr);

void delete_all(vector *arr);