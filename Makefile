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
INCLUDES          =  -Iinclude
SRCS              =  $(wildcard src/*.c)
OBJS              := $(SRCS:.c=.o)

# Compiler settings
CC                = gcc
CSTD              = c2x
CFLAGS            = -std=$(CSTD) -Wall -Werror -g -v ${INCLUDES}
LDFLAGS           = -lm

STACK_MEMORY_SIZE = 1024
CFLAGS            += -DSTACK_MEMORY_SIZE=$(STACK_MEMORY_SIZE)


# Ensure the build directory exists
$(shell mkdir -p build)
# Define the target executable
TARGET          = build/$(PROJECT)

# PHONY commands
.PHONY: all clean

# Default target
all: $(TARGET)

# Build rules
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf build/ $(OBJS)
