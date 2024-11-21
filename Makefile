# Compiler and flags
CC = g++
CFLAGS = -Wall -std=c++17 -Iinclude

# Check if we are on Windows
ifeq ($(OS),Windows_NT)
    # If on Windows, set appropriate compiler (MinGW)
    CC = g++.exe
    # Set Windows-specific paths and cleanup commands
    DEL = del /q
    RMDIR = rmdir /s /q
    MKDIR = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
    EXEC = main.exe  # Windows executable file extension
    TEST_EXEC = testExe.exe
    PATH_SEP = \\  # Windows uses backslashes
else
    # For Linux (Raspberry Pi) or other Unix-like systems
    DEL = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p "$(BUILD_DIR)"
    EXEC = main  # Linux executable (no .exe)
    TEST_EXEC = testExe
    PATH_SEP = /  # Linux uses forward slashes
endif

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests

# Output binary name
TARGET = $(EXEC)  # Output file is dependent on platform
TEST_TARGET = $(TEST_EXEC)

# Default target:
all: $(TARGET)

# Source and object files for all target
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRC = $(TEST_DIR)/$(TEST_FILE)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TEST_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(filter-out $(SRC_DIR)/main.cpp, $(SRCS))) $(BUILD_DIR)/$(basename $(TEST_FILE)).o


# Check if the target is 'test'
ifeq ($(filter test,$(MAKECMDGOALS)),test)
    # These checks will only run when the 'test' target is invoked
    TEST_FILE_CHECK:
	# Check if TEST_FILE is set
	@if [ -z "$(TEST_FILE)" ]; then \
		echo "TEST_FILE is not set. Please specify the test file name with TEST_FILE=testfile.cpp"; \
		exit 1; \
	fi

	# Check if the TEST_FILE exists in TEST_DIR
	@if [ ! -f "$(TEST_DIR)/$(TEST_FILE)" ]; then \
		echo "TEST_FILE $(TEST_FILE) does not exist in $(TEST_DIR) directory"; \
		exit 1; \
	fi
endif

# Test target:
test: TEST_FILE_CHECK $(TEST_TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# Linking the test executable
$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $@

# Compiling object files from src directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compiling object files from test directory
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup build:
clean:
ifeq ($(OS),Windows_NT)
	@if exist build $(DEL) build\* 
	@if exist build $(RMDIR) build
	@if exist $(EXEC) $(DEL) $(EXEC)
	@if exist $(TEST_EXEC) $(DEL) $(TEST_EXEC)
else
	@rm -rf $(BUILD_DIR) $(EXEC)
	@if [ -f $(TEST_TARGET) ]; then rm $(TEST_TARGET) > /dev/null 2>&1; fi
endif

.PHONY: all clean
