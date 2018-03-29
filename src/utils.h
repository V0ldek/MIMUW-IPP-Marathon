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

#define CTRL_STR_ADDUSER "addUser"
#define CTRL_STR_DELUSER "delUser"
#define CTRL_STR_ADDMOVIE "addMovie"
#define CTRL_STR_DELMOVIE "delMovie"
#define CTRL_STR_MARATHON "marathon"
#define ERROR_MSG "ERROR\n"
#define OK_MSG "OK\n"
#define EMPTY_LIST_MSG "NONE\n"
#define INITIAL_BUFFER_SIZE 32

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
