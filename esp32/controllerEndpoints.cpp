#include "controllerEndpoints.h"
#include <Arduino.h>


volatile bool commandFlags[CMD_COUNT] = { false };

void up()
{
    Serial.println("Up");
    commandFlags[CMD_UP] = true;
}

void down()
{
    Serial.println("Down");
    commandFlags[CMD_DOWN] = true;
}

void left()
{
    Serial.println("Left");
    commandFlags[CMD_LEFT] = true;
}

void right()
{
    Serial.println("Right");
    commandFlags[CMD_RIGHT] = true;
}

void select()
{
    Serial.println("Select");
    commandFlags[CMD_SELECT] = true;
}

void back()
{
    Serial.println("Back");
    commandFlags[CMD_BACK] = true;
}
