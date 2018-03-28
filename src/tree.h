/**
 * Simple tree data structure.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include "dlist.h"

// Simple tree structure with a list of successors and generic value.
typedef struct tree {

    void *value;
    dlist_t *children;

} tree;

// Creates a new tree with passed value and empty children list.
tree *tree_make(void *value);

// Adds the tree as a child of parent.
// Adds at the end of the children list.
void tree_add(tree *parent, tree *otherRoot);

// Destroys the entire tree recursively and NULLs the root pointer.
// Destroying a tree element does not free resources from the contained
// void* elem!
void tree_destroy(tree **root);

#endif // TREE_H
