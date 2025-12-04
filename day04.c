#include "lib/aoc.h"
#include "lib/list.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Storage {
    char** data;
    int32_t row_count;
    int32_t col_count;
} Storage;

Storage get_rows(FILE* const file) {
    char line[256];

    List row_list = new_list();
    size_t line_length = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        size_t length = line_length = strlen(line);
        char* row = malloc(sizeof(char) * length);

        for (size_t i = 0; i < length; i++) {
            if (line[i] == '\n') {
                row[i] = '\0';
                break;
            }
            else if (line[i] == '@') {
                row[i] = 1;
            }
            else {
                row[i] = 0;
            }
        }

        Node* node = new_node();
        node->value = row;
        append(&row_list, node);
    }

    int32_t row_count = (int32_t)row_list.length;
    int32_t col_count = (int32_t)line_length;

    char** rows = malloc((size_t)row_count * sizeof(char*));
    copy_to(&row_list, (void**)rows);
    delete_list(&row_list, 0);

    Storage storage;
    storage.data = rows;
    storage.row_count = row_count;
    storage.col_count = col_count;

    return storage;
}

void free_storage(Storage* storage) {
    for (int32_t i = 0; i < storage->row_count; i++) {
        free(storage->data[i]);
    }

    free(storage->data);
}

size_t get_neighbour_count(Storage const* const storage, int32_t const x, int32_t const y) {
    size_t neighbour_count = 0;

    for (int32_t delta_y = -1; delta_y <= 1; delta_y++) {
        for (int32_t delta_x = -1; delta_x <= 1; delta_x++) {
            if (delta_x == 0 && delta_y == 0) {
                continue;
            }

            int32_t check_x = x + delta_x;
            int32_t check_y = y + delta_y;

            if (check_x < 0 || check_x >= storage->col_count || check_y < 0
                || check_y >= storage->row_count)
            {
                continue;
            }

            if (storage->data[check_y][check_x] == 1) {
                neighbour_count++;
            }
        }
    }

    return neighbour_count;
}

void part1(FILE* const file) {
    Storage storage = get_rows(file);

    size_t accessible_count = 0;

    for (int32_t y = 0; y < storage.row_count; y++) {
        for (int32_t x = 0; x < storage.col_count; x++) {
            if (storage.data[y][x] == 0) {
                continue;
            }

            size_t neighbour_count = get_neighbour_count(&storage, x, y);

            if (neighbour_count < 4) {
                accessible_count++;
            }
        }
    }

    printf("%lu\n", accessible_count);

    free_storage(&storage);
}

void part2(FILE* const) {}

int main(int argc, char const* const* const argv) {
    run(argc, argv, part1, part2);
}
