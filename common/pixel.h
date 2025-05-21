#pragma once

#ifdef ESP32
#include <Arduino.h>
#include <FastLED.h>
typedef CRGB Pixel;
#else
typedef struct
{
    uint8_t r, g, b;
} Pixel;
#endif

#define WIDTH 31
#define HEIGHT 31
#define NUM_LEDS (WIDTH * HEIGHT)

extern Pixel framebuffer[WIDTH][HEIGHT];
