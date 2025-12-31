#pragma once

#include <stdint.h>

// Mock CRGB structure to mimic FastLED for WASM builds
struct CRGB {
    union {
        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };
        uint8_t raw[3];
    };
    
    CRGB() : r(0), g(0), b(0) {}
    CRGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
    
    inline CRGB& operator= (const CRGB& rhs) {
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        return *this;
    }
    
    inline CRGB& setRGB(uint8_t red, uint8_t green, uint8_t blue) {
        r = red;
        g = green;
        b = blue;
        return *this;
    }
    
    inline void setHSV(uint8_t hue, uint8_t sat, uint8_t val) {
        // Simple HSV to RGB conversion
        if (sat == 0) {
            r = g = b = val;
            return;
        }
        
        uint8_t region = hue / 43;
        uint8_t remainder = (hue - (region * 43)) * 6;
        uint8_t p = (val * (255 - sat)) >> 8;
        uint8_t q = (val * (255 - ((sat * remainder) >> 8))) >> 8;
        uint8_t t = (val * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;
        
        switch (region) {
            case 0: r = val; g = t; b = p; break;
            case 1: r = q; g = val; b = p; break;
            case 2: r = p; g = val; b = t; break;
            case 3: r = p; g = q; b = val; break;
            case 4: r = t; g = p; b = val; break;
            default: r = val; g = p; b = q; break;
        }
    }
};

// Common color definitions
#define CRGB_Black   CRGB(0,0,0)
#define CRGB_White   CRGB(255,255,255)
#define CRGB_Red     CRGB(255,0,0)
#define CRGB_Green   CRGB(0,255,0)
#define CRGB_Blue    CRGB(0,0,255)
#define CRGB_Yellow  CRGB(255,255,0)
#define CRGB_Cyan    CRGB(0,255,255)
#define CRGB_Magenta CRGB(255,0,255)
