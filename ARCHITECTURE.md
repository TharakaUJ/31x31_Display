# 31x31 LED Display Project - Modular Architecture

## Project Structure

```
31x31_Display/
├── common/                    # Shared code across all devices
│   ├── display.cpp/h         # Display abstraction layer
│   ├── displayNumbers.cpp/h  # Number rendering utilities
│   ├── menu.cpp/h            # Menu system
│   └── games/                # Game implementations
│       ├── snake.cpp/h       # Snake game
│       ├── tetris.cpp/h      # Tetris game
│       └── galaxia.cpp/h     # Galaxia game
├── esp32/                    # ESP32-specific implementation
│   ├── esp32.ino            # Main Arduino sketch
│   ├── webserve.cpp/h       # Web server for WiFi control
│   ├── controllerEndpoints.cpp/h  # Input handling
│   ├── htmlFiles.h          # Web UI assets
│   └── Makefile             # Build automation
└── pc/                       # PC simulator (future)
```

## Design Philosophy

The codebase is organized into three main layers:

1. **Common Layer** (`common/`): Contains platform-agnostic game logic and display abstractions
2. **Device Layer** (`esp32/`, `pc/`): Device-specific implementations (WiFi, input, hardware)
3. **Games Layer** (`common/games/`): Individual game implementations

## Building

### ESP32

```bash
cd esp32/
make compile    # Compile the project
make upload     # Upload to ESP32
make monitor    # Open serial monitor
make flash      # Upload and monitor
```

Or use arduino-cli directly:
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 esp32/
arduino-cli upload --fqbn esp32:esp32:esp32 -p /dev/ttyUSB0 esp32/
```

## Adding New Devices

To add support for a new device (e.g., Raspberry Pi):

1. Create a new directory: `mkdir rpi/`
2. Implement device-specific files (input handling, display driver)
3. Create symbolic links to common code:
   ```bash
   cd rpi/
   ln -s ../common/display.* .
   ln -s ../common/menu.* .
   ln -s ../common/displayNumbers.* .
   ln -s ../common/games/*.cpp .
   ln -s ../common/games/*.h .
   ```
4. Create a device-specific Makefile or build configuration

## Code Organization

### Common Code
- **display.h/cpp**: Defines `framebuffer[][]` and display functions
- **menu.h/cpp**: Menu navigation system
- **games/**: Self-contained game modules

### ESP32-Specific
- **controllerEndpoints**: Handles button inputs and command flags
- **webserve**: WiFi access point and web-based remote control
- **htmlFiles**: Embedded web UI

## Dependencies

- **ESP32**: Arduino-ESP32 core, FastLED library
- **Common**: Standard C++ libraries (vector, cstdlib)

## Notes

- Common files are copied to device directories during the build setup process to share code without duplication
- Each device implementation can have its own build system
- Games are self-contained and don't depend on device-specific code
