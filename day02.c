#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "lib/aoc.h"

void part1(FILE* const file) {
    size_t min = 0;
    size_t max = 0;

    size_t result = 0;

    while (fscanf(file, "%lu-%lu,", &min, &max) == 2) {
        printf("\nrange: %lu - %lu:\n", min, max);

        for (double i = (double)min; i <= max; i++) {
            double digit_count = ceil(log10(i));

            if ((int)digit_count % 2 == 1) {
                continue;
            }

            double half_count = digit_count / 2;

            size_t first_half = (size_t)floor(i / pow(10, half_count));
            size_t second_half = (size_t)i % (size_t)pow(10, half_count);

            if (first_half == second_half) {
                printf("%f\n", i);
                result += (size_t)i;
            }
        }
    }

    printf("\n%lu\n", result);
}

void part2(FILE* const file) {
    size_t min = 0;
    size_t max = 0;

    size_t result = 0;

    while (fscanf(file, "%lu-%lu,", &min, &max) == 2) {
        printf("\nrange: %lu - %lu:\n", min, max);

        for (size_t num = min; num <= max; num++) {
            size_t digit_count = (size_t)ceil(log10((double)num));

            if (digit_count == log10((double)num)) {
                continue;
            }

            // printf("num: %lu\n", num);

            for (size_t parts_count = 2; parts_count <= digit_count; parts_count++) {
                if ((size_t)digit_count % parts_count != 0) {
                    continue;
                }

                size_t repeat_count = digit_count / parts_count;

                // printf(" c: %d digits: %d\n", parts_count, repeat_count);

                size_t parts[parts_count];

                for (size_t p = 1; p <= parts_count; p++) {
                    double modulo_exponent = (double)(p * repeat_count);
                    double divide_exponent = (double)((p - 1) * repeat_count);

                    double modulo = (double)(num % (size_t)pow(10, modulo_exponent));
                    double division = modulo / (size_t)pow(10, divide_exponent);

                    parts[p - 1] = (size_t)floor(division);

                    // printf("parts[%d] = %lu\n", p - 1, parts[p - 1]);
                }

                size_t last_part = parts[0];
                char all_equal = 1;

                for (size_t p = 1; p < parts_count; p++) {
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

int main(int const argc, char const* const* const argv) {
    run(argc, argv, part1, part2);
}
