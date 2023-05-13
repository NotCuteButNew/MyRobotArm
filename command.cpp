#include "command.h"
#include <ctype.h>

Command::Command() {
  new_command[0] = {0};
  new_command[1] = {0};
  new_command[2] = {0};
  new_command[3] = {0};
  message = "";
}

bool Command::handleCommand() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\r' || c == '\n') {
      bool b = processMessage(message);
#if debug
      Logger::logDEBUG("handleCommand: [" + message + "]");
#endif
      message = "";
      return b;
    } else {
      message += c;
    }
  }
  return false;
}

bool Command::processMessage(String msg) {
  new_command[0] = 0;
  new_command[1] = 0;
  new_command[2] = 0;
  new_command[3] = 0;
  int begin_index = 0;
  int end_index = begin_index;
  // 直到找到下一个deg
  int i = 0;
  while (end_index <= msg.length()) {
    if (isspace(msg[end_index]) || end_index == msg.length()) {
      new_command[i] = msg.substring(begin_index, end_index).toFloat();
      i++;
      begin_index = end_index + 1;
    }
    end_index++;
  }
  return true;
}
