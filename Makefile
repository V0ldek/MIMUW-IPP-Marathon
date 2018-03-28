
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
SRCS=$(SRCDIR)/dlist.c $(SRCDIR)/tree.c $(SRCDIR)/movie_list.c \
$(SRCDIR)/marathon_tree.c $(SRCDIR)/main.c

# Required objects
OBJS=$(SRCS:.c=.o)

# Creates the $(PROG) executable (release version)
all: $(PROG) -D NDEBUG

# Creates the $(PROG) executable with debug info
debug: $(PROG)

# Runs the $(PROG) executable with valgrind
run: valgrind ./$(PROG)

$(PROG): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@
	    
%.c: %.o
	$(CC) $^ $(CFLAGS) -c
    
clean:
	rm -f -r $(OBJS) $(PROG) *.gch .depend
    
include .depend

.depend:
	$(CC) -MM $(SRCS)>.depend

