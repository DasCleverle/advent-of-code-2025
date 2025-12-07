#include "lib/aoc.h"
#include "lib/list.h"

#include <stdio.h>
#include <string.h>

List get_lines(FILE* const file) {
    List lines = new_list();

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t length = strlen(buffer);
        char* line = malloc(sizeof(char) * length + 1);

        strcpy(line, buffer);

        append_value(&lines, line);
    }

    return lines;
}

void print_lines(List* lines) {
    Node* curr = lines->head;
    while (curr != NULL) {
        printf("%s", (char*)curr->value);
        curr = curr->next;
    }

    printf("\n");
}

void part1(FILE* const file) {
    List lines = get_lines(file);
    size_t line_length = strlen((char*)lines.head->value);

    Node* node = lines.head->next;
    size_t split_count = 0;

    while (node != NULL) {
        char* line = node->value;
        char* prev_line = node->prev ? node->prev->value : NULL;

        if (prev_line == NULL) {
            break;
        }

        for (size_t i = 0; i < line_length; i++) {
            if (line[i] == '^' && (prev_line[i] == '|' || prev_line[i] == 'S')) {
                split_count++;

                if (i > 0) {
                    line[i - 1] = '|';
                }

                if (i < line_length - 1) {
                    line[i + 1] = '|';
                }
            }
            else if (prev_line[i] == 'S' || prev_line[i] == '|') {
                line[i] = '|';
            }
        }

        print_lines(&lines);

        node = node->next;
    }

    printf("split_count: %lu\n", split_count);

    delete_list(&lines, true);
}

size_t explore_timeline(size_t* cache[], List* lines, Node* node, size_t line_length, size_t depth) {
    if (node == NULL) {
        // print_lines(lines);
        // printf("end %lu\n\n", depth);

        return 1;
    }

    char* line = node->value;
    char* prev_line = node->prev ? node->prev->value : NULL;

    if (prev_line == NULL) {
        return 1;
    }

    size_t timeline_count = 0;

    for (size_t i = 0; i < line_length; i++) {
        if (line[i] == '^' && (prev_line[i] == '|' || prev_line[i] == 'S')) {
            if (i > 0) {
                if (cache[depth][i - 1] != 0) {
                    timeline_count += cache[depth][i - 1];
                }
                else {
                    line[i - 1] = '|';
                    size_t count = explore_timeline(cache, lines, node->next, line_length, depth + 1);
                    line[i - 1] = '.';

                    cache[depth][i - 1] = count;
                    timeline_count += count;
                }
            }

            if (i < line_length - 1) {
                if (cache[depth][i + 1] != 0) {
                    printf("cached! %lu %lu\n", depth, i);
                    timeline_count += cache[depth][i + 1];
                }
                else {
                    line[i + 1] = '|';
                    size_t count = explore_timeline(cache, lines, node->next, line_length, depth + 1);
                    line[i + 1] = '.';

                    cache[depth][i + 1] = count;
                    timeline_count += count;
                }
            }
        }
        else if (prev_line[i] == 'S' || prev_line[i] == '|') {
            if (cache[depth][i] != 0) {
                timeline_count += cache[depth][i];
            }
            else {
                line[i] = '|';
                size_t count = explore_timeline(cache, lines, node->next, line_length, depth + 1);
                line[i] = '.';

                cache[depth][i] = count;
                timeline_count += count;
            }
        }
    }

    return timeline_count;
}

void part2(FILE* const file) {
    List lines = get_lines(file);
    size_t line_length = strlen((char*)lines.head->value);

    size_t* cache[lines.length];
    for (size_t i = 0; i < lines.length; i++) {
        cache[i] = calloc(line_length, sizeof(size_t));
    }

    size_t timeline_count = explore_timeline(cache, &lines, lines.head->next, line_length, 0);

    printf("timeline_count: %lu\n", timeline_count);

    delete_list(&lines, true);

    for (size_t i = 0; i < lines.length; i++) {
        free(cache[i]);
    }
}

int main(int argc, char const* const* const argv) {
    run(argc, argv, part1, part2);
}
