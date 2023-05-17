#ifndef LOGGER_H_
#define LOGGER_H_

#include <Arduino.h>

//#define debug false

#define debug_logs()                                                           \
  do {                                                                         \
    char buf[100];                                                             \
    sprintf(buf, "DEBUG: %s:%d:%s", __FILE__, __LINE__, __FUNCTION__);         \
    Serial.println(buf);                                                       \
  } while (0)

#define LOG_ERROR 0
#define LOG_INFO 1
#define LOG_DEBUG 2

class Logger {
public:
  static void log(String message, int level);
  static void logINFO(String message);
  static void logERROR(String message);
  static void logDEBUG(String message);
};
#endif
