#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
class Logger {
public:
    void info(String msg,String moduleName);
    void debug(String msg, String moduleName);
    void warning(String msg, String moduleName);
    void error(String msg, String moduleName);
};

#endif