/**
 * Marathon task implementation.
 * Author: Mateusz Gienieczko
 * Copyright (C) 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "tree.h"
#include "marathon_tree.h"

size_t bufferSize;
char *buffer;

void initialize() {

    bufferSize = INITIAL_BUFFER_SIZE;
    buffer = malloc(bufferSize * sizeof(char));

    marathon_tree_initialize();
}

bool process_add_user(int parentID, int userID) {

    return parentID >= 0 && userID >= 0 && marathon_tree_add(parentID, userID);
}

bool process_del_user(int userID) {

    return userID > 0 && marathon_tree_remove(userID);
}

bool process_add_movie(int userID, int movieRating) {

    return userID >= 0 && movieRating >= 0 &&
           marathon_tree_add_movie(userID, movieRating);
}

bool process_del_movie(int userID, int movieRating) {

    return userID >= 0 && movieRating >= 0 &&
           marathon_tree_remove_movie(userID, movieRating);
}

bool process_marathon(int userID, int k) {

    if(userID < 0 || k < 0) {
        return false;
    }

    dlist_t *marathonResult = marathon_tree_get_marathon_list(userID, k);

    if(marathonResult != NULL) {
        dlist_print_num(marathonResult);
    } else {
        return false;
    }

    dlist_destroy(&marathonResult);

    return true;
}

void process_line(ssize_t characters) {

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
    ssize_t readLength = (command == NULL ? 0 : strlen(command) + 1) +
                         (arg1str == NULL ? 0 : strlen(arg1str) + 1) +
                         (arg2str == NULL ? 0 : strlen(arg2str) + 1);

    // If remaining is not null, we have junk at the end of the line.
    // If readLength != characters, there were multiple whitespaces
    if(remaining != NULL || readLength != characters) {

        serr(ERROR_MSG);

        return;
    }

    int arg1 = -1, arg2 = -1;

    if(arg1str != NULL && *arg1str >= '0' && *arg1str <= '9') {
        arg1 = (int) strtol(arg1str, NULL, 10);
    }

    if(arg2str != NULL && *arg2str >= '0' && *arg2str <= '9') {
        arg2 = (int) strtol(arg2str, NULL, 10);
    }

    if(errno != 0) {

        serr(ERROR_MSG);

        return;
    }

    bool errorFlag = true;

    if(strcmp(command, CTRL_STR_ADDUSER) == 0) {

        errorFlag = !process_add_user(arg1, arg2);
    } else if(strcmp(command, CTRL_STR_DELUSER) == 0) {

        errorFlag = arg2str != NULL || !process_del_user(arg1);
    } else if(strcmp(command, CTRL_STR_ADDMOVIE) == 0) {

        errorFlag = !process_add_movie(arg1, arg2);
    } else if(strcmp(command, CTRL_STR_DELMOVIE) == 0) {

        errorFlag = !process_del_movie(arg1, arg2);
    } else if(strcmp(command, CTRL_STR_MARATHON) == 0) {

        errorFlag = !process_marathon(arg1, arg2);

        if(!errorFlag) {
            return;
        }
    }

    if(errorFlag) {
        serr(ERROR_MSG);
    } else {
        printf(OK_MSG);
    }
}

void cleanup() {

    free(buffer);

    marathon_tree_cleanup();
}

int main(void) {

    initialize();

    ssize_t characters = 0;

    while((characters = getline(&buffer, &bufferSize, stdin)) > 0) {

        process_line(characters);
    }

    cleanup();

    return 0;
}
