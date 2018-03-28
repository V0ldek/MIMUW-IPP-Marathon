/**
 * Utility macros
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

// Write to standard diagnostic output.
#define serr(args...) fprintf(stderr, args)

#ifndef NDEBUG

#define NNULL(ptr, name) if((ptr) == NULL) { \
    serr("NULL pointer in %s.\n", name); exit(1); \
    exit(1); \
}

#define ISNULL(ptr, name) if((ptr) != NULL) { \
    serr("Pointer already initialized in %s.\n", name);\
    exit(1); \
}

#else

#define NNULL(ptr, name) if((ptr) == NULL) { \
    exit(1); \
}

#define ISNULL(ptr, name) if((ptr) != NULL) { \
    exit(1); \
}

#endif //NDEBUG

#endif // UTILS_H
