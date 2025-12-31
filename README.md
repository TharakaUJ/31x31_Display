# 🐍 ESP32 LED Snake Game

A modular Snake game running on an ESP32 with a 31×31 LED matrix. This project supports multiple games, a display simulator via Wi-Fi, and modular input/output separation. **Now also available as WebAssembly!** 🌐

---

## 🚀 Features

- **31×31 RGB LED grid display** (WS2812-style).
- **Snake game** with food pickup and growing behavior.
- **Game-over and restart logic**.
- **HTTP simulator** to view the LED grid on a browser.
- **Control via external endpoints** (UP, DOWN, LEFT, RIGHT, SELECT, BACK).
- **Modular architecture** for supporting additional games and menus.
- **WebAssembly build** - Run the display in any modern web browser! See [wasm/README.md](wasm/README.md) for details.

---

## 🔧 Hardware Setup

- **ESP32 (WROOM-32)**.
- **31×31 RGB LED matrix** (WS2812 or similar) — connected in a single data stream.
- **Connections**:
    - `VCC` – Power
    - `GND` – Ground
    - `DATA` – Data line (connected to a GPIO pin like G4)

---

## 📦 Libraries Used

- **FastLED** – to control the LED matrix.
- **WiFi and WebServer** – for HTTP control and display simulation.
- **freertos/FreeRTOS.h** – for task scheduling.
- **C++ STL** – `vector`, `string`, etc.

---

## 📁 Project Structure

```plaintext
common/              # Shared game logic across platforms
├── display.cpp/h         # LED framebuffer and update logic
├── displayNumbers.cpp/h  # Number rendering utilities
├── menu.cpp/h            # Menu system
└── games/               # Game implementations
    ├── snake.cpp/h
    ├── tetris.cpp/h
    └── galaxia.cpp/h
esp32/               # ESP32-specific implementation
├── esp32.ino
├── controllerEndpoints.cpp/h
├── webserve.cpp/h
└── htmlFiles.h
wasm/                # WebAssembly build (play in browser!)
├── main.cpp
├── FastLED.h        # Mock FastLED for WASM
├── Makefile
└── index.html
```

See [ARCHITECTURE.md](ARCHITECTURE.md) for detailed architecture information.

---

## 🌐 WebAssembly Build

**Play in your browser!** The entire display and game logic has been ported to WebAssembly.

- **Live demo**: Available via GitHub Pages (see [wasm/README.md](wasm/README.md))
- **Local build**: `cd wasm && make`
- **Features**: Full game support with keyboard/button controls

For detailed instructions on building and deploying, see **[wasm/README.md](wasm/README.md)**.

---

## 🌐 Web-Based Simulator (ESP32)

### HTTP Endpoint

Connect to the ESP32 via browser:

```bash
GET /simulate
```

- Returns a live grid display (HTML with colored divs).
- Useful for debugging or if the physical grid is unavailable.

### Setup

- ESP32 creates an AP:
    - **SSID**: `ESP32_AP`
    - **IP**: `192.168.4.1`
- Navigate to: `http://192.168.4.1/simulate`

---

## 🎮 Controls

The controller sets command flags that the game logic polls:

- `commandFlags[CMD_UP]`
- `commandFlags[CMD_DOWN]`
- `commandFlags[CMD_LEFT]`
- `commandFlags[CMD_RIGHT]`
- `commandFlags[CMD_SELECT]`
- `commandFlags[CMD_BACK]`

Each flag is a `bool`, reset after being read.

---

## 🧠 Game Logic (Snake)

- **Snake Representation**: A list of points (`std::vector<Point>`).
- **Game Tick**: Runs in a loop every 200ms.
- **Direction**: Updated from command flags.
- **Growth**: Snake grows when it eats food.
- **Game Over**:
    - Triggered when colliding with a wall or itself.
    - Game over screen shown (optionally in red).
- **Restart**:
    - Press `SELECT` to restart.
    - Press `BACK` to return to the menu.

---

## 🛠️ To Do

- ~~Add menu system for selecting multiple games.~~
- ~~Implement score display using the number renderer.~~
- Add more games (e.g., Pong, Space Invaders).
- Improve simulator with live updates or WebSocket.

---
