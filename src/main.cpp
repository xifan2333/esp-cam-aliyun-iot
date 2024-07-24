

#include "WifiManager.h"
#include "IoTManager.h"
#include "TimeManager.h"
#include "SdCardManager.h"
#include "Camera.h"
#include "Base64.h"
#include "Logger.h"
#include <Ticker.h>


Ticker ticker;
Camera camera;
Base64 base64;
Logger logger;
WiFiClient wifiClient;
PubSubClient mqttClient;
SdCardManager sdcardManager;
TimeManager timeManager;
WifiManager wifiManager("Tenda_2344E0","lvjiang516116");
IoTManager iotManager("k1jf1H5lHO8","ESPcam","a5c9dadff635870d067233b08ab66c3e","iot-06z00j81cbwhmp9.mqtt.iothub.aliyuncs.com",1883);




void setup()
{
  Serial.begin(115200);
  wifiManager.connect();
  if (wifiManager.checkConnection()==true)
  {
    timeManager.updateTime();
    String timestamp = String(timeManager.getTimestamp());
    iotManager.connect();
    ticker.attach(10, []() { iotManager.loop(); });
  }else{
    logger.error("WIFI连接失败","WiFi");
  }
  sdcardManager.init();
  camera.init();

}


void loop()
{
  camera_fb_t*  image = camera.capture();
  sdcardManager.saveImage(image);
  String imageBase64 = base64.encode(image->buf, image->len);
  camera.returnFrameBuffer(image);
  delay(1000);
}
