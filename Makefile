CC = gcc
CFLAGS = -O3 -Wall -Wextra -std=c11 -fopenmp
TARGET = bin/matrix

.PHONY: all clean

all: $(TARGET)

$(TARGET): src/matrix.c
	mkdir -p bin
	$(CC) $(CFLAGS) src/matrix.c -o $(TARGET)

clean:
	rm -rf bin
