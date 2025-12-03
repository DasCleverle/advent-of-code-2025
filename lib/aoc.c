#include "aoc.h"

#include <stdio.h>
#include <stdlib.h>

void run(
    int const argc,
    char const* const* const argv,
    void (*part1)(FILE* const),
    void (*part2)(FILE* const)
) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <part> <filename>\n", argv[0]);
        exit(1);
        return;
    }

    char const* const part = argv[1];
    char const* const filename = argv[2];

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(1);
        return;
    }

    if (part[0] == '1') {
        part1(file);
    }
    else if (part[0] == '2') {
        part2(file);
    }
    else {
        fprintf(stderr, "Unknown part: %s\n", part);
        exit(1);
        return;
    }

    if (file != NULL) {
        fclose(file);
    }
}
