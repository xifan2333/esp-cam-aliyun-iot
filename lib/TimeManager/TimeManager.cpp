
/**
 * @file TimeManager.cpp
 * @author 稀饭
 * @brief 时间管理器类的源文件
 */
#include "TimeManager.h"

/**
 * ### 更新时间信息
 *
 * 更新类中保存的当前时间信息。
 */
void TimeManager::updateTime()
{

    configTime(3600 * 8, 0, "ntp1.ntsc.ac.cn", "time1.aliyun.com", "ntp.tencent.com");

    // 开始NTP时间同步
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    sntp_init();

    int checkCount = 0;
    sntp_sync_status_t syncStatus = sntp_get_sync_status();

    // 尝试最多100次等待时间同步，每次等待0.1秒，总等待时间10秒
    while (syncStatus != SNTP_SYNC_STATUS_COMPLETED && checkCount < 100)
    {
        logger.info("等待时间同步...", "Time");
        checkCount++; // 增加尝试次数
        delay(1000);   // 等待1秒
        syncStatus = sntp_get_sync_status();
    }

    // 检查是否成功同步时间
    if (syncStatus == SNTP_SYNC_STATUS_COMPLETED)
    {
        sntp_stop();
        String date = getFormattedDate();
        String time = getFormattedTime();
        logger.info("更新时间成功，当前时间：" + date + " " + time, "Time");
    }
    else
    {
        // 未能在10次尝试后同步时间
        logger.error("更新时间失败，超过最大尝试次数。", "Time");
    }
}

/**
 * ### 获取当前时间戳
 *
 * 获取当前时间的时间戳。
 *
 * #### 返回
 *
 * - int：当前时间的时间戳
 */
uint_fast64_t TimeManager::getTimestamp()
{
    time_t now = time(nullptr);
    timestamp = static_cast<int_fast64_t>(now);
    return timestamp * 1000LL;
}

/**
 * ### 获取当前年份
 *
 * 获取当前时间的年份。
 *
 * #### 返回
 *
 * - int：当前年份，如果获取失败返回 -1
 */
int TimeManager::getYear()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return -1;
    }
    year = timeinfo.tm_year + 1900;
    return year;
}

/**
 * ### 获取当前月份
 *
 * 获取当前时间的月份（1-12）。
 *
 * #### 返回
 *
 * - int：当前月份，如果获取失败返回 -1
 */
int TimeManager::getMonth()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return -1;
    }
    month = timeinfo.tm_mon + 1;
    return month;
}

/**
 * ### 获取当前星期几
 *
 * 获取当前时间是星期几的数值表示（0-6，0 表示星期日）。
 *
 * #### 返回
 *
 * - int：当前星期几的数值表示，如果获取失败返回 -1
 */
int TimeManager::getWeekDay()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return -1;
    }
    weekday = timeinfo.tm_wday;
    return weekday;
}

/**
 * ### 获取当前日期
 *
 * 获取当前时间的日期（1-31）。
 *
 * #### 返回
 *
 * - int：当前日期，如果获取失败返回 -1
 */
int TimeManager::getDay()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return -1;
    }
    day = timeinfo.tm_mday;
    return day;
}

/**
 * ### 获取当前小时
 *
 * 获取当前时间的小时（0-23）。
 *
 * #### 返回
 *
 * - int：当前小时，如果获取失败返回 -1
 */
int TimeManager::getHour()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return -1;
    }
    hour = timeinfo.tm_hour;
    return hour;
}

/**
 * ### 获取当前分钟
 *
 * 获取当前时间的分钟（0-59）。
 *
 * #### 返回
 *
 * - int：当前分钟，如果获取失败返回 -1
 */
int TimeManager::getMinute()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return -1;
    }
    minute = timeinfo.tm_min;
    return minute;
}

/**
 * ### 获取当前秒数
 *
 * 获取当前时间的秒数（0-59）。
 *
 * #### 返回
 *
 * - int：当前秒数，如果获取失败返回 -1
 */
int TimeManager::getSecond()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return -1;
    }
    second = timeinfo.tm_sec;
    return second;
}

/**
 * ### 获取格式化的日期字符串
 *
 * 获取当前时间的格式化日期字符串，格式为 "YYYY-MM-DD"。
 *
 * #### 返回
 *
 * - String：格式化的日期字符串，如果获取失败返回空字符串
 */
String TimeManager::getFormattedDate()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return "";
    }
    char buffer[16];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    formattedDate = String(buffer);
    return formattedDate;
}

/**
 * ### 获取格式化的时间字符串
 *
 * 获取当前时间的格式化时间字符串，格式为 "HH:MM"。
 *
 * #### 返回
 *
 * - String：格式化的时间字符串，如果获取失败返回空字符串
 */
String TimeManager::getFormattedTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return "";
    }
    char buffer[16];
    strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);
    formattedTime = String(buffer);
    return formattedTime;
}

/**
 * ### 获取格式化的日期和时间字符串
 *
 * 获取当前时间的格式化日期和时间字符串，格式为 "YYYYMMDDHHMMSS"。
 *
 * #### 返回
 *
 * - String：格式化的日期和时间字符串，如果获取失败返回空字符串
 */
String TimeManager::getFormattedDateAndTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return "";
    }
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", &timeinfo);
    formattedDateAndTime = String(buffer);
    return formattedDateAndTime;
}

/**
 * ### 获取格式化的星期几字符串
 *
 * 获取当前时间的格式化星期几字符串，如 "星期一"。
 *
 * #### 返回
 *
 * - String：格式化的星期几字符串，如果获取失败返回空字符串
 */
String TimeManager::getFormattedWeekday()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        return "";
    }
    int weekday = timeinfo.tm_wday;
    String formattedWeekday;
    switch (weekday)
    {
    case 0:
        formattedWeekday = "星期日";
        break;
    case 1:
        formattedWeekday = "星期一";
        break;
    case 2:
        formattedWeekday = "星期二";
        break;
    case 3:
        formattedWeekday = "星期三";
        break;
    case 4:
        formattedWeekday = "星期四";
        break;
    case 5:
        formattedWeekday = "星期五";
        break;
    case 6:
        formattedWeekday = "星期六";
        break;
    default:
        formattedWeekday = "未知";
        break;
    }
    return formattedWeekday;
}
