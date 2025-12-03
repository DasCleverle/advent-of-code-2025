#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/aoc.h"

#define DIGIT_COUNT 12

void part1(FILE* const file) {
    char line[256];
    int result = 0;

    while (fgets(line, 256, file) != NULL) {
        size_t length = strlen(line);

        int first = 0;
        int second = 0;

        int largest_joltage = 0;

        for (size_t i = 0; i < length; i++) {
            if (line[i] == '\n') {
                break;
            }

            int first_digit = line[i] - '0';

            if (first_digit <= first) {
                continue;
            }

            first = first_digit;

            if (i == length) {
                // printf("No second digit\n");
                break;
            }

            // printf("first: %d\n", first);

            for (size_t j = i + 1; j < length; j++) {
                if (line[j] == '\n') {
                    break;
                }

                int second_digit = line[j] - '0';

                if (second_digit <= second) {
                    continue;
                }

                second = second_digit;

                int joltage = first * 10 + second;

                // printf("   second:  %d\n", second);
                // printf("   joltage: %d\n", joltage);

                if (joltage > largest_joltage) {
                    largest_joltage = joltage;
                }
            }

            first = 0;
            second = 0;
        }

        result += largest_joltage;

        printf("%s %d\n", line, largest_joltage);
    }

    printf("Result: %d\n", result);
}

size_t get_number(char* line, size_t start_index, size_t length, int digit_index) {
    if (digit_index < 0) {
        return 0;
    }

    // char spaces[DIGIT_COUNT * 4 + 1];
    // int space_count = (DIGIT_COUNT - digit_index) * 4;
    //
    // memset(spaces, ' ', space_count);
    // spaces[space_count] = '\0';
    //
    // char search_space[length];
    // memcpy(search_space, line + start_index, length - digit_index + 1);
    // search_space[length - start_index] = '\0';

    // printf("%sdigit_index: %d\n", spaces, digit_index);
    // printf("%ssearch_space: %s\n", spaces, search_space);

    size_t largest_digit = 0;
    size_t result = 0;

    for (size_t i = start_index; i < length - (size_t)digit_index + 1; i++) {
        size_t digit = (size_t)(line[i] - '0');

        if (digit <= largest_digit) {
            continue;
        }

        largest_digit = digit;

        // printf("%slargest: %d\n", spaces, largest_digit);

        if (i == length - 1) {
            result = largest_digit * (size_t)pow(10, digit_index - 1);
            break;
        }

        // printf("\n");

        size_t next = get_number(line, i + 1, length, digit_index - 1);
        size_t new_result = largest_digit * (size_t)pow(10, digit_index - 1) + next;

        if (new_result > result) {
            // printf("%snew_result: %lu\n", spaces, new_result);
            result = new_result;
        }
    }

    // printf("\n");
    return result;
}

void part2(FILE* const file) {
    char line[256];
    size_t result = 0;

    while (fgets(line, 256, file) != NULL) {
        size_t length = strlen(line) - 1;

        if (length == 0) {
            continue;
        }

        size_t number = get_number(line, 0, length, DIGIT_COUNT);

        printf("line:   %s", line);
        printf("result: %lu\n\n", number);

        result += number;

        // for (int d = 0; d < DIGIT_COUNT; d++) {
        //     int largest = 0;
        //     int last_index = 0;
        //
        //     for (int i = 0; i < length - DIGIT_COUNT; i++) {
        //         int num = line[i] - '0';
        //
        //         if (num <= largest) {
        //             continue;
        //         }
        //
        //         largest = num;
        //         last_index = i;
        //     }
        // }
    }

    printf("total: %lu\n", result);
}

int main(int argc, char const* const* const argv) {
    run(argc, argv, part1, part2);
}
