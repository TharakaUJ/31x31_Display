#include "display.h"
#include "displayNumbers.h"
#include "platform.h"

const uint8_t digitFont[10][7] = {
    {0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110}, // 0
    {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}, // 1
    {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111}, // 2
    {0b11111, 0b00010, 0b00100, 0b00010, 0b00001, 0b10001, 0b01110}, // 3
    {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010}, // 4
    {0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110}, // 5
    {0b00110, 0b01000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110}, // 6
    {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000}, // 7
    {0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110}, // 8
    {0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b00010, 0b01100}  // 9
};

void drawDigit(int digit, int startX, int startY, CRGB color) {
    for (int y = 0; y < 7; ++y) {
        uint8_t row = digitFont[digit][y];
        for (int x = 0; x < 5; ++x) {
            if ((row >> (4 - x)) & 1) {
                int fx = startX + x;
                int fy = startY + y;
                if (fx >= 0 && fx < WIDTH && fy >= 0 && fy < HEIGHT)
                    framebuffer[fx][fy] = color;
            }
        }
    }
}

void drawCenteredTwoDigitNumber(int value, CRGB color) {
    value = constrain(value, 0, 99);
    int tens = value / 10;
    int units = value % 10;

    int digitWidth = 5;
    int spacing = 1;
    int totalWidth = digitWidth * 2 + spacing;
    int startX = (WIDTH - totalWidth) / 2;
    int startY = (HEIGHT - 7) / 2;

    drawDigit(tens, startX, startY, color);
    drawDigit(units, startX + digitWidth + spacing, startY, color);
}
