#pragma once

#include <FastLED.h>

void drawDigit(int digit, int startX, int startY, CRGB color);
void drawCenteredTwoDigitNumber(int value, CRGB color = CRGB::Blue);
void drawCenteredThreeDigitNumber(int value, CRGB color = CRGB::Blue);