#include "controllerEndpoints.h"
#include "utils.h"

volatile bool commandFlags[CMD_COUNT] = { false };

void up()
{
    printInfo("Up");
    commandFlags[CMD_UP] = true;
}

void down()
{
    printInfo("Down");
    commandFlags[CMD_DOWN] = true;
}

void left()
{
    printInfo("Left");
    commandFlags[CMD_LEFT] = true;
}

void right()
{
    printInfo("Right");
    commandFlags[CMD_RIGHT] = true;
}

void select()
{
    printInfo("Select");
    commandFlags[CMD_SELECT] = true;
}

void back()
{
    printInfo("Back");
    commandFlags[CMD_BACK] = true;
}
