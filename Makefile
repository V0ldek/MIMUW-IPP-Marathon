
# Executable name
PROG=marathon

# C compiler
CC=gcc

# C compiler flags
CFLAGS=-Wall -Wextra -O2

# Linker flags
LDFLAGS=-Wall -Wextra

SRCDIR=src

# Header files
HDRS=

# Source files
SRCS=$(HDRS:.h=.c) $(SRCDIR)/main.c

# Required objects
OBJS=$(SRCS:.c=.o)

# Creates the $(PROG) executable
all: $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(PROG)
	    
%.c: %.o
	$(CC) $^ $(CFLAGS) -c
    
clean:
	rm -f -r $(OBJS) $(PROG) *.gch .depend
    
include .depend

.depend:
	$(CC) -MM $(SRCS)>.depend

