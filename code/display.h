#pragma once
#include <FastLED.h>

#define WIDTH 31
#define HEIGHT 31
#define NUM_LEDS (WIDTH * HEIGHT)
#define DATA_PIN 4

extern CRGB framebuffer[WIDTH][HEIGHT];

void initDisplay();
void renderFrameToLEDs();
void setupDisplay();
void clearFramebuffer();