/**
 * Extension of the tree structure. Allows all operations specified
 * by the Marathon task. Each user is a node in the tree and contains
 * a list of movies.
 * Adding and deleting a user takes constant time.
 * Adding or deleting a movie takes time proportional to the number of movies
 * currently on the list.
 * Marathon takes O(kn) time and O(k) memory, where n is the number of nodes
 * in the user's subtree and k is the length of the resultant list.
 *
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
// Returns true iff the user was successfully added.
// Takes constant time.
bool marathon_tree_add(unsigned int parentID, unsigned int userID);

// Remove the user from the tree.
// Returns true iff the user was successfully removed.
// Takes constant time.
bool marathon_tree_remove(unsigned int userID);

// Add the given movie to the user's movie_list.
// Returns true iff the movie was successfully added.
// Time proportional to the number of preferences of the user.
bool marathon_tree_add_movie(unsigned int userID, long movieRating);

// Remove the given movie from the user's movie_list.
// Returns true iff the movie was successfully removed.
// Time proportional to the number of preferences of the user.
bool marathon_tree_remove_movie(unsigned int userID, long movieRating);

// Gives a list of at most k movies that are chosen from:
// - All the user's preferences
// - Results of the marathon function for its children, but only movies that
//   have higher ratings than all of the original user's ratings are considered.
// Time proportional to k * size of the tree.
dlist_t *marathon_tree_get_marathon_list(unsigned int userID, long k);


#endif //IPP_MARATHON_MARATHON_TREE_H
