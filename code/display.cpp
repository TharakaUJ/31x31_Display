#include "display.h"

CRGB leds[NUM_LEDS];
CRGB framebuffer[WIDTH][HEIGHT];

void initDisplay()
{
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(50); // Scale as needed to prevent power issues
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
            int index = getLEDIndex(x, y);
            leds[index] = framebuffer[x][y];
        }
    }
    FastLED.show();
}

void setupDisplay()
{
    initDisplay();

    // Optional: start display task
    xTaskCreatePinnedToCore(
        [](void *)
        {
            while (true)
            {
                renderFrameToLEDs();
                vTaskDelay(pdMS_TO_TICKS(33)); // ~30FPS
            }
        },
        "DisplayTask", 2048, NULL, 1, NULL, 1);
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