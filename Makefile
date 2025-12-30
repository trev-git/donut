CC=gcc
CFLAGS=-Wall -Wextra -O2 -ffast-math -march=native
LDFLAGS=-lraylib -lm
NAME=main
OBJS=main.o

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)

.PHONY: all clean
