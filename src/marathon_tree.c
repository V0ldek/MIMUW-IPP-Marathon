/**
 * Data structure allowing more advanced tree operations, like recursive
 * calculation of values and sustaining the tree's structure throughout
 * additions and deletions in amortized constant time for each operation.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include <stdlib.h>
#include "marathon_tree.h"
#include "movie_list.h"

// Array holding all nodes, allowing for constant access time.
static tree *users[MAX_NODES];

// Internal auxiliary function calculating the marathon list recursively.
static list_t *marathon_tree_calculate_marathon_list(tree *root, int k);

// Internal function releasing resources for a single node.
static void marathon_tree_destroy_node(tree **node);

// Create the root user with ID 0
tree *marathon_tree_initialize() {

    users[0] = tree_make(NULL, NULL);

    return users[0];
}

// Create a new user and add him as child of parent.
// Takes constant time.
// TODO: handle the case of a dead node.
bool marathon_tree_add(int parentID, int userID) {

    tree *oldUser = users[userID];
    tree *parent = users[parentID];

    if(parent == NULL || !parent->bIsAlive) {
        return false;
    }

    if(oldUser != NULL) {

        if(oldUser->bIsAlive == true) {
            return false;
        }

        list_link(oldUser->children, parent->children);
    }

    tree *newTree = tree_make(NULL, NULL);

    users[userID] = newTree;

    tree_add(users[parentID], newTree);
}

// Remove the user from the tree.
// Amortized constant time.
bool marathon_tree_remove(int userID) {

    tree_mark_dead(users[userID]);

    if(users[userID]->children == NULL) {
        tree_destroy(&users[userID]);
    }
}

// Add the given movie to the user's movie_list
// Time proportional to the number of preferences of the user.
bool marathon_tree_add_movie(int userID, int movieRating) {

    movie_list_add((list_t *) users[userID]->value, movieRating);
}

// Remove the given movie from the user's movie_list
// Time proportional to the number of preferences of the user.
bool marathon_tree_remove_movie(int userID, int movieRating) {

    movie_list_remove((list_t *) &users[userID]->value, movieRating);
}

// Gives a list of at most k movies that are chosen from:
// - All the user's preferences
// - Results of the marathon function for its children, but only movies that
//   have higher ratings than all of the original user's ratings are considered.
// Time proportional to k * size of the tree.
list_t *marathon_tree_get_marathon_list(int userID, int k) {

    tree *root = users[userID];

    return marathon_tree_calculate_marathon_list(root, k);
}

// Internal auxiliary function calculating the marathon list recursively.
// TODO: handle the case of a dead node.
static list_t *marathon_tree_calculate_marathon_list(tree *root, int k) {

    list *resultMovieList = NULL;
    int resultMovieListSize = 0;

    list *iter = root->value;

    // Firstly add k best movies from the root.
    while(iter != NULL && resultMovieListSize < k) {

        movie_list_add(resultMovieList, iter->elem.val);
        ++resultMovieListSize;

        iter = iter->next;
    }

    iter = root->children;

    // Pick movies from the children.
    while(iter != NULL) {

        tree *child = iter->elem.ptr;

        list *childMovieList = marathon_tree_calculate_marathon_list(child, k);

        movie_list_merge(&resultMovieList, &childMovieList);
    }

    // Leave only k best movies.
    movie_list_shrink(&resultMovieList, k);

    return resultMovieList;
}

// Internal function releasing resources for a single node.
void marathon_tree_destroy_node(tree **node) {

    if(*node == NULL) {
        return;
    }

    list_destroy(&(*node)->children);
    list_destroy((list_t **) &(*node)->value);
    tree_destroy(node);
}

// Recursively destroy all nodes and release their resources
void marathon_tree_destroy(tree **root) {

    if(root == NULL) {
        return;
    }

    node_t *iter = (*root)->children->head;

    while(iter != NULL) {

        marathon_tree_destroy((tree **) &iter->elem.ptr);

        iter = iter->next;
    }

    marathon_tree_destroy_node(root);
}