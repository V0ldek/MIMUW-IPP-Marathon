
# Executable name
PROG=main

# C compiler
CC=gcc

# C compiler flags
CFLAGS=-Wall -Wextra -O2

# Linker flags
LDFLAGS=-Wall -Wextra

SRCDIR=src

# Source files
SRCS=$(SRCDIR)/list.c $(SRCDIR)/tree.c $(SRCDIR)/movie_list.c \
$(SRCDIR)/marathon_tree.c $(SRCDIR)/main.c

# Required objects
OBJS=$(SRCS:.c=.o)

# Creates the $(PROG) executable
all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@
	    
%.c: %.o
	$(CC) $^ $(CFLAGS) -c
    
clean:
	rm -f -r $(OBJS) $(PROG) *.gch .depend
    
include .depend

.depend:
	$(CC) -MM $(SRCS)>.depend

