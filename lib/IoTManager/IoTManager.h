#ifndef IOTMANAGER_H
#define IOTMANAGER_H

#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "Logger.h"
#include "TimeManager.h"
#include <mbedtls/md.h>
#include <Ticker.h>

extern WiFiClient wifiClient;
extern PubSubClient mqttClient;
extern Logger logger;
extern TimeManager timeManager;

#define SHA256HMAC_SIZE 32
#define MESSAGE_QUEUE_CHECK_INTERVAL 5
#define CONNECTION_CHECK_INTERVAL 10
#define KEEP_ALIVE_INTERVAL 60
#define MAX_BUFFER_SIZE 1024
#define MAX_TOPIC_SIZE 512
#define ALINK_BODY_FORMAT "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"
#define ALINK_EVENT_BODY_FORMAT "{\"id\": \"123\",\"version\": \"1.0\",\"params\": %s,\"method\": \"thing.event.%s.post\"}"
#define ALINK_TOPIC_PROP_POST "/sys/%s/%s/thing/event/property/post"
#define ALINK_TOPIC_PROP_SET "/sys/%s/%s/thing/service/property/set"
#define ALINK_TOPIC_USER "/sys/%s/%s/thing/event/user/%s"
#define ALINK_TOPIC_GENERIC "/sys/%s/%s/thing/event/%s"
#define ALINK_TOPIC_EVENT "/sys/%s/%s/thing/event"

typedef void (*callbackFunction)(JsonVariant);

struct PropertyMessage {
    String key;
    String value;
};

struct CallbackEntry {
    String key;
    callbackFunction callbackFn;
};

/**
 * @class IoTManager
 * @brief 管理 IoT 设备的连接、订阅和消息处理。
 *
 * IoTManager 类处理 MQTT 连接、属性和事件管理以及 IoT 设备的消息处理。
 * 它使用各种回调来处理特定的消息类型和属性。
 */
class IoTManager {
public:
    static IoTManager *instance;
    /**
     * @brief IoTManager 的构造函数。
     * @param productKey 设备的产品密钥。
     * @param deviceName 设备名称。
     * @param deviceSecret 设备密钥。
     * @param hostUrl MQTT 代理的主机 URL。
     * @param port MQTT 代理的端口。
     */
    IoTManager(String productKey, String deviceName, String deviceSecret, String hostUrl, u_short port);

    /**
     * @brief 使用设备密钥通过 HMAC-SHA256 对明文消息进行签名。
     * @param plaintext 要签名的明文消息。
     * @return 签名后的消息（十六进制字符串）。
     */
    String sign(const char *plaintext);

    /**
     * @brief 连接到 MQTT 代理。
     * @return 如果连接成功返回 true，否则返回 false。
     */
    bool connect();

    /**
     * @brief 检查连接状态并在必要时重新连接。
     */
    void checkConnection();

    /**
     * @brief 处理 MQTT 客户端循环以保持连接并处理消息。
     */
    void loop();

    /**
     * @brief 发布消息到指定的 MQTT 主题。
     * @param topic 要发布的 MQTT 主题。
     * @param payload 要发送的有效负载。
     * @param retained 消息是否应该由代理保留。
     * @return 如果发布成功返回 true，否则返回 false。
     */
    bool publish(String topic, String payload, bool retained);

    /**
     * @brief 发布消息到指定的 MQTT 主题。
     * @param topic 要发布的 MQTT 主题。
     * @param payload 要发送的有效负载。
     * @return 如果发布成功返回 true，否则返回 false。
     */
    bool publish(String topic, String payload);

    /**
     * @brief 发布用户消息到指定的 MQTT 主题后缀。
     * @param topicSuffix 主题后缀。
     * @param payload 要发送的有效负载。
     * @return 如果发布成功返回 true，否则返回 false。
     */
    bool publishUser(String topicSuffix, String payload);

    /**
     * @brief 订阅用户消息主题。
     * @param topicSuffix 主题后缀。
     * @param fp 回调函数。
     * @return 如果订阅成功返回 true，否则返回 false。
     */
    bool subscribeUser(String topicSuffix, callbackFunction fp);

