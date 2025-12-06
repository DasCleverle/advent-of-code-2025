#include "lib/aoc.h"
#include "lib/list.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t min;
    size_t max;
} Range;

void part1(FILE* const file) {
    List ranges = new_list();
    List ids = new_list();

    Range* range = malloc(sizeof(Range));

    while (fscanf(file, "%lu-%lu", &range->min, &range->max) == 2) {
        Node* node = new_node();
        node->value = range;

        append(&ranges, node);

        int next_char = fgetc(file);

        if (next_char == '\n') {
            next_char = fgetc(file);

            if (next_char == '\n') {
                break;
            }

            ungetc(next_char, file);
        }

        range = malloc(sizeof(Range));
    }

    size_t id;
    while (fscanf(file, "%lu\n", &id) == 1) {
        Node* node = new_node();
        node->value = (void*)id;

        append(&ids, node);
    }

    size_t count = 0;
    Node* id_node = ids.head;

    while (id_node != NULL) {
        Node* range_node = ranges.head;
        id = (size_t)id_node->value;

        while (range_node != NULL) {
            Range* range = range_node->value;

            if (id >= range->min && id <= range->max) {
                printf("%lu\n", id);
                count++;
                break;
            }

            range_node = range_node->next;
        }

        id_node = id_node->next;
    }

    printf("count: %lu\n", count);
}

int compare_ranges(void const* a, void const* b) {
    Range const* range_a = *(Range**)a;
    Range const* range_b = *(Range**)b;

    long long result = (long long)(range_a->min - range_b->min);

    if (result == 0) {
        return 0;
    }

    return result > 0 ? 1 : -1;
}

void part2(FILE* const file) {
    List ranges_list = new_list();
    List unique_ranges = new_list();

    Range* range = malloc(sizeof(Range));

    while (fscanf(file, "%lu-%lu", &range->min, &range->max) == 2) {
        Node* node = new_node();
        node->value = range;

        append(&ranges_list, node);

        range = malloc(sizeof(Range));
    }

    free(range);

    Range* ranges[ranges_list.length];
    copy_to(&ranges_list, (void**)ranges);
    qsort(ranges, ranges_list.length, sizeof(Range*), compare_ranges);

    for (size_t i = 0; i < ranges_list.length; i++) {
        Range* range = ranges[i];

        printf("checking: %lu-%lu\n", range->min, range->max);

        Node* unique_node = unique_ranges.head;
        bool found = false;

        while (unique_node != NULL) {
            Range* unique_range = unique_node->value;

            bool is_fully_contained = range->min >= unique_range->min
                                      && range->max <= unique_range->max;
            bool is_min_contained = range->min >= unique_range->min
                                    && range->min <= unique_range->max
                                    && range->max >= unique_range->max;
            bool is_max_contained = range->max >= unique_range->min
                                    && range->max <= unique_range->max
                                    && range->min <= unique_range->min;

            if (is_fully_contained) {
                printf("\tfully contained: %lu-%lu\n", unique_range->min, unique_range->max);

                found = true;
                break;
            }

            if (is_min_contained) {
                printf("\tmin contained: %lu-%lu\n", unique_range->min, unique_range->max);

                found = true;
                unique_range->max = range->max;
                break;
            }

            if (is_max_contained) {
                printf("\tmax contained: %lu-%lu\n", unique_range->min, unique_range->max);

                found = true;
                unique_range->min = range->min;
                break;
            }

            unique_node = unique_node->next;
        }

        if (!found) {
            Node* n = new_node();
            Range* r = malloc(sizeof(Range));

            r->min = range->min;
            r->max = range->max;
            n->value = r;

            append(&unique_ranges, n);

            printf("\tadded: %lu-%lu\n", range->min, range->max);
        }
    }

    size_t total = 0;
    Node* node = unique_ranges.head;

    while (node != NULL) {
        Range* range = node->value;
        printf("%lu-%lu\n", range->min, range->max);

        total += range->max - range->min + 1;
        node = node->next;
    }

    printf("total: %lu\n", total);

    delete_list(&unique_ranges, true);
    delete_list(&ranges_list, true);
}

int main(int argc, char const* const* const argv) {
    run(argc, argv, part1, part2);
}
