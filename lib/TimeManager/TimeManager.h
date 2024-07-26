/**
 * @file TimeManager.h
 * @author 稀饭
 * @brief 定义了 TimeManager 类，用于管理和获取时间信息。
 */

#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <Arduino.h>
#include <time.h>
#include "Logger.h"
#include "esp_sntp.h"

extern Logger logger; ///< 外部定义的日志记录器对象

/**
 * ### 时间管理器类
 *
 * 该类提供了获取和管理当前时间信息的功能。
 *
 * #### 属性
 *
 * - `formattedDate`：格式化的日期字符串
 * - `formattedTime`：格式化的时间字符串
 * - `formattedWeekday`：格式化的星期几字符串
 * - `formattedDateAndTime`：格式化的日期和时间字符串
 * - `timestamp`：时间戳
 * - `year`：年份
 * - `month`：月份
 * - `weekday`：星期几（0-6，0 表示星期日）
 * - `day`：日期（1-31）
 * - `hour`：小时（0-23）
 * - `minute`：分钟（0-59）
 * - `second`：秒数（0-59）
 *
 * #### 方法
 *
 * - `updateTime()`：更新时间信息
 * - `getTimestamp()`：获取当前时间戳
 * - `getYear()`：获取当前年份
 * - `getMonth()`：获取当前月份
 * - `getWeekDay()`：获取当前星期几
 * - `getDay()`：获取当前日期
 * - `getHour()`：获取当前小时
 * - `getMinute()`：获取当前分钟
 * - `getSecond()`：获取当前秒数
 * - `getFormattedDate()`：获取格式化的日期字符串
 * - `getFormattedWeekday()`：获取格式化的星期几字符串
 * - `getFormattedTime()`：获取格式化的时间字符串
 * - `getFormattedDateAndTime()`：获取格式化的日期和时间字符串
 */
class TimeManager
{
public:
    String formattedDate;        ///< 格式化的日期字符串
    String formattedTime;        ///< 格式化的时间字符串
    String formattedWeekday;     ///< 格式化的星期几字符串
    String formattedDateAndTime; ///< 格式化的日期和时间字符串
    int timestamp;               ///< 时间戳
    int year;                    ///< 年份
    int month;                   ///< 月份
    int weekday;                 ///< 星期几（0-6，0 表示星期日）
    int day;                     ///< 日期（1-31）
    int hour;                    ///< 小时（0-23）
    int minute;                  ///< 分钟（0-59）
    int second;                  ///< 秒数（0-59）

    /**
     * ### 更新时间信息
     *
     * 更新类中保存的当前时间信息。
     */
    void updateTime();

    /**
     * ### 获取当前时间戳
     *
     * 获取当前时间的时间戳。
     *
     * #### 返回
     *
     * - int：当前时间的时间戳
     */
    uint_fast64_t getTimestamp();

    /**
     * ### 获取当前年份
     *
     * 获取当前时间的年份。
     *
     * #### 返回
     *
     * - int：当前年份，如果获取失败返回 -1
     */
    int getYear();

    /**
     * ### 获取当前月份
     *
     * 获取当前时间的月份（1-12）。
     *
     * #### 返回
     *
     * - int：当前月份，如果获取失败返回 -1
     */
    int getMonth();

    /**
     * ### 获取当前星期几
     *
     * 获取当前时间是星期几的数值表示（0-6，0 表示星期日）。
     *
     * #### 返回
     *
     * - int：当前星期几的数值表示，如果获取失败返回 -1
     */
    int getWeekDay();

    /**
     * ### 获取当前日期
     *
     * 获取当前时间的日期（1-31）。
     *
     * #### 返回
     *
     * - int：当前日期，如果获取失败返回 -1
     */
    int getDay();

    /**
     * ### 获取当前小时
     *
     * 获取当前时间的小时（0-23）。
     *
     * #### 返回
     *
     * - int：当前小时，如果获取失败返回 -1
     */
    int getHour();

    /**
     * ### 获取当前分钟
     *
     * 获取当前时间的分钟（0-59）。
     *
     * #### 返回
     *
     * - int：当前分钟，如果获取失败返回 -1
     */
    int getMinute();

    /**
     * ### 获取当前秒数
     *
     * 获取当前时间的秒数（0-59）。
     *
     * #### 返回
     *
     * - int：当前秒数，如果获取失败返回 -1
     */
    int getSecond();

    /**
     * ### 获取格式化的日期字符串
     *
     * 获取当前时间的格式化日期字符串，格式为 "YYYY-MM-DD"。
     *
     * #### 返回
     *
     * - String：格式化的日期字符串，如果获取失败返回空字符串
     */
    String getFormattedDate();

    /**
     * ### 获取格式化的星期几字符串
     *
     * 获取当前时间的格式化星期几字符串，如 "星期一"。
     *
     * #### 返回
     *
     * - String：格式化的星期几字符串，如果获取失败返回空字符串
     */
    String getFormattedWeekday();

    /**
     * ### 获取格式化的时间字符串
     *
     * 获取当前时间的格式化时间字符串，格式为 "HH:MM"。
     *
     * #### 返回
     *
     * - String：格式化的时间字符串，如果获取失败返回空字符串
     */
    String getFormattedTime();

    /**
     * ### 获取格式化的日期和时间字符串
     *
     * 获取当前时间的格式化日期和时间字符串，格式为 "YYYYMMDDHHMMSS"。
     *
     * #### 返回
     *
     * - String：格式化的日期和时间字符串，如果获取失败返回空字符串
     */
    String getFormattedDateAndTime();
};

#endif // TIMEMANAGER_H
