####################################################################################################
# 
# @file: Makefile
# @author: Sagarrajvarman Ladla
# @date: 2025-08-03
# @brief: This file contains the build instructions for the StackVM project
# @version: 1.0
# @license: MIT License
# @note: This project is developed using the C23 language standard version.
#
####################################################################################################

PROJECT = stackvm

# Source files
INCLUDES          =  -Iinc
SRC_DIR           =  src
SRCS              :=  $(wildcard $(SRC_DIR)/*.c)
BUILD_DIR         =  build
RELEASE_DIR      =  release
OBJS              := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Compiler settings
CC                = gcc
CSTD              = c2x
CFLAGS            = -std=$(CSTD) -Wall -Werror -g -v ${INCLUDES}
LDFLAGS           = -lm

STACK_MEMORY_SIZE = 1024
CFLAGS            += -DSTACK_MEMORY_SIZE=$(STACK_MEMORY_SIZE)

# Define the target executable
TARGET            = $(BUILD_DIR)/$(RELEASE_DIR)/$(PROJECT)

default: all

# Default target
all: $(BUILD_DIR) $(TARGET)

# Build rules
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the build directory exists
$(BUILD_DIR):
	$(shell mkdir -p $(BUILD_DIR)/$(RELEASE_DIR))

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# PHONY commands
.PHONY: all clean

parser:
# 	@echo "Parsing source files..."
# 	@for file in $(SRCS); do \
# 		echo "Parsing $$file..."; \
# 		$(CC) $(CFLAGS) -E $$file; \
# 	done
	$(CC) -DSTACK_MEMORY_SIZE=$(STACK_MEMORY_SIZE) -Iinclude src/parser.c -o build/parser