#include "display.h"
#include "platform.h"

CRGB leds[NUM_LEDS];
CRGB framebuffer[WIDTH][HEIGHT];

void initDisplay()
{
    platformInitDisplay();
}

// Maps 2D coordinates to 1D index in a serpentine layout
int getLEDIndex(int x, int y)
{
    if (y % 2 == 0)
    {
        return y * WIDTH + x; // Even rows go left to right
    }
    else
    {
        return y * WIDTH + (WIDTH - 1 - x); // Odd rows go right to left
    }
}

void renderFrameToLEDs()
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            int flippedY = HEIGHT - 1 - y; // Flip vertically
            int flippedX = WIDTH - 1 - x; // Flip horizontally
            int index = getLEDIndex(flippedX, flippedY);
            leds[index] = framebuffer[x][y];
        }
    }
    platformShowDisplay();
}

void setupDisplay()
{
    initDisplay();

    // Optional: start display task
    createThread(
        "DisplayTask",
        [](void *)
        {
            while (true)
            {
                renderFrameToLEDs();
                platformDelay(33); // ~30FPS
            }
        },
        nullptr,
        2048,  // Stack size
        1      // Priority
    );
}

void clearFramebuffer()
{
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            framebuffer[x][y] = CRGB::Black;
        }
    }
}