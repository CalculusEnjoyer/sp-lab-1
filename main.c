#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>
#include "hashmap.h"
#include "array.h"

struct word
{
    int size;
    int count;
    char arr[31];
};

uint64_t word_hash(const void *word, uint64_t seed0, uint64_t seed1) {
    const struct word *current_word = word;
    return hashmap_sip(current_word->arr, strlen(current_word->arr), seed0, seed1);
}

int word_compare(const void *a, const void *b, void *udata) {
    const struct word *ua = a;
    const struct word *ub = b;
    return strcmp(ua->arr, ub->arr) && ua->size == ub->size;
}

void append_letter(struct word* word, char letter)
{
    int index = word->size;

    word->arr[index] = letter;
    word->size = index + 1;
}


struct word* get_next_word(FILE *file_ptr)
{
    struct  word *found = malloc(sizeof(struct word));
    char current_char;

    while (((current_char = fgetc(file_ptr)) != EOF))
    {
        if (!isalpha((unsigned char) current_char)) {
            continue;
        }
        append_letter(found, current_char);
        break;
    }


    while (((current_char = fgetc(file_ptr)) != EOF) && isalpha((unsigned char) current_char))
    {
        append_letter(found, current_char);
    }

    return found;
}

int main(int argc, char *argv[])
{
    int max_size = 0;
    DynamicCStringArray *arr = init_dynamic_array(10);
    struct hashmap *map = hashmap_new(sizeof(struct word), 0, 0, 0, word_hash, word_compare, NULL, NULL);
    const char* source_filename = argv[1];

    FILE *source_ptr;
    source_ptr = fopen(source_filename, "r");

    if (source_ptr == NULL)
    {
        printf("Can not open file!\n");
        return -1;
    }

    while (feof(source_ptr) == 0)
    {
        struct word* current_word = get_next_word(source_ptr);
        struct word* word_with_count = hashmap_get(map, current_word);
        if (word_with_count != NULL) {
            word_with_count->count = word_with_count->count + 1;
            hashmap_set(map, word_with_count);
        } else {
            current_word->count = 1;
            word_with_count = current_word;
            hashmap_set(map, current_word);
        }

        if (word_with_count->count == max_size) {
            append(arr, word_with_count->arr);
        } else if (word_with_count->count > max_size) {
            clear(arr);
            max_size = word_with_count->count;
            append(arr, word_with_count->arr);
        }
    }

    FILE *output_file;

    output_file = fopen("./output.txt", "w");
    if (output_file == NULL) {
        perror("Output file opening failed");
        return -1;
    }

    for (int i = 0; i < arr->size; i++) {
        fputs(arr->array[i], output_file);
        fputs("\n", output_file);
    }

    free_dynamic_array(arr);
    fclose(source_ptr);
    fclose(output_file);

    return 0;
}