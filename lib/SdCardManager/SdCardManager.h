#ifndef SDCARDMANAGER_H
#define SDCARDMANAGER_H

#include <Arduino.h>
#include <SD_MMC.h>
#include <esp_camera.h>
#include "Logger.h"

extern Logger logger;

class SdCardManager {
public:
    void init();
    void checkDirExists(const String& dir);
    void saveImage(camera_fb_t *fb);

};

#endif // SDCARDMANAGER_H