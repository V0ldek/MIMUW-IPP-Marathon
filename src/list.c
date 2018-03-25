/**
 * Linked list data structure.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "macros.h"

// Creates a new int element.
list_elem_t list_make_val(int val) {

    list_elem_t elem;
    elem.val = val;

    return elem;
}

// Creates a new ptr element.
list_elem_t list_make_ptr(void *ptr) {

    list_elem_t elem;
    elem.ptr = ptr;

    return elem;
}

// Creates a new node with passed parameters.
node_t *list_make_node(list_elem_t elem, node_t *next) {

    node_t *newNode = malloc(sizeof(node_t));

    if(newNode == NULL) {
#ifdef DEBUG
        serr("Out of memory in list_make_node.\n");
#endif
        exit(1);
    }

    newNode->elem = elem;
    newNode->next = next;

    return newNode;
}

// Creates a new list with passed parameters.
list_t *list_make(node_t *head, node_t *tail) {

    list_t *newList = malloc(sizeof(list_t));

    if(newList == NULL) {
#ifdef DEBUG
        serr("Out of memory in list_make.\n");
#endif
        exit(1);
    }

    newList->head = head;
    newList->tail = tail;

    return newList;
}

// Insert a new node between head and head->next.
void list_insert(node_t *node, list_elem_t elem) {

    if(node == NULL || node->next == NULL) {
#ifdef DEBUG
        serr("NULL pointer in list_insert.\n");
#endif
        exit(1);
    }

    node_t *next = node->next;

    node->next = list_make_node(elem, next);
}

// Links the two lists, attaching other to list's tail.
void list_link(list_t *list, list_t *other) {

    if(list == NULL || other == NULL) {
#ifdef DEBUG
        serr("NULL pointer in list_link.\n");
#endif
        exit(1);
    }

    list->tail->next = other->head;
    list->tail = other->tail;
}

// Destroy the element after node.
// List [head, next1, next2] becomes [head, next2].
// Destroying a list element does not free resources from the contained
// void* elem!
void list_erase(node_t *node) {

    if(node == NULL || node->next == NULL) {
#ifdef DEBUG
        serr("NULL pointer in list_erase.\n");
#endif
        exit(1);
    }

    node_t *next = node->next->next;

    free(node->next);

    node->next = next;
}

// Create a new node at the head.
void list_push_front(list_t *list, list_elem_t elem) {

    if(list == NULL) {
#ifdef DEBUG
        serr("NULL pointer in list_push_front.\n");
#endif
        exit(1);
    }

    list->head = list_make_node(elem, list->head);
}

// Destroy the head of the list.
void list_pop_front(list_t *list) {

    if(list == NULL || list->head == NULL) {
#ifdef DEBUG
        serr("NULL pointer in list_pop_front.\n");
#endif
        exit(1);
    }

    node_t *next = list->head->next;

    free(list->head);

    list->head = next;
}

// Create a new node at the tail.
void list_push_back(list_t *list, list_elem_t elem) {

    if(list == NULL) {
#ifdef DEBUG
        serr("NULL pointer in list_push_back.\n");
#endif
        exit(1);
    }

    node_t *oldTail = list->tail;

    list->tail = list_make_node(elem, NULL);

    oldTail->next = list->tail;
}

// Destroy the list and all elements in it until reaching NULL.
// NULL the passed pointer to avoid dereferencing a destroyed list later.
// Destroying a list element does not free resources from the contained
// void* ptr in elem!
void list_destroy(list_t **list) {

    if(*list == NULL) {

        return;
    }

    node_t *iter = (*list)->head;

    while(iter != NULL) {

        node_t *next = iter->next;

        free(iter);

        iter = next;
    }

    free(*list);

    *list = NULL;
}