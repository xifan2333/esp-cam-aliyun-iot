/**
 * @file Logger.cpp
 * @author 稀饭
 * @brief 日志记录器类的源文件
 */

#include "Logger.h"
#include "TimeManager.h"

extern TimeManager timeManager;

/**
 * ### 记录信息级别的日志。
 * 
 * ### 参数
 * 
 * - `msg`：日志消息
 * - `moduleName`：模块名称
 */
void Logger::info(String msg, String moduleName)
{
    String date = timeManager.getFormattedDate();
    String time = timeManager.getFormattedTime();
    Serial.println("[INFO]-" + date + " " + time + "-[" + moduleName + "]: " + msg);
}
/**
 * ### 输出调试信息
 * 
 * #### 参数
 * 
 * - `msg`：调试消息
 * - `moduleName`：模块名称
 */
void Logger::debug(String msg, String moduleName)
{
    String date = timeManager.getFormattedDate();
    String time = timeManager.getFormattedTime();
    Serial.println("[DEBUG]-" + date + " " + time + "-[" + moduleName + "]: " + msg);
}

/**
 * ### 输出警告信息
 * 
 * #### 参数
 * 
 * - `msg`：警告消息
 * - `moduleName`：模块名称
 */
void Logger::warning(String msg, String moduleName)
{
    String date = timeManager.getFormattedDate();
    String time = timeManager.getFormattedTime();
    Serial.println("[WARN]-" + date + " " + time + "-[" + moduleName + "]: " + msg);
}
/**
 * ### 输出错误信息
 * 
 * #### 参数
 * 
 * - `msg`：错误消息
 * - `moduleName`：模块名称
 */
void Logger::error(String msg, String moduleName)
{
    String date = timeManager.getFormattedDate();
    String time = timeManager.getFormattedTime();
    Serial.println("[ERROR]-" + date + " " + time + "-[" + moduleName + "]: " + msg);
}