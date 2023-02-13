# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic

# Target executable
TARGET = p3

# Source files
SRCS = server.c

# Object files
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

# Default target
all: $(TARGET)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
