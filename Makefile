# $@ - The target 
# $< - First dependancy
# $^ - all dependancy

CC = gcc
CFLAGS = -Wall -Wextra
LFLAGS = -l:libraylib.a -lm -lpthread -ldl

# MAGIC
EMFLAGS = --preload-file resources -Os -Wall lib/librayweb.a -I/usr/local/include/ -s USE_GLFW=3
EMFLAGS += -s ASYNCIFY # Fix main loop thing
EMFLAGS += --shell-file ~/Projects/c-libs/raylib/src/minshell.html -DPLATFORM_WEB

# Directories
SRC = src
OBJ = obj

SRCS = $(wildcard $(SRC)/*.c) # wildcard function read all mathing the expression
OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS)) # $(patsubst <pattern>, <replacement>, <text>)

all: game

web: $(SRCS)
	emcc -o game.html $^ $(EMFLAGS)
	rm game.html

game: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

# Compiling .c files in src/ into .o files in obj/
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

docker: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o output/game -L/raylib/src -lraylib -lm -lpthread -ldl

run: game
	./game
	
clean build:
	if test -f game; then \
  		rm game; \
	fi
	rm $(OBJS)
	make all
