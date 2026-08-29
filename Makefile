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
	rm -rf $(TARGET) $(TEST_TARGET)

TEST_SRC = $(filter-out src/main.c, $(wildcard src/*.c)) tests/main_test.c
TEST_TARGET = test_chani

test: $(TEST_SRC)
	$(CC) $(CFLAGS) -g $^ -o $(TEST_TARGET)
	./$(TEST_TARGET)
