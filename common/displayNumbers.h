#pragma once

#include <FastLED.h>

void drawCenteredTwoDigitNumber(int value, CRGB color = CRGB::Blue);
void drawDigit(int digit, int startX, int startY, CRGB color);