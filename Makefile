TARGET = client
CC = gcc
CFLAGS = -Wall -Wextra -I./h
SRC = main.c c/socket.c c/hash.c c/client.c
LIBS = -lssl -lcrypto -lcjson
OBJ = $(SRC:.c=.o)

all: $(TARGET)

# Regola per generare l'eseguibile
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(TARGET)

# Regola per compilare i file oggetto
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia dei file generati
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
