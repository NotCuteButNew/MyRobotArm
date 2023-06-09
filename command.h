#ifndef COMMAND_H
#define COMMAND_H
#include "logger.h"
#include <Arduino.h>

class Command {
public:
  Command();
  bool handleCommand();
  bool processMessage(String msg);
  void value_segment(String msg_segment);
  void cmdToRelative();
  void cmdToAbsolute();
  float new_command[4];

private:
  String message;
};

#endif