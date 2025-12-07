# ESP32 Implementation

## Setup

The ESP32 directory contains device-specific code and copies of common files.

### First Time Setup
```bash
make setup    # Copy common files from ../common/
```

### After Modifying Common Files
If you edit files in `../common/`, run:
```bash
make setup    # Re-copy updated common files
```

## Building & Uploading

```bash
make compile              # Compile only
make upload               # Compile and upload
make monitor              # Open serial monitor
make flash                # Upload and monitor
PORT=/dev/ttyUSB1 make upload  # Use different port
```

## File Organization

### Device-Specific Files (edit these)
- `esp32.ino` - Main sketch
- `webserve.cpp/h` - WiFi web server
- `controllerEndpoints.cpp/h` - Input handling
- `htmlFiles.h` - Web UI
- `Makefile` - Build configuration

### Copied from Common (edit in ../common/)
- `display.cpp/h`
- `displayNumbers.cpp/h`
- `menu.cpp/h`
- `snake.cpp/h`
- `tetris.cpp/h`
- `galaxia.cpp/h`

**Important**: Edit game and display logic in `../common/`, not in this directory!

## Cleaning Up

```bash
make clean         # Clean build cache
make clean-links   # Remove any symlinks (if you used them before)
```

## How It Works

The Makefile adds `-I../common` and `-I../common/games` to the compiler flags, allowing the code to reference common files. Common files are copied to this directory so Arduino CLI can find them during compilation.
