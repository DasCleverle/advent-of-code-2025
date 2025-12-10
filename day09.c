#include "lib/aoc.h"
#include "lib/list.h"

#include <stdlib.h>

typedef struct Point {
    size_t x, y;
} Point;

typedef struct Pair {
    Point* from;
    Point* to;
    size_t area;
} Pair;

typedef struct Room {
    char* data;
    size_t width;
    size_t height;
} Room;

List get_points(FILE* const file) {
    List points = new_list();
    Point* point = calloc(1, sizeof(Point));

    while (fscanf(file, "%lu,%lu\n", &point->x, &point->y) == 2) {
        append_value(&points, point);
        point = calloc(1, sizeof(Point));
    }

    free(point);

    return points;
}

List get_pairs(List* const points) {
    List pairs = new_list();

    size_t i = 0;
    for (Node* node = points->head; node != NULL; node = node->next, i++) {
        Point* point = node->value;

        size_t j = 0;
        for (Node* other = points->head; other != NULL; other = other->next, j++) {
            if (i >= j) {
                continue;
            }

            Point* other_point = other->value;

            long long width = llabs((long long)point->x - (long long)other_point->x) + 1;
            long long height = llabs((long long)point->y - (long long)other_point->y) + 1;

            Pair* pair = malloc(sizeof(Pair));
            pair->from = point;
            pair->to = other_point;
            pair->area = (size_t)width * (size_t)height;

            append_value(&pairs, pair);
        }
    }

    return pairs;
}

void part1(FILE* const file) {
    List points = get_points(file);
    List pairs = get_pairs(&points);

    size_t largest_area = 0;

    for (Node* node = pairs.head; node != NULL; node = node->next) {
        Pair* pair = node->value;

        if (pair->area > largest_area) {
            largest_area = pair->area;
        }
    }

    printf("%zu\n", largest_area);
}

void part2(FILE* const) {}

int main(int argc, char const* const* const argv) {
    run(argc, argv, part1, part2);
}
