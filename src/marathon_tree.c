/**
 * Data structure allowing more advanced tree operations, like recursive
 * calculation of values and sustaining the tree's structure throughout
 * additions and deletions in constant time.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include "marathon_tree.h"
#include "defines.h"

// Array holding pointers to children list nodes in which the user is
// located. For root (userID = 0) it's NULL.
static dnode_t **users = NULL;

// Pointer to the root (userID = 0) of the tree.
static tree_t *root = NULL;

// Internal auxiliary function calculating the marathon list recursively.
static void marathon_tree_calculate_marathon_list(tree_t *user, long *k,
                                                  dlist_t **resultMovieList,
                                                  long supremum);

// Internal auxiliary function adding the user's movies to the list.
static void marathon_tree_add_movies_to_marathon_list(tree_t *user, long *k,
                                                      dlist_t **resultMovieList,
                                                      long supremum);

// Internal auxiliary function returning a vertex of given id.
static tree_t *marathon_tree_get_vertex(unsigned int userID);

// Internal function releasing resources for a single vertex.
static void marathon_tree_destroy_vertex(tree_t **vertex);

// Recursively destroy all vertices and release their resources, rooted in user
static void marathon_tree_destroy(tree_t **user);


// Create the root user with ID 0
void marathon_tree_initialize() {

    ISNULL(root, "marathon_tree_initialize");

    users = calloc(MAX_USER + 1, sizeof(dnode_t *));
    root = tree_make(dlist_make_list());
}

// Release all the resources allocated and destroy the entire tree.
void marathon_tree_cleanup() {

    NNULL(root, "root/marathon_tree_cleanup");
    NNULL(users, "users/marathon_tree_cleanup");

    marathon_tree_destroy(&root);

    free(users);
}

// Create a new user and add him as child of parent.
// Takes constant time.
bool marathon_tree_add(unsigned int parentID, unsigned int userID) {

    tree_t *parent = marathon_tree_get_vertex(parentID);
    tree_t *oldUser = marathon_tree_get_vertex(userID);

    // Parent is dead or the user already exists.
    if(parent == NULL || oldUser != NULL) {
        return false;
    }

    tree_t *user = tree_make(dlist_make_list());

    // Adds user to the end of the parent's children list.
    tree_add(parent, user);

    users[userID] = dlist_get_back(parent->children);

    return true;
}

// Remove the user from the tree.
// Amortized constant time.
bool marathon_tree_remove(unsigned int userID) {

    tree_t *user = marathon_tree_get_vertex(userID);

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
bool marathon_tree_add_movie(unsigned int userID, long movieRating) {

    tree_t *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return false;
    }

    dlist_t *list = user->value;

    dnode_t *iter = list->head;

    // Skip over greater elements.
    while(dlist_next(iter) != NULL &&
          dlist_next(iter)->elem.num > movieRating) {

        iter = iter->next;
    }

    if(dlist_next(iter) == NULL || dlist_next(iter)->elem.num != movieRating) {

        dlist_insert_after(iter, dlist_make_elem_num(movieRating));

        return true;
    }

    return false;
}

// Remove the given movie from the user's movie_list
// Time proportional to the number of preferences of the user.
bool marathon_tree_remove_movie(unsigned int userID, long movieRating) {

    tree_t *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return false;
    }

    dlist_t *list = user->value;

    dnode_t *iter = dlist_get_front(list);

    // Skip over greater elements
    while(dlist_is_valid(iter) && iter->elem.num > movieRating) {
        iter = dlist_next(iter);
    }

    if(dlist_is_valid(iter) && iter->elem.num == movieRating) {

        dlist_remove(iter);

        return true;
    }

    return false;

}

// Gives a list of at most k movies that are chosen from:
// - All the user's preferences
// - Results of the marathon function for its children, but only movies that
//   have higher ratings than all of the original user's ratings are considered.
// Time proportional to k * size of the tree.
dlist_t *marathon_tree_get_marathon_list(unsigned int userID, long k) {

    tree_t *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return NULL;
    }
    if(k == 0) {
        return dlist_make_list();
    }

    dlist_t *resultMovieList = dlist_make_list();

    marathon_tree_calculate_marathon_list(user, &k, &resultMovieList, -1);

    return resultMovieList;
}

// Internal auxiliary function calculating the marathon list recursively.
static void marathon_tree_calculate_marathon_list(tree_t *user, long *k,
                                                  dlist_t **resultMovieList,
                                                  long supremum) {

    dnode_t *iter = dlist_get_front(user->value);

    long mySupremum = supremum;

    if(dlist_is_valid(iter) && iter->elem.num > mySupremum) {
        mySupremum = iter->elem.num;
    }

    marathon_tree_add_movies_to_marathon_list(user, k, resultMovieList,
                                              supremum);

    // Recurse over children nodes.
    dnode_t *childIter = dlist_get_front(user->children);

    while(dlist_is_valid(childIter)) {

        marathon_tree_calculate_marathon_list(childIter->elem.ptr, k,
                                              resultMovieList, mySupremum);

        childIter = dlist_next(childIter);
    }
}

// Internal auxiliary function adding the user's movies to the list.
static void marathon_tree_add_movies_to_marathon_list(tree_t *user, long *k,
                                                      dlist_t **resultMovieList,
                                                      long supremum) {

    dnode_t *iter = dlist_get_front(user->value);
    dnode_t *resultIter = (*resultMovieList)->head;

    // Update the list with user's movies, assuring they are bigger
    // than the supremum.
    // If the list is less than k in length adds another element.
    // Otherwise replaces the smallest movie currently on the list.
    while(dlist_is_valid(iter) && iter->elem.num > supremum) {

        // Skip over greater elements.
        while(dlist_next(resultIter) != NULL &&
              dlist_next(resultIter)->elem.num > iter->elem.num) {

            resultIter = dlist_next(resultIter);
        }
        dnode_t *next = dlist_next(resultIter);

        // If we can add another element.
        if(*k > 0 && (next == NULL || next->elem.num != iter->elem.num)) {

            dlist_insert_after(resultIter, dlist_make_elem_num(iter->elem.num));
            --(*k);
        }
        else if(next != NULL && next->elem.num != iter->elem.num) {

            // Get the smallest element, move him to after iter and
            // update its contained value.
            dnode_t *smallest = dlist_get_back(*resultMovieList);

            dlist_insert_node_after(resultIter, smallest);

            smallest->elem.num = iter->elem.num;
        }

        iter = dlist_next(iter);
    }
}

// Internal auxiliary function returning a vertex with given id.
static tree_t *marathon_tree_get_vertex(unsigned int userID) {

    if(userID > MAX_USER) {
        return NULL;
    }

    if(userID == 0) {
        return root;
    }

    dnode_t *userLocation = users[userID];

    if(userLocation == NULL) {
        return NULL;
    }

    return userLocation->elem.ptr;
}

// Internal function releasing resources for a single vertex.
static void marathon_tree_destroy_vertex(tree_t **vertex) {

    if(*vertex == NULL) {
        return;
    }

    dlist_destroy((dlist_t **) &(*vertex)->value);
    tree_destroy(vertex);
}

// Recursively destroy all vertices and release their resources rooted in user
static void marathon_tree_destroy(tree_t **user) {

    if(*user == NULL) {
        return;
    }

    dnode_t *iter;

    while((iter = dlist_get_back((*user)->children)) != NULL) {

        marathon_tree_destroy((tree_t **) &iter->elem.ptr);

        dlist_pop_back((*user)->children);
    }

    marathon_tree_destroy_vertex(user);
}
