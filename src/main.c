/**
 * Marathon task implementation.
 *
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "defines.h"
#include "marathon_tree.h"

// Create the tree and the input buffer.
void initialize(char **buffer, size_t *bufferSize) {

    *bufferSize = INITIAL_BUFFER_SIZE;
    *buffer = malloc(*bufferSize * sizeof(char));

    marathon_tree_initialize();
}

// Release resources.
void cleanup(char **buffer, size_t *bufferSize) {

    free(*buffer);
    *bufferSize = 0;

    marathon_tree_cleanup();
}

// True iff the userID is allowed by the specification.
bool is_in_user_range(long userID) {

    return userID >= 0 && userID <= MAX_USER;
}

// True iff the movieRating is allowed by the specification.
bool is_in_movie_range(long movieRating) {

    return movieRating >= 0 && movieRating <= MAX_MOVIE;
}

// True iff the k parameter is allowed by the specification.
bool is_in_marathon_range(long k) {

    return k >= 0 && k <= MAX_MARATHON;
}

// Try to perform the addUser operation.
bool process_add_user(long parentID, long userID) {

    if(!is_in_user_range(parentID) || !is_in_user_range(userID)) {
        return false;
    }

    return marathon_tree_add((unsigned int) parentID, (unsigned int) userID);
}

// Try to perform the delUser operation.
bool process_del_user(long userID) {

    if(userID == 0 || !is_in_user_range(userID)) {
        return false;
    }

    return marathon_tree_remove((unsigned int) userID);
}

// Try to perform the addMovie operation.
bool process_add_movie(long userID, long movieRating) {

    if(!is_in_user_range(userID) || !is_in_movie_range(movieRating)) {
        return false;
    }

    return marathon_tree_add_movie((unsigned int) userID, movieRating);
}

// Try to perform the delMovie operation.
bool process_del_movie(long userID, long movieRating) {

    if(!is_in_user_range(userID) || !is_in_movie_range(movieRating)) {
        return false;
    }

    return marathon_tree_remove_movie((unsigned int) userID, movieRating);
}

// Try to perform the marathon operation.
bool process_marathon(long userID, long k) {

    if(!is_in_user_range(userID) || !is_in_marathon_range(k)) {
        return false;
    }

    dlist_t *marathonResult = marathon_tree_get_marathon_list(
            (unsigned int) userID, k);

    if(marathonResult != NULL) {
        dlist_print_num(marathonResult);
    }
    else {
        return false;
    }

    dlist_destroy(&marathonResult);

    return true;
}

// Reads a line from the standard input into buffer.
bool read_line(char **buffer, size_t *bufferSize) {

    int character;
    unsigned int pos = 0;

    while((character = fgetc(stdin)) != '\n' && character != EOF) {

        if(pos + 1 >= *bufferSize) {

            *bufferSize *= 2;
            *buffer = realloc(*buffer, *bufferSize);

            NNULL(*buffer, "read_line");
        }

        (*buffer)[pos++] = (char) character;
    }

    // The '\n' or eof is removed and replaced with a null termination.
    (*buffer)[pos] = '\0';

    return character != EOF;
}

// Processes the command in buffer by performing the appropriate the operation
// or printing the ERROR_MSG from defines.h.
void process_line(char *buffer) {

    // Plus one because of the endline.
    size_t characters = strlen(buffer) + 1;

    // Empty line
    if(characters == 1) {
        return;
    }

    // Comment
    if(buffer[0] == '#') {
        return;
    }

    // Read the command, up to two arguments and if there is anything else.
    char *command = strtok(buffer, " \n");
    char *arg1str = strtok(NULL, " \n");
    char *arg2str = strtok(NULL, " \n");
    char *remaining = strtok(NULL, " \n");

    // Get the expected length of the input including whitespaces.
    size_t readLength = (command == NULL ? 0 : strlen(command) + 1) +
                        (arg1str == NULL ? 0 : strlen(arg1str) + 1) +
                        (arg2str == NULL ? 0 : strlen(arg2str) + 1);

    // If remaining is not null, we have junk at the end of the line.
    // If readLength != characters, there were multiple whitespaces
    if(remaining != NULL || readLength != characters) {

        serr(ERROR_MSG);

        return;
    }

    long arg1 = -1, arg2 = -1;

    if(arg1str != NULL && *arg1str >= '0' && *arg1str <= '9') {
        arg1 = strtol(arg1str, NULL, 10);
    }

    if(arg2str != NULL && *arg2str >= '0' && *arg2str <= '9') {
        arg2 = strtol(arg2str, NULL, 10);
    }

    // This means that strtol conversion resulted in an overflow
    // and the argument/s was/were out of range of long.
    if(errno != 0) {

        serr(ERROR_MSG);
        errno = 0;

        return;
    }

    bool errorFlag = true;

    if(strcmp(command, CTRL_STR_ADDUSER) == 0) {

        errorFlag = !process_add_user(arg1, arg2);
    }
    else if(strcmp(command, CTRL_STR_DELUSER) == 0) {

        errorFlag = arg2str != NULL || !process_del_user(arg1);
    }
    else if(strcmp(command, CTRL_STR_ADDMOVIE) == 0) {

        errorFlag = !process_add_movie(arg1, arg2);
    }
    else if(strcmp(command, CTRL_STR_DELMOVIE) == 0) {

        errorFlag = !process_del_movie(arg1, arg2);
    }
    else if(strcmp(command, CTRL_STR_MARATHON) == 0) {

        errorFlag = !process_marathon(arg1, arg2);

        // Marathon does not print OK.
        if(!errorFlag) {
            return;
        }
    }

    if(errorFlag) {
        serr(ERROR_MSG);
    }
    else {
        printf(OK_MSG);
    }
}

int main(void) {

    size_t bufferSize;
    char *buffer;

    initialize(&buffer, &bufferSize);

    while(read_line(&buffer, &bufferSize)) {

        process_line(buffer);
    }

    cleanup(&buffer, &bufferSize);

    return 0;
}
