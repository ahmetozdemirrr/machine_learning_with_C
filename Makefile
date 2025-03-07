# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude/
LDFLAGS = -lm

# File directories
EXAMPLES_DIR = examples
BIN_DIR = bin

# Find the example files automatically
EXAMPLES = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_BINS = $(EXAMPLES:$(EXAMPLES_DIR)/%.c=$(BIN_DIR)/%)

# Default target
all: prepare examples


# Create directories
prepare:
	@mkdir -p $(BIN_DIR)


# Compile the examples
examples: $(EXAMPLE_BINS)

# Rule for each example
$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)


# Clean
clean:
	rm -rf $(BIN_DIR)


.PHONY: all prepare examples clean
