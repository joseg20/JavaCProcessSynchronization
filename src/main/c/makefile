CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -pthread

BIN = bin

OBJS = $(BIN)/main.o

$(BIN)/%.o: %.c
	mkdir -p $(BIN)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(BIN)/main

clean:
	rm -rf $(BIN)

run: all
	$(BIN)/main
