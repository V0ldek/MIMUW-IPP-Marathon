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

#define CTRL_STR_ADDUSER "addUser"
#define CTRL_STR_DELUSER "delUser"
#define CTRL_STR_ADDMOVIE "addMovie"
#define CTRL_STR_DELMOVIE "delMovie"
#define CTRL_STR_MARATHON "marathon"
#define ERROR_MSG "ERROR\n"
#define OK_MSG "OK\n"
#define EMPTY_LIST_MSG "NONE\n"
#define INITIAL_BUFFER_SIZE 32

size_t bufferSize;
char *buffer;
tree *root;

void print_list(dlist_t *list) {

    NNULL(list, "print_list");

    dnode_t *iter = dlist_get_front(list);

    if(iter == NULL) {

        printf(EMPTY_LIST_MSG);

        return;
    }

    while(dlist_next(iter) != NULL) {

        printf("%d ", iter->elem->num);

        iter = dlist_next(iter);
    }

    printf("%d\n", iter->elem->num);
}

void initialize() {

    bufferSize = INITIAL_BUFFER_SIZE;
    buffer = malloc(bufferSize * sizeof(char));

    root = marathon_tree_initialize();
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
        print_list(marathonResult);
    } else {
        return false;
    }

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

    serr("Read %s with arg1 = %d", command, arg1);

    if(arg2str != NULL) {
        serr(" and arg2 = %d", arg2);
    }

    serr(".\n");

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

    marathon_tree_destroy(&root);
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
