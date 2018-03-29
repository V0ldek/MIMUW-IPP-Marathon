/**
 * Data structure allowing more advanced tree operations, like recursive
 * calculation of values and sustaining the tree's structure throughout
 * additions and deletions in amortized constant time for each operation.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include <stdlib.h>
#include <stdio.h>
#include "marathon_tree.h"
#include "movie_list.h"
#include "utils.h"

// Array holding pointers to children list nodes in which the user is
// located. For root (userID = 0) it's NULL.
static dnode_t **users = NULL;

// Pointer to the root (userID = 0) of the tree.
static tree *root = NULL;

// Internal auxiliary function calculating the marathon list recursively.
static dlist_t *marathon_tree_calculate_marathon_list(tree *user, int k);

// Internal auxiliary function returning a vertex of given id.
static tree *marathon_tree_get_vertex(int userID);

// Internal function releasing resources for a single vertex.
static void marathon_tree_destroy_vertex(tree **vertex);

// Recursively destroy all vertices and release their resources, rooted in user
static void marathon_tree_destroy(tree **user);

// Create the root user with ID 0
void marathon_tree_initialize() {

    ISNULL(root, "marathon_tree_initialize");

    users = calloc(MAX_NODES, sizeof(dnode_t *));
    root = tree_make(dlist_make_list());
}

// Create a new user and add him as child of parent.
// Takes constant time.
bool marathon_tree_add(int parentID, int userID) {

    tree *parent = marathon_tree_get_vertex(parentID);
    tree *oldUser = marathon_tree_get_vertex(userID);

    // Parent is dead or the user already exists.
    if(parent == NULL || oldUser != NULL) {
        return false;
    }

    tree *user = tree_make(dlist_make_list());

    // Adds user to the end of the parent's children list.
    tree_add(parent, user);

    users[userID] = dlist_get_back(parent->children);

    return true;
}

// Remove the user from the tree.
// Amortized constant time.
bool marathon_tree_remove(int userID) {

    tree *user = marathon_tree_get_vertex(userID);

    if(user == NULL || user == root) {
        return false;
    }

    dnode_t *userNode = users[userID];
    dnode_t *prevNode = userNode->prev;

    // Remove the user from his parent's children list,
    // but link all of user's children to that list in his place.
    dlist_remove(userNode);

    dlist_insert_list_after(prevNode, user->children);

    marathon_tree_destroy(&user);

    users[userID] = NULL;

    return true;
}

// Add the given movie to the user's movie_list
// Time proportional to the number of preferences of the user.
bool marathon_tree_add_movie(int userID, int movieRating) {

    tree *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return false;
    }

    return movie_list_add((dlist_t *) user->value, movieRating);
}

// Remove the given movie from the user's movie_list
// Time proportional to the number of preferences of the user.
bool marathon_tree_remove_movie(int userID, int movieRating) {

    tree *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return false;
    }

    return movie_list_remove((dlist_t *) user->value, movieRating);

}

// Gives a list of at most k movies that are chosen from:
// - All the user's preferences
// - Results of the marathon function for its children, but only movies that
//   have higher ratings than all of the original user's ratings are considered.
// Time proportional to k * size of the tree.
dlist_t *marathon_tree_get_marathon_list(int userID, int k) {

    tree *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return NULL;
    }
    if(k == 0) {
        return dlist_make_list();
    }

    return marathon_tree_calculate_marathon_list(user, k);
}

// Release all the resources allocated and destroy the entire tree.
void marathon_tree_cleanup() {

    NNULL(root, "root/marathon_tree_cleanup");
    NNULL(users, "users/marathon_tree_cleanup");

    marathon_tree_destroy(&root);

    free(users);
}

// Internal auxiliary function returning a vertex with given id.
static tree *marathon_tree_get_vertex(int userID) {

    if(userID < 0 || userID >= MAX_NODES) {
        return NULL;
    }

    if(userID == 0) {
        return root;
    }

    dnode_t *userLocation = users[userID];

    if(userLocation == NULL) {
        return NULL;
    }

    return userLocation->elem->ptr;
}

// Internal auxiliary function calculating the marathon list recursively.
static dlist_t *marathon_tree_calculate_marathon_list(tree *user, int k) {

    dlist_t *resultMovieList = dlist_make_list();
    dlist_t *myMovieList = user->value;

    dnode_t *iter = dlist_get_front(myMovieList);

    // Get the greates element on the user's list.
    // As all elements are >= 0, we can assume -1 for an empty list.
    int supremum = iter == NULL ? -1 : iter->elem->num;

    for(int i = 0; i < k && iter != NULL; ++i) {

        dlist_push_back(resultMovieList, dlist_make_elem_num(iter->elem->num));

        iter = dlist_next(iter);
    }

    iter = dlist_get_front(user->children);

    while(dlist_is_valid(iter)) {

        dlist_t *childMovieList =
                marathon_tree_calculate_marathon_list(iter->elem->ptr, k);

        movie_list_filter(childMovieList, supremum);
        movie_list_merge(resultMovieList, childMovieList);
        movie_list_shrink(resultMovieList, k);

        dlist_destroy(&childMovieList);

        iter = dlist_next(iter);
    }

    return resultMovieList;
}

// Internal function releasing resources for a single vertex.
static void marathon_tree_destroy_vertex(tree **vertex) {

    if(*vertex == NULL) {
        return;
    }

    dlist_destroy((dlist_t **) &(*vertex)->value);
    tree_destroy(vertex);
}

// Recursively destroy all vertices and release their resources rooted in user
static void marathon_tree_destroy(tree **user) {

    if(*user == NULL) {
        return;
    }

    dnode_t *iter;

    while((iter = dlist_get_back((*user)->children)) != NULL) {

        marathon_tree_destroy((tree **) &iter->elem->ptr);

        dlist_pop_back((*user)->children);
    }

    marathon_tree_destroy_vertex(user);
}
