# WASM Build Configuration
EMCC = emcc
CXX = em++

# Directories
COMMON_DIR = ../common
GAMES_DIR = $(COMMON_DIR)/games
BUILD_DIR = build
DIST_DIR = dist

# Compiler flags
CXXFLAGS = -std=c++17 -O3 -Wall
EMSCRIPTEN_FLAGS = -s WASM=1 \
                   -s ALLOW_MEMORY_GROWTH=1 \
                   -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","HEAPU8"]' \
                   -s MODULARIZE=1 \
                   -s EXPORT_NAME="DisplayModule" \
                   -s ENVIRONMENT=web \
                   -s ASYNCIFY=1 \
                   -s ASYNCIFY_STACK_SIZE=65536

# Include paths
INCLUDES = -I. -I$(COMMON_DIR) -I$(GAMES_DIR)

# Source files
COMMON_SOURCES = $(COMMON_DIR)/display.cpp \
                 $(COMMON_DIR)/displayNumbers.cpp \
                 $(COMMON_DIR)/menu.cpp

GAME_SOURCES = $(GAMES_DIR)/snake.cpp \
               $(GAMES_DIR)/tetris.cpp \
               $(GAMES_DIR)/galaxia.cpp

WASM_SOURCES = main.cpp \
               controllerEndpoints.cpp \
               platform.cpp \
               FastLED.cpp

ALL_SOURCES = $(COMMON_SOURCES) $(GAME_SOURCES) $(WASM_SOURCES)

# Output files
OUTPUT = $(DIST_DIR)/display.js

.PHONY: all clean setup

all: setup $(OUTPUT)

setup:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(DIST_DIR)

$(OUTPUT): $(ALL_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(EMSCRIPTEN_FLAGS) \
		$(ALL_SOURCES) \
		-o $(OUTPUT)
	@echo "WASM build complete! Output: $(OUTPUT)"
	@echo "Files generated:"
	@ls -lh $(DIST_DIR)

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)

# Development build with debugging
debug: CXXFLAGS += -g -O0 -s ASSERTIONS=1 -s SAFE_HEAP=1
debug: EMSCRIPTEN_FLAGS += -s DEMANGLE_SUPPORT=1
debug: $(OUTPUT)

# Test the build
test: all
	@echo "To test, serve the dist/ directory with a web server:"
	@echo "  python3 -m http.server 8000"
	@echo "Or use emrun:"
	@echo "  emrun --browser chrome $(DIST_DIR)/index.html"

.DEFAULT_GOAL := all
