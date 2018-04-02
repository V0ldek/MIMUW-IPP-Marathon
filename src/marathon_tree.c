/**
 * Implementation of marathon_tree.h.
 *
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
static void
marathon_tree_calculate_marathon_list(tree_t *user, long *remainingSpace,
                                      dlist_t **resultMovieList,
                                      long supremum);

// Internal auxiliary function adding the user's movies to the list.
static void
marathon_tree_add_movies_to_marathon_list(tree_t *user, long *remainingSpace,
                                          dlist_t **resultMovieList,
                                          long threshold);

// Internal auxiliary function returning a vertex of given id or NULL if such
// user does not exist.
static tree_t *marathon_tree_get_vertex(unsigned int userID);

// Internal function releasing resources for a single vertex.
static void marathon_tree_destroy_vertex(tree_t **vertex);

// Recursively destroy all vertices and release their resources, rooted in user.
static void marathon_tree_destroy(tree_t **user);


void marathon_tree_initialize() {

    ISNULL(root, "marathon_tree_initialize");

    users = calloc(MAX_USER + 1, sizeof(dnode_t *));

    // Assure alloc did not fail.
    NNULL(users, "marathon_tree_initialize");

    root = tree_make(dlist_make_list());
}

void marathon_tree_cleanup() {

    NNULL(root, "root/marathon_tree_cleanup");
    NNULL(users, "users/marathon_tree_cleanup");

    marathon_tree_destroy(&root);

    free(users);
}

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

bool marathon_tree_remove(unsigned int userID) {

    tree_t *user = marathon_tree_get_vertex(userID);

    // Can never delete a root or a dead user.
    if(user == NULL || user == root) {
        return false;
    }

    dnode_t *userNode = users[userID];
    dnode_t *prevNode = userNode->prev;

    // Remove the user from his parent's children list,
    // but link all of user's children to that list in his place.
    // Since it is only a few pointers reattached the time is constant.
    dlist_remove(userNode);

    dlist_insert_list_after(prevNode, user->children);

    marathon_tree_destroy(&user);

    users[userID] = NULL;

    return true;
}

bool marathon_tree_add_movie(unsigned int userID, long movieRating) {

    tree_t *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return false;
    }

    dlist_t *movieList = user->value;

    dnode_t *iter = movieList->head;

    // Skip over greater elements.
    while(dlist_next(iter) != NULL &&
          dlist_next(iter)->elem.num > movieRating) {

        iter = iter->next;
    }

    // Only insert if the element is already on the list.
    if(dlist_next(iter) == NULL || dlist_next(iter)->elem.num != movieRating) {

        dlist_insert_after(iter, dlist_make_elem_num(movieRating));

        return true;
    }

    return false;
}

bool marathon_tree_remove_movie(unsigned int userID, long movieRating) {

    tree_t *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return false;
    }

    dlist_t *movieList = user->value;

    dnode_t *iter = dlist_get_front(movieList);

    // Skip over greater elements.
    while(dlist_is_valid(iter) && iter->elem.num > movieRating) {
        iter = dlist_next(iter);
    }

    // Check if the movie exists on the list.
    if(dlist_is_valid(iter) && iter->elem.num == movieRating) {

        dlist_remove(iter);

        return true;
    }

    return false;

}

dlist_t *marathon_tree_get_marathon_list(unsigned int userID, long k) {

    tree_t *user = marathon_tree_get_vertex(userID);

    if(user == NULL) {
        return NULL;
    }

    // Make sure we do not go through the entire tree
    // needlessly in the corner case.
    if(k == 0) {
        return dlist_make_list();
    }

    dlist_t *resultMovieList = dlist_make_list();

    // Calculate the list recursively.
    // Initial supremum can be -1 because all movie rating's are >= 0.
    marathon_tree_calculate_marathon_list(user, &k, &resultMovieList, -1);

    return resultMovieList;
}

static void
marathon_tree_calculate_marathon_list(tree_t *user, long *remainingSpace,
                                      dlist_t **resultMovieList,
                                      long supremum) {

    dnode_t *iter = dlist_get_front(user->value);

    // Get the new supremum for this subtree.
    long newSupremum = supremum;

    if(dlist_is_valid(iter) && iter->elem.num > newSupremum) {
        newSupremum = iter->elem.num;
    }

    // Update the list with values from this user's movie list.
    marathon_tree_add_movies_to_marathon_list(user, remainingSpace,
                                              resultMovieList,
                                              supremum);

    // Recurse over children nodes.
    dnode_t *childIter = dlist_get_front(user->children);

    while(dlist_is_valid(childIter)) {

        marathon_tree_calculate_marathon_list(childIter->elem.ptr,
                                              remainingSpace,
                                              resultMovieList, newSupremum);

        childIter = dlist_next(childIter);
    }
}

// Adds the elements from the user's movie list to the resultMovieList.
// Ignores elements lower than the threshold. Extends the list by at most
// remainingSpace elements. Tries to maximise the elements on the list by
// removing lowest elements when remainingSpace is zero.
static void
marathon_tree_add_movies_to_marathon_list(tree_t *user, long *remainingSpace,
                                          dlist_t **resultMovieList,
                                          long threshold) {

    dnode_t *iter = dlist_get_front(user->value);
    dnode_t *resultIter = (*resultMovieList)->head;

    // Update the list with user's movies, assuring they are bigger
    // than the threshold.
    // If the list is less than remainingSpace in length adds another element.
    // Otherwise replaces the smallest movie currently on the list.
    while(dlist_is_valid(iter) && iter->elem.num > threshold) {

        // Skip over greater elements.
        while(dlist_next(resultIter) != NULL &&
              dlist_next(resultIter)->elem.num > iter->elem.num) {

            resultIter = dlist_next(resultIter);
        }
        dnode_t *next = dlist_next(resultIter);

        // If we can add another element.
        if(*remainingSpace > 0 &&
           (next == NULL || next->elem.num != iter->elem.num)) {

            dlist_insert_after(resultIter, dlist_make_elem_num(iter->elem.num));
            --(*remainingSpace);
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

static void marathon_tree_destroy_vertex(tree_t **vertex) {

    if(*vertex == NULL) {
        return;
    }

    dlist_destroy((dlist_t **) &(*vertex)->value);
    tree_destroy(vertex);
}

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
