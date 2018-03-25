/**
 * Tree data structure
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "macros.h"

// Creates a new tree with passed parameters.
tree *tree_make(void *value, list_t *children) {

    tree *newTree = malloc(sizeof(tree));

    if(newTree == NULL) {
#ifdef DEBUG
        serr("Out of memory in tree_make");
#endif
        exit(1);
    }

    newTree->value = value;
    newTree->children = children;
    newTree->bIsAlive = true;

    return newTree;
}

// Adds the tree as a child of parent.
void tree_add(tree *parent, tree *otherRoot) {

    list_push_front(parent->children, list_make_ptr(otherRoot));
}

// Marks the node as dead. Does not free any resources.
void tree_mark_dead(tree *node) {

    node->bIsAlive = false;
}


// Destroys the entire tree recursively and NULLs the root pointer.
// Destroying a tree element does not free resources from the contained
// void* elem!
void tree_destroy(tree **root) {

    node_t *iter = (*root)->children->head;

    while(iter != NULL) {

        tree_destroy(iter->elem.ptr);
        iter = iter->next;
    }

    list_destroy(&(*root)->children);
    free(*root);

    *root = NULL;
}