#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include "hashmap.h"
#include "array.h"

struct word
{
    int size;
    int count;
    wchar_t arr[31];
};

uint64_t word_hash(const void *word, uint64_t seed0, uint64_t seed1) {
    const struct word *current_word = word;
    return hashmap_sip(current_word->arr, wcslen(current_word->arr), seed0, seed1);
}

int word_compare(const void *a, const void *b, void *udata) {
    const struct word *ua = a;
    const struct word *ub = b;
    return wcscmp(ua->arr, ub->arr) && ua->size == ub->size;
}

void add_symbol(struct word* word, wchar_t letter)
{
    int index = word->size;

    word->arr[index] = letter;
    word->size = index + 1;
}

int is_character(wchar_t c)
{
    return iswalpha(c);
}


struct word* extract_next_word(FILE *file_ptr)
{
    struct  word *found = malloc(sizeof(struct word));
    wchar_t current_wchar_t;

    while (((current_wchar_t = fgetwc(file_ptr)) != EOF))
    {
        if (!is_character((wchar_t) current_wchar_t)) {
            continue;
        }
        add_symbol(found, current_wchar_t);
        break;
    }


    while (((current_wchar_t = fgetwc(file_ptr)) != EOF) && is_character((wchar_t) current_wchar_t))
    {
        add_symbol(found, current_wchar_t);
    }

    return found;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "uk_UA.UTF-8");
    vector *arr = init(10);
    struct hashmap *map = hashmap_new(sizeof(struct word), 0, 0, 0, word_hash, word_compare, NULL, NULL);
    int max_count = 0;

    const wchar_t* source_filename = argv[1];
    FILE *source_ptr;
    source_ptr = fopen(source_filename, "r");
    if (source_ptr == NULL)
    {
        printf("Can not open file!\n");
        return -1;
    }

    while (feof(source_ptr) == 0)
    {
        struct word* current_word = extract_next_word(source_ptr);
        struct word* word_with_count = hashmap_get(map, current_word);

        if (word_with_count != NULL) {
            current_word->count = word_with_count->count + 1;
            hashmap_set(map, current_word);
        } else {
            current_word->count = 1;
            hashmap_set(map, current_word);
        }

        if (current_word->count == max_count) {
            add(arr, current_word->arr);
        } else if (current_word->count > max_count) {
            delete_all(arr);
            max_count = current_word->count;
            add(arr, current_word->arr);
        }
    }

    FILE *output_file;
    output_file = fopen("./output.txt", "w");
    if (output_file == NULL) {
        perror("Output file opening failed");
        return -1;
    }

    for (int i = 0; i < arr->size; i++) {
        fputws(arr->array[i], output_file);
        fputs("\n", output_file);
    }

    terminate(arr);
    fclose(source_ptr);
    fclose(output_file);

    return 0;
}