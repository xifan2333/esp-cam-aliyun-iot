#include "QiniuClient.h"

QiniuClient::QiniuClient(String accessKey, String secretKey, String bucketName, String domain, String zone)
    : HTTPClient(), accessKey(accessKey), secretKey(secretKey), bucketName(bucketName), domain(domain)
{
    this->uploadToken = "";
    if (zone == "z0" || zone == "华东")
    {
        uploadHost = "upload.qiniup.com";
    }
    else if (zone == "cn-east-2" || zone == "华东-浙江2")
    {
        uploadHost = "upload-cn-east-2.qiniup.com";
    }
    else if (zone == "z1" || zone == "华北")
    {
        uploadHost = "upload-z1.qiniup.com";
    }
    else if (zone == "z2" || zone == "华南")
    {
        uploadHost = "upload-z2.qiniup.com";
    }
    else if (zone == "na0" || zone == "北美")
    {
        uploadHost = "upload-na0.qiniup.com";
    }
    else if (zone == "as0" || zone == "东南亚")
    {
        uploadHost = "upload-as0.qiniup.com";
    }
    else if (zone == "ap-southeast-2" || zone == "河内")
    {
        uploadHost = "upload-ap-southeast-2.qiniup.com";
    }
    else if (zone == "ap-southeast-3" || zone == "胡志明")
    {
        uploadHost = "upload-ap-southeast-3.qiniup.com";
    }
    else
    {
        logger.error("空间代码错误", "Qiniu");
        uploadHost = "";
    }
}

String QiniuClient::generateUploadToken(String policy)
{
    String urlSafePolicy = _base64.urlSafeEncode(policy);
    const uint8_t *key = (const uint8_t *)this->secretKey.c_str();
    size_t key_len = this->secretKey.length();
    uint8_t resultArray[20]; 
    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, md_info, 1);
    mbedtls_md_hmac_starts(&ctx, key, key_len);
    mbedtls_md_hmac_update(&ctx, (const uint8_t *)urlSafePolicy.c_str(), urlSafePolicy.length());
    mbedtls_md_hmac_finish(&ctx, resultArray);
    mbedtls_md_free(&ctx);
    String encodeSign = _base64.urlSafeEncode(resultArray,sizeof(resultArray));
    this->uploadToken = this->accessKey + ":" + encodeSign + ":" + urlSafePolicy;
    return this->uploadToken;
}
String QiniuClient::generateUploadPolicy(String imageName)
{
    JsonDocument policy;
    policy["scope"] = this->bucketName + ":" + imageName;
    policy["deadline"] = timeManager.getTimestamp() + 3600;
    policy["isPrefixalScope"] = 1;
    policy["returnBody"] = "{\"name\":\"$(fname)\",\"url\":\"" + this->domain + "/$(key)\"}";
    String policyString = "";
    serializeJson(policy, policyString);
    return policyString;
}

String QiniuClient::generateBoundary() {
    String boundary = "----WebKitFormBoundary";
    for (int i = 0; i < 16; i++) {
        boundary += String((char)random(97, 122)); // a-z的随机字符
    }
    return boundary;
}

String QiniuClient::uploadImage(String imageName, const uint8_t *imageData, size_t imageLength)
{
    String boundary = generateBoundary();
    String token = generateUploadToken(generateUploadPolicy(imageName));

    // 构建表单的头部部分
    String head = "--" + boundary + "\r\n"
                                    "Content-Disposition: form-data; name=\"key\"\r\n\r\n" +
                  imageName + "\r\n"
                              "--" +
                  boundary + "\r\n"
                             "Content-Disposition: form-data; name=\"token\"\r\n\r\n" +
                  token + "\r\n"
                          "--" +
                  boundary + "\r\n"
                             "Content-Disposition: form-data; name=\"file\"; filename=\"" +
                  imageName + "\"\r\n"
                              "Content-Type: application/octet-stream\r\n\r\n";

    // 构建表单的尾部部分
    String tail = "\r\n--" + boundary + "--\r\n";

    // 创建完整的请求体
    size_t fullBodyLength = head.length() + imageLength + tail.length();
    uint8_t *fullBody = new uint8_t[fullBodyLength];

    // 将头部数据复制到请求体
    memcpy(fullBody, head.c_str(), head.length());
    // 将文件数据复制到请求体
    memcpy(fullBody + head.length(), imageData, imageLength);
    // 将尾部数据复制到请求体
    memcpy(fullBody + head.length() + imageLength, tail.c_str(), tail.length());

    String url = "http://" + uploadHost;

    this->begin(url);
    this->addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);
    this->addHeader("Content-Length", String(fullBodyLength));

    // 发送完整的请求体
    int httpCode = this->POST(fullBody, fullBodyLength);

    // 清理分配的内存
    delete[] fullBody;

    // 获取响应并处理
    String response = this->getString();
    this->end();

    if (httpCode == HTTP_CODE_OK) {
       JsonDocument doc;
        deserializeJson(doc, response);
        String url = doc["url"].as<String>();
        logger.info("上传成功: " + url, "Qiniu");
        return url;
    } else {
        logger.error("上传失败: " + response, "Qiniu");
        return "";
    }
}