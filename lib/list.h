#pragma once

#include <stdlib.h>

typedef struct Node {
    void* value;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    size_t length;
    Node* head;
    Node* tail;
} List;

List new_list();
void delete_list(List* list, char free_values);

Node* new_node();
void append(List* list, Node* node);
void copy_to(List* list, void* array[]);
