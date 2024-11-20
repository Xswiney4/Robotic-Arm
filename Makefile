# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude

# Check if we are on Windows
ifeq ($(OS),Windows_NT)
    # If on Windows, set appropriate compiler (MinGW)
    CC = gcc.exe
    # Set Windows-specific paths
    DEL = del /q
    RMDIR = rmdir /s /q
    MKDIR = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
    EXEC = main.exe  # Windows executable file extension
else
    # For Linux (Raspberry Pi) or other Unix-like systems
    DEL = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
    EXEC = main  # Linux executable (no .exe)
endif

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Output binary name
TARGET = $(EXEC)  # Output file is dependent on platform

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# Compiling object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning build artifacts
clean:
	@if exist build $(DEL) build\* 
	@if exist build $(RMDIR) build
	@if exist $(EXEC) $(DEL) $(EXEC)

.PHONY: all clean
