/**
 * @file IoTManager.cpp
 * @author 稀饭
 * @brief 实现了 IoTManager 类的方法，用于管理物联网设备连接及通信。
 */
#include "IoTManager.h"

/**
 * ### IoTManager 构造函数
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
IoTManager::IoTManager(String productKey, String deviceName, String deviceSecret, String hostUrl, u_short port)
{
    this->productKey = productKey;
    this->deviceName = deviceName;
    this->deviceSecret = deviceSecret;
    this->brifeId = productKey + "." + deviceName;
    this->username = deviceName + "&" + productKey;
    this->hostUrl = hostUrl;
    this->port = port;
}

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
String IoTManager::sign(String plaintext, String deviceSecret)
{
    byte hashCode[SHA256HMAC_SIZE];
    SHA256 sha256;

    const char *key = deviceSecret.c_str();
    size_t keySize = deviceSecret.length();

    sha256.resetHMAC(key, keySize);
    sha256.update((const byte *)plaintext.c_str(), plaintext.length());
    sha256.finalizeHMAC(key, keySize, hashCode, sizeof(hashCode));

    String sign = "";
    for (byte i = 0; i < SHA256HMAC_SIZE; ++i)
    {
        sign += "0123456789ABCDEF"[hashCode[i] >> 4];
        sign += "0123456789ABCDEF"[hashCode[i] & 0xf];
    }

    return sign;
}

/**
 * ### 连接到 MQTT 服务器
 *
 * 使用设定的连接参数连接到 MQTT 服务器。
 *
 * #### 返回
 *
 * - bool：连接成功返回 true，否则返回 false
 */
bool IoTManager::connect()
{
    String timestamp = String(timeManager.getTimestamp());
    logger.info("当前时间戳：" + timestamp, "MQTT");
    this->clientId = brifeId + "|securemode=2,signmethod=hmacsha256,timestamp=" + timestamp + "|";
    String plainText = "clientId" + brifeId + "deviceName" + deviceName + "productKey" + productKey + "timestamp" + timestamp;
    this->password = sign(plainText, deviceSecret);
    mqttClient.setServer(hostUrl.c_str(), port);
    mqttClient.setClient(wifiClient);
    mqttClient.connect(clientId.c_str(), username.c_str(), password.c_str());
    if (mqttClient.connected())
    {
        logger.info("MQTT连接成功", "MQTT");
    }
    else
    {
        String errorMsg;
        switch (mqttClient.state())
        {
            case -4:
                errorMsg = "MQTT连接失败: 没有可用的网络";
                break;
            case -3:
                errorMsg = "MQTT连接失败: DNS解析失败";
                break;
            case -2:
                errorMsg = "MQTT连接失败: 连接被拒绝";
                break;
            case -1:
                errorMsg = "MQTT连接失败: 客户端配置错误";
                break;
            case 0:
                errorMsg = "MQTT连接失败: 连接被拒绝(未知原因)";
                break;
            case 1:
                errorMsg = "MQTT连接失败: 连接被拒绝(不支持的协议版本)";
                break;
            case 2:
                errorMsg = "MQTT连接失败: 连接被拒绝(非法客户端标识符)";
                break;
            case 3:
                errorMsg = "MQTT连接失败: 连接被拒绝(服务器不可用)";
                break;
            case 4:
                errorMsg = "MQTT连接失败: 连接被拒绝(无效用户名或密码)";
                break;
            case 5:
                errorMsg = "MQTT连接失败: 连接被拒绝(未授权)";
                break;
            default:
                errorMsg = "MQTT连接失败: 未知错误(" + String(mqttClient.state()) + ")";
                break;
        }
        logger.error(errorMsg, "MQTT");
    }
    return mqttClient.connected();
}

/**
 * ### 检查 MQTT 连接状态
 *
 * 检查当前 MQTT 连接状态，若未连接则尝试重新连接。
 */
void IoTManager::checkConnection()
{
    bool connected = mqttClient.connected();
    if (!connected)
    {
        connect();
    }
    else
    {
        mqttClient.loop();
    }
}
