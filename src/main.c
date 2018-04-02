/**
 * Marathon task implementation.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "defines.h"
#include "marathon_tree.h"

size_t bufferSize;
char *buffer;


void initialize() {

    bufferSize = INITIAL_BUFFER_SIZE;
    buffer = malloc(bufferSize * sizeof(char));

    marathon_tree_initialize();
}

void cleanup() {

    free(buffer);

    marathon_tree_cleanup();
}

bool is_in_user_range(long user) {

    return user >= 0 && user <= MAX_USER;
}

bool is_in_movie_range(long movieRating) {

    return movieRating >= 0 && movieRating <= MAX_MOVIE;
}

bool is_in_marathon_range(long k) {

    return k >= 0 && k <= MAX_MARATHON;
}

bool process_add_user(long parentID, long userID) {

    if(!is_in_user_range(parentID) || !is_in_user_range(userID)) {
        return false;
    }

    return marathon_tree_add((unsigned int) parentID, (unsigned int) userID);
}

bool process_del_user(long userID) {

    if(userID == 0 || !is_in_user_range(userID)) {
        return false;
    }

    return marathon_tree_remove((unsigned int) userID);
}

bool process_add_movie(long userID, long movieRating) {

    if(!is_in_user_range(userID) || !is_in_movie_range(movieRating)) {
        return false;
    }

    return marathon_tree_add_movie((unsigned int) userID, movieRating);
}

bool process_del_movie(long userID, long movieRating) {

    if(!is_in_user_range(userID) || !is_in_movie_range(movieRating)) {
        return false;
    }

    return marathon_tree_remove_movie((unsigned int) userID, movieRating);
}

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

bool read_line() {

    int character;
    unsigned int pos = 0;

    while((character = fgetc(stdin)) != '\n' && character != EOF) {

        if(pos + 1 >= bufferSize) {

            bufferSize *= 2;
            buffer = realloc(buffer, bufferSize);

            NNULL(buffer, "read_line");
        }

        buffer[pos++] = (char) character;
    }

    // The '\n' or eof is removed and replaced with a null termination.
    buffer[pos] = '\0';

    return character != EOF;
}

void process_line() {

    size_t characters = strlen(buffer);

    // Empty line
    if(characters == 0) {
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

    // If remaining != NULL we had trash at the end of line.
    // If the second case is true we had whitespaces at the end of line.
    if(remaining != NULL || *(buffer + characters - 1) == ' ') {

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

    if(errno != 0) {

        serr(ERROR_MSG);

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

    initialize();

    while(read_line()) {

        process_line();
    }

    cleanup();

    return 0;
}
