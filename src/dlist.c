/**
 * Linked list data structure.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include "dlist.h"
#include "defines.h"

// Makes a new empty list object. Always takes at least 2 * sizeof(dnode_t)
// memory for dummy objects.
dlist_t *dlist_make_list() {

    dlist_t *list = malloc(sizeof(dlist_t));

    NNULL(list, "dlist_make_list");

    list->head = dlist_make_node(NULL, dlist_make_elem_ptr(NULL), NULL);
    list->tail = dlist_make_node(list->head, dlist_make_elem_ptr(NULL), NULL);
    list->head->next = list->tail;

    return list;
}

// Makes a new node object.
dnode_t *dlist_make_node(dnode_t *prev, dlist_elem_t elem, dnode_t *next) {

    dnode_t *node = malloc(sizeof(dnode_t));

    NNULL(node, "dlist_make_node");

    node->prev = prev;
    node->elem = elem;
    node->next = next;

    return node;
}

// Makes a new dlist_elem_t object with passed pointer as value ptr.
dlist_elem_t dlist_make_elem_ptr(void *ptr) {

    dlist_elem_t elem;
    elem.ptr = ptr;

    return elem;

}

// Makes a new dlist_elem_t object with passed integer as value num.
dlist_elem_t dlist_make_elem_num(int num) {

    dlist_elem_t elem;
    elem.num = num;

    return elem;
}

// Returns the actual first element of the list (not dummy).
dnode_t *dlist_get_front(dlist_t *list) {

    NNULL(list, "dlist_get_front");

    return dlist_is_valid(list->head->next) ? list->head->next : NULL;
}

// Returns the actual last element of the list (not dummy).
dnode_t *dlist_get_back(dlist_t *list) {

    NNULL(list, "dlist_get_back");

    return dlist_is_valid(list->tail->prev) ? list->tail->prev : NULL;
}

// Returns true if passed node is a correct, non-dummy element of a list.
bool dlist_is_valid(dnode_t *iter) {

    return iter != NULL && iter->next != NULL && iter->prev != NULL;
}

// Returns the element after iter or NULL if it is the last element
// (ignores dummies).
dnode_t *dlist_next(dnode_t *iter) {

    NNULL(iter, "dlist_next");

    return dlist_is_valid(iter->next) ? iter->next : NULL;
}

// Creates a new node with passed value and adds it at the end of the list.
void dlist_push_back(dlist_t *list, dlist_elem_t elem) {

    NNULL(list, "list/dlist_push_back");
    NNULL(list->tail, "tail/dlist_push_back");

    dlist_insert_after(list->tail->prev, elem);
}

// Creates a new node after the passed one.
// The passed node has to be not the tail.
void dlist_insert_after(dnode_t *iter, dlist_elem_t elem) {

    NNULL(iter, "iter/dlist_insert_after");
    NNULL(iter->next, "next/dlist_insert_after");

    dnode_t *oldNext = iter->next;

    dnode_t *newNode = dlist_make_node(iter, elem, oldNext);

    iter->next = newNode;
    oldNext->prev = newNode;
}

// Cuts the other node from its list and inserts after iter.
// The passed node has to be not the tail.
void dlist_insert_node_after(dnode_t *iter, dnode_t *other) {

    NNULL(iter->next, "dlist_insert_node_after");

    if(other != iter->next) {

        dnode_t *oldIterNext = iter->next;
        dnode_t *oldOtherNext = other->next;
        dnode_t *oldOtherPrev = other->prev;

        oldOtherPrev->next = oldOtherNext;
        oldOtherNext->prev = oldOtherPrev;

        oldIterNext->prev = other;
        iter->next = other;

        other->prev = iter;
        other->next = oldIterNext;
    }
}

// Inserts all the elements from the passed list after the given node.
// The node has to be not the tail.
// The elements are moved, so the other list is emptied as a result.
// Takes constant time.
void dlist_insert_list_after(dnode_t *iter, dlist_t *other) {

    NNULL(iter, "iter/dlist_insert_list_after");
    NNULL(other, "other/dlist_insert_list_after");
    NNULL(iter->next, "next/dlist_insert_list_after");
    NNULL(other->head, "head/dlist_insert_list_after");
    NNULL(other->tail, "tail/dlist_insert_list_after");

    dnode_t *otherFront = dlist_get_front(other);
    dnode_t *otherBack = dlist_get_back(other);

    // List is empty, nothing to insert.
    if(otherFront == NULL) {
        return;
    }

    dnode_t *oldNext = iter->next;

    iter->next = otherFront;
    otherFront->prev = iter;

    oldNext->prev = otherBack;
    otherBack->next = oldNext;

    other->head->next = NULL;
    other->tail->prev = NULL;
}

// Removes the node from the list. It has to be a valid, non-dummy node.
void dlist_remove(dnode_t *iter) {

    NNULL(iter, "iter/dlist_remove");
    NNULL(iter->next, "next/dlist_remove");
    NNULL(iter->prev, "prev/dlist_remove");

    iter->next->prev = iter->prev;
    iter->prev->next = iter->next;

    free(iter);
}

// Removes the last element (ignores dummies)
void dlist_pop_back(dlist_t *list) {

    NNULL(list, "list/dlist_pop_back");
    NNULL(list->tail, "tail/dlist_pop_back");

    if(dlist_is_valid(list->tail->prev)) {
        dlist_remove(list->tail->prev);
    }
}

// Prints the list assuming it contains integers.
// Prints EMPTY_LIST_MSG if it is empty.
void dlist_print_num(dlist_t *list) {

    NNULL(list, "print_list");

    dnode_t *iter = dlist_get_front(list);

    if(iter == NULL) {

        printf(EMPTY_LIST_MSG);

        return;
    }

    while(dlist_next(iter) != NULL) {

        printf("%d ", iter->elem.num);

        iter = dlist_next(iter);
    }

    printf("%d\n", iter->elem.num);
}

// Destroys all elements on the list.
// Warning: does not release any resources contained in ptr elements.
void dlist_destroy(dlist_t **list) {

    NNULL(*list, "dlist_destroy");

    while(dlist_get_back(*list) != NULL) {
        dlist_pop_back(*list);
    }

    free((*list)->head);
    free((*list)->tail);
    free(*list);

    *list = NULL;
}