#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1(char* filename) {
    FILE* f = fopen(filename, "r");

    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
        return;
    }

    char line[256];
    int result = 0;

    while (fgets(line, 256, f) != NULL) {
        int length = strnlen(line, 256);

        int first = 0;
        int second = 0;

        int largest_joltage = 0;

        for (int i = 0; i < length; i++) {
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

            for (int j = i + 1; j < length; j++) {
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

    fclose(f);
}

#define DIGIT_COUNT 12

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

    int largest_digit = 0;
    size_t result = 0;

    for (size_t i = start_index; i < length - digit_index + 1; i++) {
        int digit = line[i] - '0';

        if (digit <= largest_digit) {
            continue;
        }

        largest_digit = digit;

        // printf("%slargest: %d\n", spaces, largest_digit);

        if (i == length - 1) {
            result = largest_digit * pow(10, digit_index - 1);
            break;
        }

        // printf("\n");

        size_t next = get_number(line, i + 1, length, digit_index - 1);
        size_t new_result = largest_digit * pow(10, digit_index - 1) + next;

        if (new_result > result) {
            // printf("%snew_result: %lu\n", spaces, new_result);
            result = new_result;
        }
    }

    // printf("\n");
    return result;
}

void part2(char* filename) {
    FILE* f = fopen(filename, "r");

    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
        return;
    }

    char line[256];
    size_t result = 0;

    while (fgets(line, 256, f) != NULL) {
        size_t length = strnlen(line, 256) - 1;

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

    fclose(f);
}

int main(int argc, char** argv) {
    switch (argv[1][0]) {
        case '1':
            part1(argv[2]);
            break;

        case '2':
            part2(argv[2]);
            break;
    }

    return 0;
}
