#include "command.h"
#include "WString.h"
#include <ctype.h>

Command::Command() {
  new_command[0] = {0};
  new_command[1] = {0};
  new_command[2] = {0};
  new_command[3] = {0};
  message = "";
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
    if (msg[end_index]==',' || end_index == msg.length()) {
      new_command[i] = msg.substring(begin_index, end_index).toFloat();
      i++;
      begin_index = end_index + 1;
    }
    end_index++;
  }
#if debug
  Logger::logDEBUG(msg + "\tprocessMessage: [" + String(new_command[0]) + " " +
                   String(new_command[1]) + " " + String(new_command[2]) + " " +
                   String(new_command[3]) + "]");
#endif
  return true;
}
