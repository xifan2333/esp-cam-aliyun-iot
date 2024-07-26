#ifndef QINIU_CLIENT_H
#define QINIU_CLIENT_H

#include <HTTPClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <mbedtls/md.h>
#include "_Base64.h"
#include "Logger.h"
#include "TimeManager.h"


extern WiFiClient wifiClient;
extern Logger logger;
extern _Base64 _base64;
extern TimeManager timeManager;

class QiniuClient:public HTTPClient{

    public:
        String  accessKey;
        String secretKey;
        String bucketName;
        String domain;
        String zone;
        QiniuClient(String accessKey, String secretKey, String bucketName, String domain,String zone);
       
        String uploadImage(String imageName,const uint8_t *imageData, size_t imageLength);
    private:
        String uploadHost;
        String uploadToken;
        String generateUploadToken(String policy);
        String generateUploadPolicy(String imageName);
        String generateBoundary();
};
#endif