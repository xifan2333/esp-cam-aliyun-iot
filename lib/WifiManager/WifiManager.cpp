
/**
 * @file WifiManager.cpp
 * @author 稀饭
 * @brief 实现了 WifiManager 类的方法，用于连接到 WiFi 网络并检查连接状态。
 */
#include "WifiManager.h"

/**
 * ### 构造函数
 *
 * 初始化 WiFiManager 对象并设置要连接的 WiFi 网络的 SSID 和密码。
 *
 * #### 参数
 *
 * - ssid：WiFi 网络的SSID
 * - password：WiFi 网络的密码
 */
WifiManager::WifiManager(String ssid, String password)
{
    this->ssid = ssid;
    this->password = password;
}

/**
 * ### 检查 WiFi 连接状态
 *
 * 检查当前设备是否已连接到 WiFi 网络。
 *
 * #### 返回
 *
 * - bool：true 表示已连接，false 表示未连接
 */
bool WifiManager::checkConnection()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        logger.info("检查 WiFi 状态：已连接", "WiFi");
        return true;
    }
    else
    {
        logger.error("检查 WiFi 状态：未连接", "WiFi");
        return false;
    }
}

/**
 * ### 连接到 WiFi 网络
 *
 * 尝试连接到预设的 WiFi 网络，使用构造函数中提供的 SSID 和密码。
 * 如果连接成功，将保持连接状态；如果在 10 秒内未能成功连接，将放弃连接。
 */
void WifiManager::connect()
{
    WiFi.begin(ssid.c_str(), password.c_str());
    int count = 0;
    while (WiFi.status() != WL_CONNECTED && count < 10)
    {
        delay(1000);
        logger.info("WiFi 正在连接中……", "WiFi");
        count++;
    }
}
