#include <emscripten.h>
#include "../common/display.h"
#include "../common/displayNumbers.h"
#include "../common/menu.h"
#include "controllerEndpoints.h"

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
    void menu() {
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

    EMSCRIPTEN_KEEPALIVE
    void displayThreeDigitNumber(int value) {
        drawCenteredThreeDigitNumber(value, CRGB::Blue);
    }


}
