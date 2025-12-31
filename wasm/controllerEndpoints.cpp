#include "controllerEndpoints.h"

// Controller state flags
volatile bool commandFlags[CMD_COUNT] = {false};

void up() {
    commandFlags[CMD_UP] = true;
}

void down() {
    commandFlags[CMD_DOWN] = true;
}

void left() {
    commandFlags[CMD_LEFT] = true;
}

void right() {
    commandFlags[CMD_RIGHT] = true;
}

void select() {
    commandFlags[CMD_SELECT] = true;
}

void back() {
    commandFlags[CMD_BACK] = true;
}
