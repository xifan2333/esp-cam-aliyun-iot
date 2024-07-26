

#include "WifiManager.h"
#include "IoTManager.h"
#include "TimeManager.h"
#include "SdCardManager.h"
#include "Camera.h"
#include "_Base64.h"
#include "Logger.h"
#include "QiniuClient.h"



Ticker ticker;
Camera camera;
_Base64 _base64;
Logger logger;
WiFiClient wifiClient;
PubSubClient mqttClient;
SdCardManager sdcardManager;
TimeManager timeManager;
WifiManager wifiManager("Tenda_2344E0","lvjiang516116");
IoTManager iotManager("k1jf1H5lHO8","ESPcam","a5c9dadff635870d067233b08ab66c3e","iot-06z00j81cbwhmp9.mqtt.iothub.aliyuncs.com",1883);
QiniuClient qiniuClient("-FrVRtN6n86rbnw6iwLF8SZHHJ8mv2NNJNtNYYIL","6XraLGydOPzTtG3Yrs65e4VKPu2X7M-oXUg7PvDu","storage-fan","https://storage.xifan.fun","z0");



void setup()
{
  Serial.begin(115200);
  wifiManager.connect();
  if (wifiManager.checkConnection()==true)
  {
    timeManager.updateTime();
    String timestamp = String(timeManager.getTimestamp());
    iotManager.connect();
  }
  sdcardManager.init();
  camera.init();

}


void loop()
{
  camera_fb_t*  image = camera.capture();
  sdcardManager.saveImage(image);
  String imageName = "image" + String(timeManager.getTimestamp()) + ".jpg";
  String url = qiniuClient.uploadImage(imageName,image->buf,image->len);
  if(url!="")
  {
    iotManager.sendProperty("img",url);
  }
  else
  {
    iotManager.sendProperty("img","error");
  }
  camera.returnFrameBuffer(image);
  delay(1000);
}
