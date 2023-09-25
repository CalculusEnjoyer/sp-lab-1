#include <stdio.h>


typedef struct {
    const wchar_t **array;
    int size;
    int capacity;
} vector;

vector *init(int initial_capacity);

void add(vector *arr, const wchar_t *element);

void terminate(vector *arr);

void delete_all(vector *arr);