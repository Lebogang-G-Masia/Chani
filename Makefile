CC = gcc
CFLAGS = -Wall -Wextra
SRC = $(wildcard src/*.c)
TARGET = chani

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

debug: $(SRC)
	$(CC) $(CFLAGS) -g $^ -o $(TARGET)

clean:
	rm -rf $(TARGET)

