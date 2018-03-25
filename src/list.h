/**
 * Linked list data structure.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */

#ifndef LIST_H
#define LIST_H

// Elements held in the list - ints or pointers.
typedef union list_elem_t {

    void *ptr;
    int val;

} list_elem_t;

// Linked list generic node holding elem_t type elements.
typedef struct node {

    list_elem_t elem;
    struct node *next;

} node_t;

// List is a head and a tail.
typedef struct list {

    struct node *head;
    struct node *tail;

} list_t;

// Creates a new int element.
list_elem_t list_make_val(int val);

// Creates a new ptr element.
list_elem_t list_make_ptr(void *ptr);

// Creates a new node with passed parameters.
node_t *list_make_node(list_elem_t elem, node_t *next);

// Creates a new list with passed parameters.
list_t *list_make(node_t *head, node_t *tail);

// Create a new node between node and node->next.
// The node has to be not NULL. If adding at the front use push_front instead.
void list_insert(node_t *node, list_elem_t elem);

// Links the two lists, attaching other to list's tail.
void list_link(list_t *list, list_t *other);

// Destroy the element after node. Has to be not NULL.
// If erasing head use pop_front instead.
// List [head, next1, next2] becomes [head, next2].
// Destroying a list element does not free resources from the contained
// void* elem!
void list_erase(node_t *node);

// Create a new node at the head.
void list_push_front(list_t *list, list_elem_t elem);

// Destroy the head of the list.
void list_pop_front(list_t *list);

// Create a new node at the tail.
void list_push_back(list_t *list, list_elem_t elem);

// Destroy the list and all elements in it until reaching NULL.
// NULL the passed pointer to avoid dereferencing a destroyed list later.
// Destroying a list element does not free resources from the contained
// void* ptr in elem!
void list_destroy(list_t **list);

#endif // LIST_H
