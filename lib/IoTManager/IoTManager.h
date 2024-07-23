/**
 * @file IoTManager.h
 * @brief 定义了 IoTManager 类，用于管理物联网设备的连接和数据通信。
 * 
 * This file defines the IoTManager class, which handles connection and data communication
 * for IoT devices.
 */

#ifndef IOTMANAGER_H
#define IOTMANAGER_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "Logger.h"
#include "TimeManager.h"
#include <Crypto.h>
#include <SHA256.h>

#define SHA256HMAC_SIZE 32

extern WiFiClient wifiClient;    ///< 外部定义的 WiFi 客户端对象
extern PubSubClient mqttClient;  ///< 外部定义的 MQTT 客户端对象
extern Logger logger;            ///< 外部定义的日志记录器对象
extern TimeManager timeManager;  ///< 外部定义的时间管理器对象

/**
 * ### IoT 管理器类
 * 
 * 该类负责管理物联网设备的连接和数据通信。
 * 
 * #### 属性
 * 
 * - `productKey`：物联网设备的产品标识符
 * - `deviceName`：物联网设备的名称
 * - `deviceSecret`：物联网设备的密钥
 * - `username`：MQTT 连接的用户名
 * - `password`：MQTT 连接的密码
 * - `clientId`：MQTT 客户端 ID
 * - `hostUrl`：MQTT 服务器地址
 * - `brifeId`：物联网设备的简短 ID
 * - `port`：MQTT 服务器端口号
 * 
 * #### 方法
 * 
 * - `IoTManager()`：构造函数
 * - `sign()`：使用 HMAC-SHA256 签名方法获取密码
 * - `connect()`：连接到 MQTT 服务器
 * - `checkConnection()`：检查 MQTT 连接状态
 * - `sendData()`：发送数据到 MQTT 服务器
 */
class IoTManager {
public:
    String productKey;   ///< 物联网设备的产品标识符
    String deviceName;   ///< 物联网设备的名称
    String deviceSecret; ///< 物联网设备的密钥
    String username;     ///< MQTT 连接的用户名
    String password;     ///< MQTT 连接的密码
    String clientId;     ///< MQTT 客户端 ID
    String hostUrl;      ///< MQTT 服务器地址
    String brifeId;      ///< 物联网设备的简短 ID
    u_short port;        ///< MQTT 服务器端口号
    
    /**
     * ### 构造函数
     * 
     * 初始化 IoTManager 对象并设置物联网设备的连接参数。
     * 
     * #### 参数
     * 
     * - `productKey`：产品标识符
     * - `deviceName`：设备名称
     * - `deviceSecret`：设备密钥
     * - `hostUrl`：MQTT 服务器地址
     * - `port`：MQTT 服务器端口号
     */
    IoTManager(String productKey, String deviceName, String deviceSecret, String hostUrl, u_short port);

    /**
     * ### 使用 HMAC-SHA256 签名方法对文本进行签名
     * 
     * 使用设备密钥对指定文本进行 HMAC-SHA256 签名。
     * 
     * #### 参数
     * 
     * - `plaintext`：要签名的原始文本
     * - `deviceSecret`：设备密钥
     * 
     * #### 返回
     * 
     * - String：签名后的字符串
     */
    String sign(String plaintext, String deviceSecret);

    /**
     * ### 连接到 MQTT 服务器
     * 
     * 尝试连接到预设的 MQTT 服务器，使用构造函数中提供的参数。
     * 如果连接成功，将保持连接状态；如果连接失败，则记录错误日志。
     * 
     * #### 返回
     * 
     * - bool：连接成功返回 true，否则返回 false
     */
    bool connect();

    /**
     * ### 检查 MQTT 连接状态
     * 
     * 检查当前 MQTT 连接状态，若未连接则尝试重新连接。
     * 如果连接成功，调用 MQTT 客户端的 loop 方法以保持连接。
     */
    void checkConnection();

    /**
     * ### 发送数据到 MQTT 服务器
     * 
     * 将指定的键值对数据发送到 MQTT 服务器。
     * 
     * #### 参数
     * 
     * - `key`：数据的键
     * - `value`：数据的值
     */
    void sendData(String key, String value);
};

#endif // IOTMANAGER_H
