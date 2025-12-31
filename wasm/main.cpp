#include <emscripten.h>
#include <emscripten/bind.h>
#include "../common/display.h"
#include "../common/menu.h"
#include "controllerEndpoints.h"

// Framebuffer for WASM - matches the common display interface
CRGB framebuffer[WIDTH][HEIGHT];

// Export framebuffer pointer for JavaScript access
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    uint8_t* getFramebufferPointer() {
        return (uint8_t*)framebuffer;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int getWidth() {
        return WIDTH;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int getHeight() {
        return HEIGHT;
    }
    
    EMSCRIPTEN_KEEPALIVE
    void setup() {
        setupDisplay();
        menu_init();
    }
    
    EMSCRIPTEN_KEEPALIVE
    void loop() {
        // Game loop is handled in menu system
        // Just update the display
        renderFrameToLEDs();
    }
    
    // Controller input functions
    EMSCRIPTEN_KEEPALIVE
    void handleUp() {
        up();
    }
    
    EMSCRIPTEN_KEEPALIVE
    void handleDown() {
        down();
    }
    
    EMSCRIPTEN_KEEPALIVE
    void handleLeft() {
        left();
    }
    
    EMSCRIPTEN_KEEPALIVE
    void handleRight() {
        right();
    }
    
    EMSCRIPTEN_KEEPALIVE
    void handleSelect() {
        select();
    }
    
    EMSCRIPTEN_KEEPALIVE
    void handleBack() {
        back();
    }
}

// Emscripten bindings (alternative to extern "C" for more complex types)
using namespace emscripten;

EMSCRIPTEN_BINDINGS(display_module) {
    function("setup", &setup);
    function("loop", &loop);
    function("getFramebufferPointer", &getFramebufferPointer, allow_raw_pointers());
    function("getWidth", &getWidth);
    function("getHeight", &getHeight);
    function("handleUp", &handleUp);
    function("handleDown", &handleDown);
    function("handleLeft", &handleLeft);
    function("handleRight", &handleRight);
    function("handleSelect", &handleSelect);
    function("handleBack", &handleBack);
}
