CC = clang 
CFLAGS = -Wall -Wextra -Wpedantic -std=c18 -Iinclude
EXE = Game

BUILD_DIR = build
SRC_DIR = src
LDLIBS = -lSDL3

OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES: $(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/$(EXE)

all: $(TARGET)

$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

run:
	$(TARGET)

format:
	clang-format -i $(SRC_FILES) $(widcard include/*.h)

.PHONY: all clean run format

