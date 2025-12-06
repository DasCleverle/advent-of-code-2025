#include "lib/aoc.h"
#include "lib/list.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List get_lines(FILE* const file) {
    List lines = new_list();

    char line[4096];
    while (fgets(line, sizeof(line), file) != NULL) {
        size_t length = strlen(line);

        if (length == 0 || strcmp(line, "\n") == 0) {
            continue;
        }

        Node* node = new_node();

        char* value = malloc(sizeof(char) * length + 1);
        strcpy(value, line);

        node->value = value;
        append(&lines, node);
    }

    return lines;
}

void part1(FILE* const file) {
    List lines = get_lines(file);
    List problems = new_list();

    Node* curr = lines.head;
    while (curr != lines.tail) {
        char* start = curr->value;
        char* end = NULL;
        size_t index = 0;

        for (size_t number = strtoul(start, &end, 10); start != end;
             number = strtoul(start, &end, 10))
        {
            List* list = list_get(&problems, index);

            if (list == NULL) {
                list = malloc(sizeof(List));
                list->length = 0;
                list->head = NULL;
                list->tail = NULL;

                append_value(&problems, list);
            }

            append_value(list, (void*)number);

            start = end;
            index++;
        }

        curr = curr->next;
    }

    char* operations = (char*)lines.tail->value;
    size_t length = strlen(operations);
    size_t problem_index = 0;

    size_t total = 0;

    for (size_t i = 0; i < length; i++) {
        if (isspace(operations[i])) {
            continue;
        }

        List* operands = list_get(&problems, problem_index++);
        char operation = operations[i];

        size_t result = operation == '+' ? 0 : 1;

        Node* curr = operands->head;
        while (curr != NULL) {
            size_t value = (size_t)curr->value;

            printf("%c %lu\n", operation, value);

            result = operation == '+' ? result + value : result * value;
            curr = curr->next;
        }

        printf("result: %lu\n\n", result);

        total += result;
    }

    printf("total: %lu\n", total);
}

void part2(FILE* const file) {
    List lines = get_lines(file);

    char all_spaces[lines.length - 1];
    memset(all_spaces, ' ', lines.length - 1);
    all_spaces[lines.length - 1] = '\0';

    size_t line_length = strlen((char*)lines.head->value);

    List problems = new_list();

    List* operands = malloc(sizeof(List));
    operands->length = 0;
    operands->head = NULL;
    operands->tail = NULL;

    for (long i = (long)(line_length - 1); i >= 0; i--) {
        char number_chars[lines.length - 1];
        number_chars[lines.length - 1] = '\0';

        Node* curr = lines.head;
        size_t j = 0;
        while (curr != lines.tail) {
            char* line = curr->value;

            number_chars[j] = line[i];

            j++;
            curr = curr->next;
        }

        if (strcmp(number_chars, all_spaces) == 0) {
            append_value(&problems, operands);

            operands = malloc(sizeof(List));
            operands->length = 0;
            operands->head = NULL;
            operands->tail = NULL;

            continue;
        }

        size_t number = strtoul(number_chars, NULL, 10);
        append_value(operands, (void*)number);
    }

    append_value(&problems, operands);

    char* operations = (char*)lines.tail->value;
    size_t length = strlen(operations);
    size_t problem_index = 0;

    size_t total = 0;

    for (long i = (long)(length - 1); i >= 0; i--) {
        if (isspace(operations[i])) {
            continue;
        }

        List* operands = list_get(&problems, problem_index++);

        if (operands == NULL) {
            printf("no operands\n");
            break;
        }

        char operation = operations[i];

        size_t result = operation == '+' ? 0 : 1;

        Node* curr = operands->head;
        while (curr != NULL) {
            size_t value = (size_t)curr->value;

            printf("%c %lu\n", operation, value);

            result = operation == '+' ? result + value : result * value;
            curr = curr->next;
        }

        printf("result: %lu\n\n", result);

        total += result;
    }

    printf("total: %lu\n", total);
}

int main(int argc, char const* const* const argv) {
    run(argc, argv, part1, part2);
}
