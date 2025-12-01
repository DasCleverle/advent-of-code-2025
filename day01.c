#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void get_password() {
    FILE* f = fopen("day01.txt", "r");

    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
        return;
    }

    int num_hit_0 = 0;
    int dial = 50;

    char direction;
    int count;

    while (fscanf(f, "%c%d\n", &direction, &count) == 2) {
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

    fclose(f);
}

void get_password_2() {
    FILE* f = fopen("day01.txt", "r");

    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
        return;
    }

    int num_hit_0 = 0;
    int dial = 50;

    char direction;
    int count;

    while (fscanf(f, "%c%d\n", &direction, &count) == 2) {
        int dial_prev = dial;

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

    fclose(f);
}

int main() {
    get_password_2();
}
