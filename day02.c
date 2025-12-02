#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1() {
    FILE* f = fopen("day02.txt", "r");

    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
        return;
    }

    unsigned long min = 0;
    unsigned long max = 0;

    unsigned long result = 0;

    while (fscanf(f, "%lu-%lu,", &min, &max) == 2) {
        printf("\nrange: %lu - %lu:\n", min, max);

        for (double i = min; i <= max; i++) {
            double digit_count = ceil(log10(i));

            if ((int)digit_count % 2 == 1) {
                continue;
            }

            double half_count = digit_count / 2;

            int first_half = floor(i / pow(10, half_count));
            int second_half = (unsigned long)i % (unsigned long)pow(10, half_count);

            if (first_half == second_half) {
                printf("%f\n", i);
                result += i;
            }
        }
    }

    printf("\n%lu\n", result);
}

void part2() {
    FILE* f = fopen("day02.txt", "r");

    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
        return;
    }

    unsigned long min = 0;
    unsigned long max = 0;

    unsigned long result = 0;

    while (fscanf(f, "%lu-%lu,", &min, &max) == 2) {
        printf("\nrange: %lu - %lu:\n", min, max);

        for (unsigned long num = min; num <= max; num++) {
            unsigned long digit_count = ceil(log10(num));

            if (digit_count == log10(num)) {
                continue;
            }

            // printf("num: %lu\n", num);

            for (int parts_count = 2; parts_count <= digit_count; parts_count++) {
                if (digit_count % parts_count != 0) {
                    continue;
                }

                int repeat_count = digit_count / parts_count;

                // printf(" c: %d digits: %d\n", parts_count, repeat_count);

                unsigned long parts[parts_count];

                for (int p = 1; p <= parts_count; p++) {

                    parts[p - 1] = floor(
                        num % (unsigned long)pow(10, p * repeat_count)
                        / pow(10, (p - 1) * repeat_count)
                    );

                    // printf("parts[%d] = %lu\n", p - 1, parts[p - 1]);
                }

                unsigned long last_part = parts[0];
                char all_equal = 1;

                for (int p = 1; p < parts_count; p++) {
                    if (parts[p] != last_part) {
                        all_equal = 0;
                        break;
                    }
                }

                if (all_equal) {
                    printf("%lu\n", num);
                    result += num;
                    break;
                }
            }
        }
    }

    printf("\n%lu\n", result);
}

int main() {
    part2();
    return 0;
}
