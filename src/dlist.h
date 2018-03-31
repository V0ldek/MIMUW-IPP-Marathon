/**
 * Doubly linked list data structure.
 * All operations with the exception of dlist_destroy and dlist_print_num
 * take constant time.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */

#ifndef DLIST_H
#define DLIST_H

#include <stdbool.h>

// Elements held in the dlist - ints or pointers.
typedef union dlist_elem_t {

    void *ptr;
    int num;

} dlist_elem_t;

// Doubly linked dlist generic node holding elem_t type elements.
typedef struct dnode_t {

    struct dnode_t *prev;
    struct dnode_t *next;
    dlist_elem_t elem;

} dnode_t;

// A list is a head and a tail. Both head and tail are always not NULL.
// Head and tail are dummy nodes. They are the only elements with NULLs as
// values of elem.
typedef struct dlist_t {

    dnode_t *head;
    dnode_t *tail;

} dlist_t;

// Makes a new empty list object. Always takes at least 2 * sizeof(dnode_t)
// memory for dummy objects.
dlist_t *dlist_make_list();

// Makes a new node object.
dnode_t *dlist_make_node(dnode_t *prev, dlist_elem_t elem, dnode_t *next);

// Makes a new dlist_elem_t object with passed pointer as value ptr.
dlist_elem_t dlist_make_elem_ptr(void *ptr);

// Makes a new dlist_elem_t object with passed integer as value num.
dlist_elem_t dlist_make_elem_num(int num);

// Returns the actual first element of the list (not dummy).
dnode_t *dlist_get_front(dlist_t *list);

// Returns the actual last element of the list (not dummy).
dnode_t *dlist_get_back(dlist_t *list);

// Returns true if passed node is a correct, non-dummy element of a list.
bool dlist_is_valid(dnode_t *iter);

// Returns the element after iter or NULL if it is the last element
// (ignores dummies).
dnode_t *dlist_next(dnode_t *iter);

// Creates a new node with passed value and adds it at the end of the list.
void dlist_push_back(dlist_t *list, dlist_elem_t elem);

// Creates a new node after the passed one.
// The passed node has to be not the tail.
void dlist_insert_after(dnode_t *iter, dlist_elem_t elem);

// Cuts the other node from its list and inserts after iter.
// The passed node has to be not the tail.
void dlist_insert_node_after(dnode_t *iter, dnode_t *other);

// Inserts all the elements from the passed list after the given node.
// The node has to be not the tail.
// The elements are moved, so the other list is emptied as a result.
// Takes constant time.
void dlist_insert_list_after(dnode_t *iter, dlist_t *other);

// Removes the node from the list. It has to be a valid, non-dummy node.
void dlist_remove(dnode_t *iter);

// Removes the last element (ignores dummies)
void dlist_pop_back(dlist_t *list);

// Prints the list assuming it contains integers.
// Prints EMPTY_LIST_MSG if it is empty.
void dlist_print_num(dlist_t *list);

// Destroys all elements on the list.
// Warning: does not release any resources contained in ptr elements.
void dlist_destroy(dlist_t **list);


#endif // DLIST_H
