#include "command.h"

bool Command::handleCommand() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\r' || c == '\n') {
      bool b = processMessage(message);
      message = "";
      return b;
    } else {
      message += c;
    }
  }
  return false;
}

bool Command::processMessage(String msg) {

  new_command.valueX = 0;
  new_command.valueY = 0;
  new_command.valueZ = 0;
  int active_index = 0;
  active_index++;
  int temp_index = active_index;
  // 直到找到下一个字符
  while (temp_index < msg.length() && !isAlpha(msg[temp_index])) {
    temp_index++;
  }
  active_index = temp_index; 
  temp_index++;
  while (temp_index < msg.length()) {
    while (!isAlpha(msg[temp_index]) || msg[temp_index] == '.') {
      temp_index++;
      if (temp_index == msg.length()) {
        break;
      }
    }
    value_segment(msg.substring(active_index, temp_index));
    active_index = temp_index;
    temp_index++;
  }
  return true;
}