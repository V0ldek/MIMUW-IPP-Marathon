/**
 * Simple tree data structure.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include "list.h"

// Simple tree structure with a list of successors and generic value.
typedef struct tree {

    void *value;
    list_t *children;
    bool bIsAlive;

} tree;

// Creates a new tree with passed parameters.
tree *tree_make(void *value, list_t *children);

// Adds the tree as a child of parent.
void tree_add(tree *parent, tree *otherRoot);

// Marks the node as dead. Does not free any resources.
void tree_mark_dead(tree *node);

// Destroys the entire tree recursively and NULLs the root pointer.
// Destroying a tree element does not free resources from the contained
// void* elem!
void tree_destroy(tree **root);

#endif // TREE_H
