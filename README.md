# 🐍 ESP32 LED Snake Game

A modular Snake game running on an ESP32 with a 31×31 LED matrix. This project supports multiple games, a display simulator via Wi-Fi, and modular input/output separation.

---

## 🚀 Features

- **31×31 RGB LED grid display** (WS2812-style).
- **Snake game** with food pickup and growing behavior.
- **Game-over and restart logic**.
- **HTTP simulator** to view the LED grid on a browser.
- **Control via external endpoints** (UP, DOWN, LEFT, RIGHT, SELECT, BACK).
- **Modular architecture** for supporting additional games and menus.

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
src/
├── display.h / .cpp         # LED framebuffer and update logic
├── controllerEndpoints.h    # Memory-mapped control input flags
├── snakeGame.cpp            # Main game logic
├── displayNumbers.cpp        # 2-digit number renderer
```

---

## 🌐 Web-Based Simulator

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

- Add menu system for selecting multiple games.
- Implement score display using the number renderer.
- Add more games (e.g., Tetris, Pong).
- Improve simulator with live updates or WebSocket.

---
