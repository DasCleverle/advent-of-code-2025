#include "list.h"
#include <stdlib.h>

List new_list() {
    List list;
    list.length = 0;
    list.head = NULL;
    list.tail = NULL;

    return list;
}

void delete_list(List* list, char free_values) {
    Node* curr = list->head;

    while (curr != NULL) {
        Node* next = curr->next;

        if (free_values) {
            free(curr->value);
        }

        free(curr);
        curr = next;
    }
}

Node* new_node() {
    Node* new_node = malloc(sizeof(Node));
    new_node->value = 0;
    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

void append(List* list, Node* node) {
    list->length++;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    }
    else {
        list->tail->next = node;
        node->prev = list->tail;

        list->tail = node;
    }
}

void copy_to(List* list, void* array[]) {
    Node* curr = list->head;
    int i = 0;

    while (curr != NULL) {
        array[i] = curr->value;
        curr = curr->next;
        i++;
    }
}
