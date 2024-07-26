#include "IoTManager.h"

IoTManager* IoTManager::instance = nullptr;

IoTManager::IoTManager(String productKey, String deviceName, String deviceSecret, String hostUrl, u_short port)
    : productKey(productKey),
      deviceName(deviceName),
      deviceSecret(deviceSecret),
      hostUrl(hostUrl),
      port(port)
{
    this->briefId = productKey + "." + deviceName;
    this->username = deviceName + "&" + productKey;
    IoTManager::instance = this;

    char* topicBuffer = new char[MAX_TOPIC_SIZE];  // 动态分配缓冲区

    snprintf(topicBuffer, MAX_TOPIC_SIZE, ALINK_TOPIC_PROP_POST, productKey.c_str(), deviceName.c_str());
    this->topicPropPost = String(topicBuffer);

    snprintf(topicBuffer, MAX_TOPIC_SIZE, ALINK_TOPIC_PROP_SET, productKey.c_str(), deviceName.c_str());
    this->topicPropSet = String(topicBuffer);

    snprintf(topicBuffer, MAX_TOPIC_SIZE, ALINK_TOPIC_EVENT, productKey.c_str(), deviceName.c_str());
    this->topicEvent = String(topicBuffer);

    snprintf(topicBuffer, MAX_TOPIC_SIZE, ALINK_TOPIC_USER, productKey.c_str(), deviceName.c_str());
    this->topicUser = String(topicBuffer);

    delete[] topicBuffer;  // 释放动态分配的缓冲区
}

String IoTManager::sign(const char *plaintext)
{
   

   // 转换字符串到 C 样式的 char 数组
    const char *key = deviceSecret.c_str();
    size_t keySize = deviceSecret.length();
    uint8_t *sign = new uint8_t[32]; // SHA-256 produces a 32-byte hash

    // 初始化 mbedTLS HMAC 上下文
    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, md_info, 1);

    // 开始 HMAC 计算
    mbedtls_md_hmac_starts(&ctx, (const unsigned char *)key, keySize);

    // 输入数据进行 HMAC 计算
    mbedtls_md_hmac_update(&ctx, (const unsigned char *)plaintext, strlen(plaintext));

    // 完成 HMAC 计算并获取结果
    mbedtls_md_hmac_finish(&ctx, sign);

    // 释放 mbedTLS HMAC 上下文
    mbedtls_md_free(&ctx);

    // 将 HMAC 签名转换为十六进制字符串
    char hexSign[32 * 2 + 1];
    for (int i = 0; i < 32; i++) {
        sprintf(&hexSign[i * 2], "%02x", sign[i]);
    }
    hexSign[32 * 2] = '\0';

    // 清理内存
    delete[] sign;

    String result = String(hexSign);
    return result;
}

bool IoTManager::connect()
{
    String timestamp = String(timeManager.getTimestamp());
    char clientIdBuffer[256];
    snprintf(clientIdBuffer, sizeof(clientIdBuffer), "%s|securemode=2,signmethod=hmacsha256,timestamp=%s|", briefId.c_str(), timestamp.c_str());
    this->clientId = String(clientIdBuffer);

    char plainTextBuffer[256];
    snprintf(plainTextBuffer, sizeof(plainTextBuffer), "clientId%sdeviceName%sproductKey%s%s%s",
             briefId.c_str(), deviceName.c_str(), productKey.c_str(), "timestamp", timestamp.c_str());
    this->password = sign(plainTextBuffer);
    mqttClient.setServer(hostUrl.c_str(), port);
    mqttClient.setBufferSize(MAX_BUFFER_SIZE);
    mqttClient.setKeepAlive(KEEP_ALIVE_INTERVAL);
    mqttClient.setClient(wifiClient);
    mqttClient.setCallback(IoTManager::mqttCallback);
    mqttClient.connect(clientId.c_str(), username.c_str(), password.c_str());
    if (mqttClient.connected())
    {
        logger.info("MQTT连接成功", "MQTT");
        connectionCheckTicker.attach(CONNECTION_CHECK_INTERVAL, IoTManager::checkConnectionCallback);
        queueCheckTicker.attach(MESSAGE_QUEUE_CHECK_INTERVAL, IoTManager::checkMessageQueueCallback);
    }
    else
    {
        logError();
    }
    return mqttClient.connected();
}

void IoTManager::checkConnection()
{
    if (!mqttClient.connected())
    {
        logError();
        logger.info("MQTT连接断开，尝试重新连接……", "MQTT");
        connect();
    }
}