    /**
     * @brief 取消订阅用户消息主题。
     * @param topicSuffix 主题后缀。
     * @return 如果取消订阅成功返回 true，否则返回 false。
     */
    bool unsubscribeUser(String topicSuffix);

    /**
     * @brief 订阅指定的 MQTT 主题。
     * @param topic 要订阅的 MQTT 主题。
     * @param qos 服务质量（QoS）级别。
     * @param fp 回调函数。
     * @return 如果订阅成功返回 true，否则返回 false。
     */
    bool subscribe(String topic, uint8_t qos, callbackFunction fp);

    /**
     * @brief 订阅指定的 MQTT 主题。
     * @param topic 要订阅的 MQTT 主题。
     * @param fp 回调函数。
     * @return 如果订阅成功返回 true，否则返回 false。
     */
    bool subscribe(String topic, callbackFunction fp);

    /**
     * @brief 取消订阅指定的 MQTT 主题。
     * @param topic 要取消订阅的 MQTT 主题。
     * @return 如果取消订阅成功返回 true，否则返回 false。
     */
    bool unsubscribe(String topic);

    /**
     * @brief 发送属性消息。
     * @param key 属性的键。
     * @param value 属性的值。
     */
    void sendProperty(String key, String value);

    /**
     * @brief 发送属性消息。
     * @param key 属性的键。
     * @param value 属性的值（整数）。
     */
    void sendProperty(String key, int value);

    /**
     * @brief 发送属性消息。
     * @param key 属性的键。
     * @param value 属性的值（浮点数）。
     */
    void sendProperty(String key, float value);

    /**
     * @brief 发送属性消息。
     * @param key 属性的键。
     * @param value 属性的值（双精度浮点数）。
     */
    void sendProperty(String key, double value);

    /**
     * @brief 发送事件消息。
     * @param eventId 事件ID。
     * @param param 事件参数。
     */
    void sendEvent(String eventId, String param);

    /**
     * @brief 发送事件消息。
     * @param eventId 事件ID。
     */
    void sendEvent(String eventId);

    /**
     * @brief 添加属性的回调函数。
     * @param key 属性的键。
     * @param callbackFn 回调函数。
     * @return 如果添加成功返回 true，否则返回 false。
     */
    bool bindData(String key, callbackFunction callbackFn);

    /**
     * @brief 移除属性的回调函数。
     * @param key 属性的键。
     * @return 如果移除成功返回 true，否则返回 false。
     */
    bool unbindData(String key);
    static void checkMessageQueueCallback();
    static void checkConnectionCallback();
    static void mqttCallback(char *topic, byte *payload, unsigned int length);
private:
    String productKey;
    String deviceName;
    String deviceSecret;
    String briefId;
    String username;
    String hostUrl;
    u_short port;
    String clientId;
    String password;
    String topicPropPost;
    String topicPropSet;
    String topicEvent;
    String topicUser;

    std::vector<PropertyMessage> messageQueue; // 使用vector管理消息队列
    std::vector<CallbackEntry> callbackArray; // 使用vector管理回调函数
    Ticker queueCheckTicker;
    Ticker connectionCheckTicker;

    /**
     * @brief 记录错误信息。
     */
    void logError();

    /**
     * @brief 处理收到的 MQTT 消息。
     * @param topic 主题。
     * @param payload 消息负载。
     * @param length 消息长度。
     */
    void callback(char *topic, byte *payload, unsigned int length);

    /**
     * @brief 处理属性设置消息。
     * @param jsonVariant JSON 变体。
     */
    void processPropertySetMessage(JsonVariant jsonVariant);

    /**
     * @brief 处理用户消息。
     * @param topic 主题。
     * @param jsonVariant JSON 变体。
     */
    void processUserMessage(const char *topic, JsonVariant jsonVariant);

    /**
     * @brief 处理通用消息。
     * @param topic 主题。
     * @param jsonVariant JSON 变体。
     */
    void processGenericMessage(const char *topic, JsonVariant jsonVariant);

    /**
     * @brief 检查消息队列。
     */
    void checkMessageQueue();


    /**
     * @brief 发送通用属性消息。
     * @param Payload 消息负载。
     */
    void sendGenericPropetry(String Payload);
   
};

#endif // IOTMANAGER_H

