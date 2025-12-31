# WASM Build for 31x31 LED Display

This directory contains the WebAssembly build configuration for running the 31x31 LED Display in a web browser.

## Prerequisites

- Emscripten SDK (emsdk)
- Make

### Installing Emscripten

```bash
# Clone emsdk
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate latest version
./emsdk install latest
./emsdk activate latest

# Add to your shell profile
source ./emsdk_env.sh
```

## Building

```bash
# Build the WASM module
make

# Clean build artifacts
make clean

# Build with debugging symbols
make debug
```

## Running

After building, you need to serve the files over HTTP (WASM cannot be loaded via file://).

### Option 1: Python HTTP Server
```bash
python3 -m http.server 8000
```

Then open http://localhost:8000/index.html

### Option 2: Using emrun
```bash
emrun --browser chrome dist/index.html
```

### Option 3: Any web server
Serve the `wasm/` directory with your preferred web server (nginx, Apache, etc.)

## Structure

- `main.cpp` - Entry point, exports functions to JavaScript
- `platform.cpp` - Platform abstraction layer (threading, delays, display)
- `controllerEndpoints.cpp/h` - Controller input handling
- `FastLED.h` - Mock FastLED library for WASM compatibility
- `Makefile` - Build configuration with ASYNCIFY support
- `index.html` - Web interface for the display
- `dist/` - Generated WASM and JavaScript files (created during build)

## How It Works

The WASM build uses Emscripten's ASYNCIFY feature to enable cooperative multitasking:

- **Game threads**: Run as regular function calls that yield via `emscripten_sleep()`
- **Display rendering**: JavaScript reads the framebuffer directly via `getFramebufferPointer()`
- **Input handling**: Button clicks and keyboard events call exported handler functions
- **Event loop**: Browser's `requestAnimationFrame` handles the display refresh

This allows the same game logic from ESP32 to run in the browser without modification!

## Controls

- **Arrow Keys** or **Click Buttons**: Navigate
- **Enter** or **SEL**: Select
- **Escape** or **BACK**: Back

## Exported Functions

The following functions are exported for JavaScript:

- `setup()` - Initialize the display and menu system
- `loop()` - Main game loop
- `getFramebufferPointer()` - Get pointer to RGB framebuffer
- `getWidth()` - Get display width (31)
- `getHeight()` - Get display height (31)
- `handleUp/Down/Left/Right()` - Controller inputs
- `handleSelect()` - Select button
- `handleBack()` - Back button

## Development

The WASM build shares the same `common/` codebase as the ESP32 build, ensuring consistent game logic across platforms.

### Adding New Features

1. Add your code to `common/` directory
2. Rebuild with `make`
3. Refresh your browser

### Debugging

Build with debug symbols:
```bash
make debug
```

Then use browser developer tools to inspect WebAssembly execution.
