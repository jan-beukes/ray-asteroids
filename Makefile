# $@ - The target 
# $< - First dependancy
# $^ - all dependancy

CC = gcc
CFLAGS = -Wall -Wextra
LFLAGS = -lraylib -lm

# Directories
SRC = src
OBJ = obj

SRCS = $(wildcard $(SRC)/*.c) # wildcard function read all mathing the expression
OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS)) # $(patsubst <pattern>, <replacement>, <text>)

all: game

game: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

# Compiling .c files in src/ into .o files in obj/
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: game
	./game
	
clean:
	rm $(OBJ)/*.o game
