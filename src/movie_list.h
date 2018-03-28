/**
 * Wrapper for a list structure that allows holding movie ids sorted
 * in descending order (removes duplicates) and removing particular movie ids.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#ifndef IPP_MARATHON_MOVIE_LIST_H
#define IPP_MARATHON_MOVIE_LIST_H

#include <stdbool.h>
#include "dlist.h"

// Adds the movie of given id to the list.
// Returns true if the movie was added, false if it was already in the list.
// Takes time proportional to the size of the list.
bool movie_list_add(dlist_t *list, int movie);

// Finds and removes the movie of given id from the list.
// Returns true if the movie was removed, false if it did not exist in the list.
// Takes time proportional to the size of the list.
bool movie_list_remove(dlist_t *list, int movie);

// Merges the lists leaving all elements from the master lists and each
// element from the other list that is greater than every element in the
// original master list. Merged elements are removed from other
// and added to master.
// Takes time proportional to the elements added.
void movie_list_merge(dlist_t *master, dlist_t *other);

// Shrinks the list to hold at most k greatest elements.
// Takes time proportional to k.
void movie_list_shrink(dlist_t *list, int k);

#endif //IPP_MARATHON_MOVIE_LIST_H
