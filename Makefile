# Makefile for Game Menu Project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Debug flags (uncomment to enable DEBUG mode)
# CFLAGS += -DDEBUG

# Target executable
TARGET = game_menu

# Source files
SOURCES = main.c menu.c games.c utilities.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += -DDEBUG -g
debug: clean $(TARGET)

# Clean build files
clean:
	del /Q $(TARGET).exe $(OBJECTS) 2>nul || echo Clean complete

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all debug clean run
