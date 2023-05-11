#ifndef COMMAND_H
#define COMMAND_H
#include <Arduino.h>

struct Cmd {
  float valueX;
  float valueY;
  float valueZ;
};

class Command {
  public:
    Command();
    bool handleCommand();
    bool processMessage(String msg);
    void value_segment(String msg_segment);
    Cmd getCmd() const;
    void cmdToRelative();
    void cmdToAbsolute();  
    bool isRelativeCoord;
    Cmd new_command;

  private: 
    String message;
};

#endif