/**
 * Implementation of tree.h.
 *
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */

#include "tree.h"
#include "defines.h"

tree_t *tree_make(void *value) {

    tree_t *newTree = malloc(sizeof(tree_t));

    // Assure malloc has not failed.
    NNULL(newTree, "tree_make.\n");

    newTree->value = value;
    newTree->children = dlist_make_list();

    return newTree;
}

void tree_add(tree_t *parent, tree_t *otherRoot) {

    NNULL(parent, "parent/tree_add");
    NNULL(otherRoot, "otherRoot/tree_add");

    dlist_push_back(parent->children, dlist_make_elem_ptr(otherRoot));
}

void tree_destroy(tree_t **root) {

    if(*root == NULL) {
        return;
    }

    dlist_t *childrenList = (*root)->children;
    dnode_t *iter = dlist_get_front(childrenList);

    while(dlist_is_valid(iter)) {

        tree_destroy((tree_t **) &iter->elem.ptr);
    }

    dlist_destroy(&(*root)->children);

    // Note that the void *value is not managed by us and not freed.

    free(*root);

    *root = NULL;
}