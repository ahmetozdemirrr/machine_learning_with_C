# Compiler settings
CC = gcc # compiler
CFLAGS = -Wall -Wextra -O2 -Iinclude/ -fPIC
LDFLAGS = -lm

# File directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
EXAMPLES_DIR = examples

# Find the source files automatically
SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
LIBRARY = $(BIN_DIR)/libmlc.a

# Find the sample files automatically
EXAMPLES = $(wildcard $(EXAMPLES_DIR)/*.c)


# Default target
all: prepare $(LIBRARY)


# Create directories
prepare:
	@mkdir -p $(BUILD_DIR)/activations
	@mkdir -p $(BIN_DIR)

# Compile the library
$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

# Inspect object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Compile the examples
examples: $(LIBRARY) $(EXAMPLES:$(EXAMPLES_DIR)/%.c=$(BIN_DIR)/%)

# Rule for the each example
$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@ -L$(BIN_DIR) -lmlc $(LDFLAGS)


# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


.PHONY: all prepare examples clean