void IoTManager::loop()
{
    mqttClient.loop();
}

bool IoTManager::publish(String topic, String payload, bool retained)
{
    return mqttClient.publish(topic.c_str(), payload.c_str(), retained);
}

bool IoTManager::publish(String topic, String payload)
{
    return mqttClient.publish(topic.c_str(), payload.c_str());
}

bool IoTManager::publishUser(String topicSuffix, String payload)
{
    char topic[150];
    strcpy(topic, topicUser.c_str());
    return publish(String(strcat(topic, topicSuffix.c_str())), payload);
}

bool IoTManager::subscribeUser(String topicSuffix, callbackFunction fp)
{
    char *topic = new char[150];
    strcpy(topic, topicUser.c_str());
    return subscribe(String(strcat(topic, topicSuffix.c_str())), fp);
}

bool IoTManager::unsubscribeUser(String topicSuffix)
{
    char *topic = new char[150];
    strcpy(topic, topicUser.c_str());
    return unsubscribe(String(strcat(topic, topicSuffix.c_str())));
}

bool IoTManager::subscribe(String topic, uint8_t qos, callbackFunction fp)
{
    bool ret = false;
    if (mqttClient.subscribe(topic.c_str(), qos))
    {
        ret = true;
        bindData(topic, fp);
        logger.info("订阅: " + topic, "MQTT");
    }
    return ret;
}

bool IoTManager::unsubscribe(String topic)
{
    bool ret = false;
    if (mqttClient.unsubscribe(topic.c_str()))
    {
        ret = true;
        unbindData(topic);
        logger.info("取消订阅: " + topic, "MQTT");
    }
    return ret;
}

void IoTManager::sendProperty(String key, String value)
{
    PropertyMessage msg = {key, value};
    messageQueue.push_back(msg);
}

void IoTManager::sendProperty(String key, int value)
{
    PropertyMessage msg = {key, String(value)};
    messageQueue.push_back(msg);
}
void IoTManager::sendProperty(String key, float value)
{
    PropertyMessage msg = {key, String(value)};
    messageQueue.push_back(msg);
}

void IoTManager::sendProperty(String key, double value)
{
    PropertyMessage msg = {key, String(value)};
    messageQueue.push_back(msg);
}
void IoTManager::sendEvent(String eventId, String parameters)
{
    char topicPath[156];
    snprintf(topicPath, sizeof(topicPath), "%s/%s/post", topicEvent.c_str(), eventId.c_str());

    char jsonPayload[1024];
    snprintf(jsonPayload, sizeof(jsonPayload), ALINK_EVENT_BODY_FORMAT, parameters.c_str(), eventId.c_str());

    logger.info("发送事件 " + String(topicPath) + " " + String(jsonPayload), "MQTT");

    bool publishSuccess = mqttClient.publish(topicPath, jsonPayload);
    if (publishSuccess)
    {
        logger.info("MQTT事件发送成功: " + String(jsonPayload), "MQTT");
    }
    else
    {
        logger.error("MQTT事件发送失败: " + String(jsonPayload), "MQTT");
    }
}

void IoTManager::sendEvent(String eventId)
{
    sendEvent(eventId, "{}");
}

bool IoTManager::bindData(String key, callbackFunction callbackFn)
{
    if (key == NULL || callbackFn == NULL)
    {
        return false;
    }
    for (auto &entry : callbackArray)
    {
        if (entry.callbackFn == nullptr)
        {
            entry.key = key;
            entry.callbackFn = callbackFn;
            return true;
        }
    }

    callbackArray.push_back({key, callbackFn});
    return true;
}

bool IoTManager::unbindData(String key)
{
    for (auto it = callbackArray.begin(); it != callbackArray.end();)
    {
        if (it->key == key)
        {
            it = callbackArray.erase(it);
            return true;
        }
        else
        {
            it++;
        }
    }
    return false;
}

