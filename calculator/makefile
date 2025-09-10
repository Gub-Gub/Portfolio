.PHONY: all clean tests check

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Werror
LDLIBS  = -lm

BIN     = calc
OBJS    = main.o ops.o parse.o combi.o

# ---- Tests config ----
TESTSRC = test.c        
TESTBIN = tests
TESTOBJ = $(TESTSRC:.c=.o)
TESTOBJS = $(TESTOBJ) ops.o parse.o combi.o

# Default build
all: $(BIN)

# Main program
$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(LDLIBS)

# Tests program (no indirection to avoid circular dep)
tests: $(TESTOBJS)
	$(CC) $(TESTOBJS) -o $(TESTBIN) $(CFLAGS) $(LDLIBS)

# Optional: build + run tests in one go
check: tests
	./$(TESTBIN)

# Generic .c → .o rule
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Cleanup
clean:
	rm -f $(BIN) $(TESTBIN) *.o