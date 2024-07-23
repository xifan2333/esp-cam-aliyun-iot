/**
 * @file Logger.h
 * @author 稀饭
 * @brief 日志记录器类的头文件
 */
#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>
/**
 * ### 日志记录器类，用于记录不同级别的日志信息。
 */
class Logger {
public:
    /**
     * ### 记录信息级别的日志。
     * 
     * #### 参数
     * 
     * - `msg`：日志消息
     * - `moduleName`：模块名称
     */
    void info(String msg, String moduleName);

    /**
     * ### 记录调试级别的日志。
     * 
     * - `msg`：日志消息
     * - `moduleName`：模块名称
     */
    void debug(String msg, String moduleName);

    /**
     * ### 记录警告级别的日志。
     * 
     * #### 参数
     * 
     * - `msg`：日志消息
     * - `moduleName`：模块名称
     */
    void warning(String msg, String moduleName);

    /**
     * ### 记录错误级别的日志。
     * 
     * #### 参数
     * 
     * - `msg`：日志消息
     * - `moduleName`：模块名称
     */
    void error(String msg, String moduleName);
};

#endif