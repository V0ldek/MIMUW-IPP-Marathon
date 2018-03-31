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
