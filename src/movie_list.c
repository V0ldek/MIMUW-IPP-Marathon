/**
 * Wrapper for a list structure that allows holding movie ids sorted
 * in descending order (removes duplicates) and removing particular movie ids.
 * Author: Mateusz ienieczko
 * Copyright (C) 2018
 */

#include <stdio.h>
#include "dlist.h"
#include "movie_list.h"
#include "utils.h"

// Adds the movie of given id to the list.
// Returns true if the movie was added, false if it was already in the list.
// Takes time proportional to the size of the list.
bool movie_list_add(dlist_t *list, int movie) {

    NNULL(list, "movie_list_add");

    dnode_t *iter = dlist_get_front(list);

    // Check if there are greater elements.
    if(iter == NULL || iter->elem->num < movie) {

        dlist_push_front(list, dlist_make_elem_num(movie));

        return true;
    }

    // Don't add if this is the greatest element.
    if(iter->elem->num == movie) {
        return false;
    }

    // Skip over greater elements.
    while(dlist_next(iter) != NULL && dlist_next(iter)->elem->num > movie) {

        iter = iter->next;
    }

    if(dlist_next(iter) == NULL || dlist_next(iter)->elem->num != movie) {

        dlist_insert_after(iter, dlist_make_elem_num(movie));

        return true;
    }

    return false;
}

// Finds and removes the movie of given id from the list.
// Returns true if the movie was removed, false if it did not exist in the list.
// Takes time proportional to the size of the list.
bool movie_list_remove(dlist_t *list, int movie) {

    NNULL(list, "movie_list_remove");

    dnode_t *iter = dlist_get_front(list);

    // Skip over greater elements
    while(dlist_is_valid(iter) && iter->elem->num > movie) {

        iter = dlist_next(iter);
    }

    if(dlist_is_valid(iter) && iter->elem->num == movie) {

        dlist_remove(iter);

        return true;
    }

    return false;
}

// Merges the lists leaving all elements from the master list and each
// element from the other list that is greater than every element in the
// original master list. Merged elements are removed from other
// and added to master.
// Takes time proportional to the elements added.
void movie_list_merge(dlist_t *master, dlist_t *other) {

    NNULL(master, "master/movie_list_merge");
    NNULL(other, "other/movie_list_merge");

    if(dlist_get_front(master) == NULL) {

        dnode_t *oldHead = master->head;
        dnode_t *oldTail = master->tail;

        master->head = other->head;
        master->tail = other->tail;

        other->head = oldHead;
        other->tail = oldTail;

        return;
    }

    // Greatest element in the master list.
    int masterSupremum = dlist_get_front(master)->elem->num;

    dnode_t *iter = dlist_get_front(other);

    if(iter == NULL || iter->elem->num <= masterSupremum) {
        return;
    }

    // Find the last element greater than the supremum.
    while(dlist_next(iter) != NULL &&
          dlist_next(iter)->elem->num > masterSupremum) {

        iter = dlist_next(iter);
    }

    // Slice the other list and rearrange the pointers.
    // Old other head becomes new master's head, use the old
    // master's head as the new head of the other, which contains
    // the rejected elements.

    dnode_t *oldMasterHead = master->head;
    dnode_t *oldOtherHead = other->head;
    dnode_t *oldIterNext = iter->next;
    dnode_t *newIterNext = dlist_get_front(master);

    master->head = oldOtherHead;
    other->head = oldMasterHead;

    oldMasterHead->next = oldIterNext;
    oldIterNext->prev = oldMasterHead;
    iter->next = newIterNext;
    newIterNext->prev = iter;
}

// Shrinks the list to hold at most k greatest elements.
// Takes time proportional to k.
void movie_list_shrink(dlist_t *list, int k) {

    NNULL(list, "movie_list_shrink");

    dnode_t *iter = dlist_get_front(list);

    if(iter == NULL) {
        return;
    }

    // Find the last element that will remain and erase all other.
    for(int i = 0; i < k - 1 && dlist_next(iter) != NULL; ++i) {

        iter = dlist_next(iter);
    }

    while(dlist_next(iter) != NULL) {

        dlist_pop_back(list);
    }
}