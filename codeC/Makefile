CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = main.o avl.o io.o
TARGET = main

all: directories $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p output tmp

clean:
	rm -f $(OBJ) $(TARGET)
	rm -rf tmp output