void IoTManager::logError()
{
    String errorMsg;
    int state = mqttClient.state();

    switch (state)
    {
    case MQTT_CONNECTION_TIMEOUT:
        errorMsg = "MQTT连接失败: 连接超时";
        break;
    case MQTT_CONNECTION_LOST:
        errorMsg = "MQTT连接失败: 连接丢失";
        break;
    case MQTT_CONNECT_FAILED:
        errorMsg = "MQTT连接失败: 连接请求失败";
        break;
    case MQTT_DISCONNECTED:
        errorMsg = "MQTT连接失败: 已断开连接";
        break;
    case MQTT_CONNECT_BAD_PROTOCOL:
        errorMsg = "MQTT连接失败: 不支持的协议版本";
        break;
    case MQTT_CONNECT_BAD_CLIENT_ID:
        errorMsg = "MQTT连接失败: 不合格的客户端标识符";
        break;
    case MQTT_CONNECT_UNAVAILABLE:
        errorMsg = "MQTT连接失败: 服务器不可用";
        break;
    case MQTT_CONNECT_BAD_CREDENTIALS:
        errorMsg = "MQTT连接失败: 无效的用户名或密码";
        break;
    case MQTT_CONNECT_UNAUTHORIZED:
        errorMsg = "MQTT连接失败: 未授权";
        break;
    default:
        errorMsg = "MQTT连接失败: 未知错误";
        break;
    }
    logger.error(errorMsg, "MQTT");
}

void IoTManager::callback(char *topic, byte *payload, unsigned int length)
{
    // 使用静态缓冲区
    static char payloadBuffer[1024];

    if (length >= sizeof(payloadBuffer))
    {
        logger.error("接收到超大消息负载，无法处理", "MQTT");
        return;
    }

    // 将消息负载添加 null 结尾并复制到缓冲区中
    memcpy(payloadBuffer, payload, length);
    payloadBuffer[length] = '\0';

    logger.info("收到 MQTT 消息 [" + String(topic) + "] " + payloadBuffer, "MQTT");

    // 根据实际负载大小选择合适的静态JSON文档

    JsonDocument doc;
    DeserializationError parseError = deserializeJson(doc, payloadBuffer);

    if (parseError)
    {
        logger.error("JSON 解析失败", "MQTT");
        return;
    }
    JsonVariant jsonVariant = doc.as<JsonVariant>();

    if (strstr(topic, topicPropSet.c_str()))
    {
        processPropertySetMessage(jsonVariant);
    }
    else if (strstr(topic, topicUser.c_str()))
    {
        processUserMessage(topic, jsonVariant);
    }
    else
    {
        processGenericMessage(topic, jsonVariant);
    }
}

void IoTManager::processPropertySetMessage(JsonVariant jsonVariant)
{
    for (const auto &callbackEntry : callbackArray)
    {
        if (!callbackEntry.key.isEmpty())
        {
            bool keyExists = jsonVariant["params"].containsKey(callbackEntry.key.c_str());
            if (keyExists)
            {
                callbackEntry.callbackFn(jsonVariant["params"][callbackEntry.key.c_str()]);
            }
        }
    }
}

void IoTManager::processUserMessage(const char *topic, JsonVariant jsonVariant)
{
    for (const auto &callbackEntry : callbackArray)
    {
        if (!callbackEntry.key.isEmpty() && callbackEntry.key == topic)
        {
            callbackEntry.callbackFn(jsonVariant);
        }
    }
}

void IoTManager::processGenericMessage(const char *topic, JsonVariant jsonVariant)
{
    for (const auto &callbackEntry : callbackArray)
    {
        if (!callbackEntry.key.isEmpty() && callbackEntry.key == topic)
        {
            callbackEntry.callbackFn(jsonVariant);
        }
    }
}

void IoTManager::checkMessageQueue()
{
    for (auto it = messageQueue.begin(); it != messageQueue.end();)
    {
        JsonDocument doc;
        JsonObject root = doc.to<JsonObject>();
        root["id"] = String(timeManager.getTimestamp());
        root["version"] = "1.0";
        root["params"][it->key] = it->value;

        String payloadStr;
        serializeJson(root, payloadStr);

        sendGenericPropetry(payloadStr.c_str());

        // 清空已发送的消息
        it = messageQueue.erase(it);
    }
}

void IoTManager::sendGenericPropetry(String payload)
{
    bool success = mqttClient.publish(topicPropPost.c_str(), payload.c_str(), false);
    logger.info("发送属性 " + topicPropPost + " " + payload, "MQTT");
    if (success)
    {
        logger.info("MQTT属性发送成功: " + String(payload), "MQTT");
    }
    else
    {
        logError();
        logger.error("MQTT属性发送失败: " + String(payload), "MQTT");
    }
}

void IoTManager::checkConnectionCallback(){
    if(IoTManager::instance){
        IoTManager::instance->checkConnection();
    }

}
void IoTManager::checkMessageQueueCallback(){
    if(IoTManager::instance){
        IoTManager::instance->checkMessageQueue();
    }
}
void IoTManager::mqttCallback(char *topic, byte *payload, unsigned int length){
    if(IoTManager::instance){
        IoTManager::instance->callback(topic, payload, length);
    }
}