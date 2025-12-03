#include "lib/aoc.h"

#include <stdint.h>
#include <stdio.h>
#include <strings.h>

void part1(FILE* const file) {
    int num_hit_0 = 0;
    int dial = 50;

    char direction;
    int count;

    while (fscanf(file, "%c%d\n", &direction, &count) == 2) {
        printf("%d -> %c%d -> ", dial, direction, count);

        if (direction == 'R') {
            dial += count;
        }
        else if (direction == 'L') {
            dial -= count;
        }

        while (dial > 99) {
            dial -= 100;
        }

        while (dial < 0) {
            dial += 100;
        }

        printf("%d\n", dial);

        if (dial == 0) {
            printf("HIT!\n");
            num_hit_0++;
        }
    }

    printf("%d\n", num_hit_0);
}

void part2(FILE* const file) {
    int num_hit_0 = 0;
    int dial = 50;

    char direction;
    int count;

    while (fscanf(file, "%c%d\n", &direction, &count) == 2) {
        if (direction == 'R') {
            for (int i = 0; i < count; i++) {
                dial++;

                if (dial == 100) {
                    dial = 0;
                }

                if (dial == 0) {
                    num_hit_0++;
                }
            }
        }
        else if (direction == 'L') {
            for (int i = 0; i < count; i++) {
                dial--;

                if (dial == -1) {
                    dial = 99;
                }

                if (dial == 0) {
                    num_hit_0++;
                }
            }
        }
    }

    printf("%d\n", num_hit_0);
}

int main(int argc, char const* const* const argv) {
    run(argc, argv, part1, part2);
}
