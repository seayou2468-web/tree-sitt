# Makefile for local verification (simulating iOS build steps)

CC ?= clang
AR ?= ar
CFLAGS += -Isrc -Iinclude -fPIC

SRC = src/parser.c src/scanner.c src/diagnostics.c
OBJ = $(SRC:.c=.o)
LIB = libtree-sitter-swift.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)

.PHONY: all clean
