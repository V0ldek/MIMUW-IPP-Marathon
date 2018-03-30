/**
 * Tree data structure
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include "tree_t.h"
#include "defines.h"

// Creates a new tree with passed value and empty children list.
tree_t *tree_make(void *value) {

    tree_t *newTree = malloc(sizeof(tree_t));

    NNULL(newTree, "tree_make.\n");

    newTree->value = value;
    newTree->children = dlist_make_list();

    return newTree;
}

// Adds the tree as a child of parent.
// Adds at the end of the children list.
void tree_add(tree_t *parent, tree_t *otherRoot) {

    NNULL(parent, "parent/tree_add");
    NNULL(otherRoot, "otherRoot/tree_add");

    dlist_push_back(parent->children, dlist_make_elem_ptr(otherRoot));
}

// Destroys the entire tree recursively and NULLs the root pointer.
// Destroying a tree element does not free resources from the contained
// void* elem!
void tree_destroy(tree_t **root) {

    if(*root == NULL) {
        return;
    }

    dlist_t *childrenList = (*root)->children;
    dnode_t *iter = dlist_get_front(childrenList);

    while(dlist_is_valid(iter)) {

        tree_destroy((tree_t **) &iter->elem->ptr);

        dlist_next(iter);
    }

    dlist_destroy(&(*root)->children);

    free(*root);
    *root = NULL;
}