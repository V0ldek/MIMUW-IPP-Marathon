
# Executable name
PROG=main

# C compiler
CC=gcc

# C compiler flags
CFLAGS=-Wall -Wextra -g -O2

# Linker flags
LDFLAGS=-Wall -Wextra

# Valgrind flags
VALGRINDFLAGS=--leak-check=full --show-leak-kinds=all

# Release/debug
DEBUG ?= 0
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

$(PROG): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@
	  
%.c: %.o
	$(CC) $^ $(CFLAGS) -c
    
clean:
	rm -f -r $(OBJS) $(PROG) *.gch .depend
    
include .depend

.depend:
	$(CC) -MM $(SRCS)>.depend

