#pragma once

enum Command {
  CMD_UP = 0,
  CMD_DOWN,
  CMD_LEFT,
  CMD_RIGHT,
  CMD_SELECT,
  CMD_BACK,
  CMD_COUNT
};

extern volatile bool commandFlags[CMD_COUNT];


void up();
void down();
void left();
void right();
void select();
void back();