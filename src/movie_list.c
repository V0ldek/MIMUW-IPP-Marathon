/**
 * Wrapper for a list structure that allows holding movie ids sorted
 * in descending order (removes duplicates) and removing particular movie ids.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */

#include <stdlib.h>
#include "list.h"
#include "movie_list.h"

// Adds the movie of given id to the list.
// Takes time proportional to the size of the list.
bool movie_list_add(list_t *list, int movie) {

    if(list == NULL) {

        list = list_make(NULL, NULL);
    }

    if(list->head == NULL) {

        list->head = list_make_node(list_make_val(movie), NULL);

        return true;
    }

    node_t *iter = list->head;

    // Skip over greater elements.
    while(iter->next != NULL && iter->next->elem.val > movie) {

        iter = iter->next;
    }

    if(iter->next == NULL || iter->next->elem.val != movie) {

        list_insert(iter, list_make_val(movie));

        return true;
    }

    return false;
}

// Finds and removes the movie of given id from the list.
// Returns true if the movie was removed, false if it did not exist in the list.
// Takes time proportional to the size of the list.
bool movie_list_remove(list_t *list, int movie) {

    if(list == NULL || list->head == NULL) {

        return false;
    }

    if(list->head->elem.val == movie) {

        list_pop_front(list);

        return true;
    }

    node_t *iter = list->head;

    // Skip over greater elements.
    while(iter->next != NULL && iter->next->elem.val > movie) {

        iter = iter->next;
    }

    if(iter->next != NULL && iter->next->elem.val == movie) {

        list_erase(iter);

        return true;
    }

    return false;
}

// Merges the lists leaving all elements from the master lists and each
// element from the other list that is greater than every element in the
// original master list. Merged elements are removed from other
// and added to master.
// Takes time proportional to the size of other.
void movie_list_merge(list_t **master, list_t **other) {

    if(*other == NULL || (*other)->head == NULL) {
        return;
    }

    if(*master == NULL || (*master)->head == NULL) {

        *master = *other;

        return;
    }

    // Greatest element in the master list.
    int masterSupremum = (*master)->head->elem.val;

    node_t *iter = (*other)->head;

    // Find the last element greater than the supremum.
    while(iter->next != NULL && iter->next->elem.val > masterSupremum) {

        iter = iter->next;
    }

    // Slice the other list leaving elements not greater than the supremum
    // as a rejected list.
    node_t *rejectedHead = iter->next;
    node_t *rejectedTail = (*other)->tail;

    (*other)->tail = iter;
    iter->next = NULL;

    // We first attach master to other and then point to other as the new
    // merged list, because we want the greatest elements in the front.
    list_link(*other, *master);

    *master = *other;

    // Set the other list to contain rejected elements.
    (*other)->head = rejectedHead;
    (*other)->tail = rejectedTail;

    if((*other)->head == NULL) {
        (*other) = NULL;
    }
}

// Shrinks the list to hold at most k greatest elements.
// Takes time proportional to k.
void movie_list_shrink(list_t **list, int k) {

    if(*list == NULL || (*list)->head == NULL) {
        return;
    }

    if(k == 0) {

        list_destroy(list);

        return;
    }

    node_t *iter = (*list)->head;

    // Find the last element that will remain and erase all other.
    while(iter->next != NULL && k > 1) {

        iter = iter->next;
        --k;
    }

    while(iter->next != NULL) {

        list_erase(iter);
    }
}