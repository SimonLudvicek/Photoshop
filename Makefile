CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/photoshop.c
OBJ_FILES = $(BUILD_DIR)/main.o $(BUILD_DIR)/photoshop.o
TARGET = $(BIN_DIR)/photoshop

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean