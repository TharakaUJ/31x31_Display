#pragma once

#include "pixel.h"

void drawCenteredTwoDigitNumber(int value, Pixel color = Pixel{0, 0, 255});
void drawDigit(int digit, int startX, int startY, Pixel color);