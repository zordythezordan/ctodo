CC = gcc

# Source files (all .c files in the current directory)
SRCS = $(shell find . -name "*.c")

OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = ctodo

# Libraries to link
LIBS = -lcgui -lxcb-keysyms

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

# Compile each .c file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
