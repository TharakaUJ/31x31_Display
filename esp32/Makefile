# Makefile for ESP32 31x31 Display Project
# Uses arduino-cli for building and uploading

# Configuration
FQBN = esp32:esp32:esp32
SKETCH_DIR = .
PORT ?= /dev/ttyUSB0
BAUD_RATE = 115200
COMMON_DIR = ../common

# Build flags to include common directory
BUILD_FLAGS = --build-property "compiler.cpp.extra_flags=-I$(PWD)/$(COMMON_DIR) -I$(PWD)/$(COMMON_DIR)/games"

# Targets
.PHONY: all compile upload monitor clean help

all: compile

compile:
	@echo "Compiling ESP32 sketch..."
	arduino-cli compile --fqbn $(FQBN) $(BUILD_FLAGS) $(SKETCH_DIR)

upload: compile
	@echo "Uploading to ESP32..."
	arduino-cli upload --fqbn $(FQBN) -p $(PORT) $(SKETCH_DIR)

setup:
	@echo "Setting up ESP32 directory..."
	@echo "Copying common files..."
	@cp -f ../common/*.cpp ../common/*.h .
	@cp -f ../common/games/*.cpp ../common/games/*.h .
	@echo "Setup complete!"

clean-links:
	@echo "Removing any symlinks..."
	@find . -maxdepth 1 -type l -delete
	@echo "Symlinks removed"

monitor:
	@echo "Opening serial monitor..."
	arduino-cli monitor -p $(PORT) -b $(BAUD_RATE)

flash: upload monitor

clean:
	@echo "Cleaning build artifacts..."
	arduino-cli cache clean
	@echo "Clean complete"

help:
	@echo "ESP32 31x31 Display - Available targets:"
	@echo "  make compile     - Compile the sketch"
	@echo "  make upload      - Compile and upload to ESP32"
	@echo "  make monitor     - Open serial monitor"
	@echo "  make flash       - Upload and open monitor"
	@echo "  make setup       - Copy common files to esp32/"
	@echo "  make clean       - Clean build cache"
	@echo "  make clean-links - Remove symlinks (if any)"
	@echo ""
	@echo "Variables:"
	@echo "  PORT         - Serial port (default: /dev/ttyUSB0)"
	@echo "  BAUD_RATE    - Serial baud rate (default: 115200)"
	@echo ""
	@echo "Note: Common files are copied from ../common/ during setup"
	@echo "      Changes to common files require running 'make setup' again"
