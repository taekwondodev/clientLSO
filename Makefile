CC = gcc
CFLAGS = -Wall -Wextra -Ih
SRC = main.c c/socket.c
OBJ = $(SRC:.c=.o)

all: main.o

main.o: $(SRC)
	$(CC) $(CFLAGS) $^ -o main.o

clean:
	rm -f $(OBJ) main.o

.PHONY: all clean