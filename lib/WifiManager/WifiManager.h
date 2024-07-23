/**
 * @file WifiManager.h
 * @author 稀饭
 * @brief 定义了 WiFiManager 类，用于连接到 WiFi 网络并检查连接状态。
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "Logger.h"

extern Logger logger;        // 外部定义的日志记录器对象
extern WiFiClient wifiClient; // 外部定义的 WiFi 客户端对象

/**
 * ### WiFi 管理器类
 * 
 * 该类负责连接到指定的 WiFi 网络并检查连接状态。
 */
class WifiManager {
public:
    String ssid;       ///< WiFi 网络的SSID
    String password;   ///< WiFi 网络的密码

    /**
     * ### 构造函数
     * 
     * 初始化 WifiManager 对象并设置要连接的 WiFi 网络的 SSID 和密码。
     * 
     * #### 参数
     * 
     * - `ssid`：WiFi 网络的SSID
     * - `password`：WiFi 网络的密码
     */
    WifiManager(String ssid, String password);

    /**
     * ### 连接到 WiFi 网络
     * 
     * 尝试连接到预设的 WiFi 网络，使用构造函数中提供的 SSID 和密码。
     * 如果连接成功，将保持连接状态；如果在 10 秒内未能成功连接，将放弃连接。
     */
    void connect();

    /**
     * ### 检查 WiFi 连接状态
     * 
     * 检查当前设备是否已连接到 WiFi 网络。
     * 
     * #### 返回
     * 
     * -  bool：true | false 当前 WiFi 连接状态，true 表示已连接，false 表示未连接
     */
    bool checkConnection();
};

#endif // WIFI_MANAGER_H
