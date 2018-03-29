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

// Destroys all elements lesser or equal to the requirement.
void movie_list_filter(dlist_t *list, int requirement) {

    NNULL(list, "movie_list_filter");

    dnode_t *iter = list->head;

    // Find the last remaining element.
    while(dlist_next(iter) != NULL &&
            dlist_next(iter)->elem->num > requirement) {

        iter = dlist_next(iter);
    }

    // Remove all further elements.
    while(dlist_next(iter) != NULL) {

        dlist_pop_back(list);
    }
}

// Merges two lists sustaining the descending ordering.
// The resultant other list contains only the elements
// that were already on the first list.
// Takes time proportional to the size of two lists.
void movie_list_merge(dlist_t *master, dlist_t *other) {

    NNULL(master, "master/movie_list_merge");
    NNULL(other, "other/movie_list_merge");

    dnode_t *iter = master->head;
    dnode_t *otherIter = dlist_get_front(other);

    // Process all the elements on the other list.
    while(dlist_is_valid(otherIter)) {

        // Skip over elements in master that are greater than the current one.
        while(dlist_next(iter) != NULL &&
              dlist_next(iter)->elem->num > otherIter->elem->num) {

            iter = dlist_next(iter);
        }

        dnode_t *next = dlist_next(otherIter);

        // Only add if the result is unique.
        if(dlist_next(iter) == NULL ||
                dlist_next(iter)->elem->num < otherIter->elem->num) {

            dnode_t *oldNext = iter->next;
            dnode_t *oldOtherPrev = otherIter->prev;
            dnode_t *oldOtherNext = otherIter->next;

            oldOtherPrev->next = oldOtherNext;
            oldOtherNext->prev = oldOtherPrev;

            otherIter->prev = iter;
            otherIter->next = oldNext;

            oldNext->prev = otherIter;
            iter->next = otherIter;
        }

        otherIter = next;
    }
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