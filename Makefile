# Makefile for LCD Project on DE10 Standard

CC = gcc
CFLAGS = -Wall -O2 -std=c99 -I./includes -I./dependencies -I./dependencies/hwlib_stub
LDFLAGS = -lrt

# Directories
SRC_DIR = src
DEP_DIR = dependencies
OBJ_DIR = obj
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/simon_says

# Source files
SRCS = $(SRC_DIR)/main.c \
	   $(SRC_DIR)/game_logic.c \
       $(SRC_DIR)/hal/hal-api.c \
       $(SRC_DIR)/peripherals/lcd.c \
       $(DEP_DIR)/LCD_Lib.c \
       $(DEP_DIR)/LCD_Driver.c \
       $(DEP_DIR)/LCD_Hw.c \
       $(DEP_DIR)/lcd_graphic.c \
       $(DEP_DIR)/font.c \
       $(DEP_DIR)/terasic_lib.c

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter $(SRC_DIR)/%,$(SRCS))) \
       $(patsubst $(DEP_DIR)/%.c,$(OBJ_DIR)/dep_%.o,$(filter $(DEP_DIR)/%,$(SRCS)))

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/peripherals
	@mkdir -p $(BIN_DIR)

# Link object files to create executable
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

# Compile source files from src directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile dependency files
$(OBJ_DIR)/dep_%.o: $(DEP_DIR)/%.c
	@echo "Compiling dependency $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Run the program
run: $(TARGET)
	@echo "Running LCD demo..."
	sudo ./$(TARGET)

# Phony targets
.PHONY: all clean run directories
