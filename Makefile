# C Makefile for the Marathon assignment
# Use with DEBUG=0/1 for release/debug versions
#
# Author: Mateusz Gienieczko
# Copyright (C) 2018

# Release/debug
DEBUG?=0

# Executable name
PROG=main

# C compiler
CC=gcc

# C compiler flags
CFLAGS=-Wall -Wextra -g -O2 -std=c11

# Linker flags
LDFLAGS=

# Valgrind flags
VALGRINDFLAGS=--leak-check=full --show-leak-kinds=all

# If not debug version, add appropriate flag
ifeq ($(DEBUG), 0)
	CFLAGS+=-DNDEBUG
endif
	
# Sources directory
SRCDIR=src

# Source files
SRCS=$(SRCDIR)/dlist.c $(SRCDIR)/tree.c \
$(SRCDIR)/marathon_tree.c $(SRCDIR)/main.c

# Required objects
OBJS=$(SRCS:.c=.o)

# Creates the $(PROG) executable (release version)
all: $(PROG)

# Runs the $(PROG) executable with valgrind
run: $(PROG)
	valgrind $(VALGRINDFLAGS) ./$(PROG)

# Link all objects into the executable
$(PROG): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@
	  
%.c: %.o
	$(CC) $^ $(CFLAGS) -c
    
clean:
	rm -f -r $(OBJS) $(PROG) *.gch .depend
    
include .depend

.depend:
	$(CC) -MM $(SRCS)>.depend

