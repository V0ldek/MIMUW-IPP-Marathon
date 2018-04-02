/**
 * Simple tree data structure.
 *
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#ifndef TREE_H
#define TREE_H

#include "dlist.h"

// Simple tree structure with a list of successors and generic pointer value.
typedef struct tree_t {

    void *value;
    dlist_t *children;

} tree_t;

// Creates a new tree node with passed value and empty children list.
tree_t *tree_make(void *value);

// Adds the otherRoot as a child of parent.
// Adds at the end of the children list.
void tree_add(tree_t *parent, tree_t *otherRoot);

// Destroys the entire tree recursively and NULLs the root pointer.
// Destroying a tree element does not free resources from the contained
// void* elem!
void tree_destroy(tree_t **root);

#endif // TREE_H
