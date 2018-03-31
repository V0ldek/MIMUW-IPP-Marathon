/**
 * Data structure allowing more advanced tree operations, like recursive
 * calculation of values and sustaining the tree's structure throughout
 * additions and deletions in constant time.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */

#ifndef IPP_MARATHON_MARATHON_TREE_H
#define IPP_MARATHON_MARATHON_TREE_H

#include "tree.h"

// Create the root user with ID 0 and set up the tree for further use.
void marathon_tree_initialize();

// Release all the resources allocated and destroy the entire tree.
void marathon_tree_cleanup();

// Create a new user and add him as child of parent.
// Takes constant time.
bool marathon_tree_add(int parentID, int userID);

// Remove the user from the tree.
// Takes constant time.
bool marathon_tree_remove(int userID);

// Add the given movie to the user's movie_list
// Time proportional to the number of preferences of the user.
bool marathon_tree_add_movie(int userID, int movieRating);

// Remove the given movie from the user's movie_list
// Time proportional to the number of preferences of the user.
bool marathon_tree_remove_movie(int userID, int movieRating);

// Gives a list of at most k movies that are chosen from:
// - All the user's preferences
// - Results of the marathon function for its children, but only movies that
//   have higher ratings than all of the original user's ratings are considered.
// Time proportional to k * size of the tree.
dlist_t *marathon_tree_get_marathon_list(int userID, int k);


#endif //IPP_MARATHON_MARATHON_TREE_H
