# Executable name
BIN = med

# Compiler and compiler flags
CC = gcc
IDIRS = `pkg-config --cflags sdl2` -I/usr/include/SDL2 -I./include # sdl, ttf, ./include
LDIRS = -L/usr/lib/x86_64-linux-gnu # ttf
LIBS = `pkg-config --libs sdl2` $(LDIRS) -lSDL2_ttf

CFLAGS = -Wall -Wextra -std=c11 $(IDIRS)

# Source files and object files
SRC = main.c utils.c font.c render.c vec.c line.c editor.c
OBJ = $(SRC:.c=.o)

# The search path for all files not found in the current directory
VPATH = ./src:./include

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LIBS)

# %< is a magic variable and regers to the first dep
main.o: main.c utils.h font.h vec.h editor.h render.h
	$(CC) $(CFLAGS) -c $<

utils.o: utils.c utils.h font.h editor.h
	$(CC) $(CFLAGS) -c $<

vec.o: vec.c vec.h
	$(CC) $(CFLAGS) -c $<

font.o: font.c font.h utils.h
	$(CC) $(CFLAGS) -c $<

render.o: render.c render.h editor.h utils.h font.h vec.h
	$(CC) $(CFLAGS) -c $<

line.o: line.c line.h utils.h
	$(CC) $(CFLAGS) -c $<

editor.o: editor.c editor.h line.h utils.h
	$(CC) $(CFLAGS) -c $<


# add $(OBJ) to rm after you know id doesn't delete everything
# $(addprefix ./src/, $(OBJ))
clean:
	rm -f $(BIN) $(OBJ)

run: $(BIN)
	./$(BIN)

# Tells make to execute the recipe, not look for the rule with the filename of the task
.PHONY: all clean run
