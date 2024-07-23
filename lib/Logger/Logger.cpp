#include "Logger.h"
#include "TimeManager.h"
extern TimeManager timeManager;
void Logger::info(String msg, String moduleName)
{
    String date = timeManager.getFormattedDate();
    String time = timeManager.getFormattedTime();
    Serial.println("[INFO]-" + date + " " + time + "-[" + moduleName + "]: " + msg);
}
void Logger::debug(String msg, String moduleName)
{
    String date = timeManager.getFormattedDate();
    String time = timeManager.getFormattedTime();
    Serial.println("[DEBUG]-" + date + " " + time + "-[" + moduleName + "]: " + msg);
}

void Logger::warning(String msg, String moduleName)
{
    String date = timeManager.getFormattedDate();
    String time = timeManager.getFormattedTime();
    Serial.println("[WARN]-" + date + " " + time + "-[" + moduleName + "]: " + msg);
}

void Logger::error(String msg, String moduleName)
{
    String date = timeManager.getFormattedDate();
    String time = timeManager.getFormattedTime();
    Serial.println("[ERROR]-" + date + " " + time + "-[" + moduleName + "]: " + msg);
}