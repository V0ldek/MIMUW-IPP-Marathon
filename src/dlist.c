/**
 * Implementation of dlist.h.
 *
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include "dlist.h"
#include "defines.h"

dlist_t *dlist_make_list() {

    dlist_t *list = malloc(sizeof(dlist_t));

    // Assure that malloc has not failed.
    NNULL(list, "dlist_make_list");

    list->head = dlist_make_node(NULL, dlist_make_elem_ptr(NULL), NULL);
    list->tail = dlist_make_node(list->head, dlist_make_elem_ptr(NULL), NULL);
    list->head->next = list->tail;

    return list;
}

dnode_t *dlist_make_node(dnode_t *prev, dlist_elem_t elem, dnode_t *next) {

    dnode_t *node = malloc(sizeof(dnode_t));

    // Assure that malloc has not failed.
    NNULL(node, "dlist_make_node");

    node->prev = prev;
    node->elem = elem;
    node->next = next;

    return node;
}

dlist_elem_t dlist_make_elem_ptr(void *ptr) {

    dlist_elem_t elem;
    elem.ptr = ptr;

    return elem;

}

dlist_elem_t dlist_make_elem_num(long num) {

    dlist_elem_t elem;
    elem.num = num;

    return elem;
}

dnode_t *dlist_get_front(dlist_t *list) {

    NNULL(list, "dlist_get_front");

    return dlist_is_valid(list->head->next) ? list->head->next : NULL;
}

dnode_t *dlist_get_back(dlist_t *list) {

    NNULL(list, "dlist_get_back");

    return dlist_is_valid(list->tail->prev) ? list->tail->prev : NULL;
}

bool dlist_is_valid(dnode_t *iter) {

    return iter != NULL && iter->next != NULL && iter->prev != NULL;
}

dnode_t *dlist_next(dnode_t *iter) {

    NNULL(iter, "dlist_next");

    return dlist_is_valid(iter->next) ? iter->next : NULL;
}

void dlist_push_back(dlist_t *list, dlist_elem_t elem) {

    NNULL(list, "list/dlist_push_back");
    NNULL(list->tail, "tail/dlist_push_back");

    dlist_insert_after(list->tail->prev, elem);
}

void dlist_insert_after(dnode_t *iter, dlist_elem_t elem) {

    NNULL(iter, "iter/dlist_insert_after");
    NNULL(iter->next, "next/dlist_insert_after");

    dnode_t *oldNext = iter->next;

    dnode_t *newNode = dlist_make_node(iter, elem, oldNext);

    iter->next = newNode;
    oldNext->prev = newNode;
}

void dlist_insert_node_after(dnode_t *iter, dnode_t *other) {

    NNULL(iter->next, "iternext/dlist_insert_node_after");
    NNULL(other->next, "othernext/dlist_insert_node_after");
    NNULL(other->prev, "otherprev/dlist_insert_node_after");

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

void dlist_remove(dnode_t *iter) {

    NNULL(iter, "iter/dlist_remove");
    NNULL(iter->next, "next/dlist_remove");
    NNULL(iter->prev, "prev/dlist_remove");

    iter->next->prev = iter->prev;
    iter->prev->next = iter->next;

    // Note that the lem is not managed by us and is not freed.

    free(iter);
}

void dlist_pop_back(dlist_t *list) {

    NNULL(list, "list/dlist_pop_back");
    NNULL(list->tail, "tail/dlist_pop_back");

    if(dlist_is_valid(list->tail->prev)) {
        dlist_remove(list->tail->prev);
    }
}

void dlist_print_num(dlist_t *list) {

    NNULL(list, "print_list");

    dnode_t *iter = dlist_get_front(list);

    if(iter == NULL) {

        printf(EMPTY_LIST_MSG);

        return;
    }

    while(dlist_next(iter) != NULL) {

        printf("%ld ", iter->elem.num);

        iter = dlist_next(iter);
    }

    printf("%ld\n", iter->elem.num);
}

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