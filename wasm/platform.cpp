#include "../common/platform.h"
#include <emscripten.h>
#include <exception>

// Exception type to signal thread exit
struct ThreadExit : public std::exception {
    const char* what() const noexcept override {
        return "Thread exit requested";
    }
};

void createThread(const char* name, ThreadFunction function, void* params, 
                  int stackSize, int priority)
{
    // In WASM, we directly call the function since we're single-threaded
    // The function will run cooperatively with the JS event loop via platformDelay
    try {
        function(params);
    } catch (const ThreadExit&) {
        // Thread requested exit, this is normal
    }
}

void platformDelay(int milliseconds)
{
    // Use emscripten_sleep to yield to the browser event loop
    // This allows the browser to process events and render
    emscripten_sleep(milliseconds);
}

void platformDeleteCurrentThread()
{
    // Throw exception to unwind and exit the current "thread"
    throw ThreadExit();
}

void platformInitDisplay()
{
    // No hardware initialization needed for WASM
    // The framebuffer is already accessible to JavaScript
}

void platformShowDisplay()
{
    // No-op for WASM - JavaScript reads the framebuffer directly via getFramebufferPointer
    // The render loop in index.html handles displaying the framebuffer
}
