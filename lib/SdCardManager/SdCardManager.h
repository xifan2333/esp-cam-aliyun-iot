/**
 * @file SdCardManager.h
 * @author 稀饭
 * @brief 用于管理内存卡，包含初始化、检测是否存在、写入图片
 */

#ifndef SDCARDMANAGER_H
#define SDCARDMANAGER_H

#include <Arduino.h>
#include <SD_MMC.h>
#include <esp_camera.h>
#include "Logger.h"
#include "TimeManager.h"

extern TimeManager timeManager;
extern Logger logger;
/**
 * ### 内存卡管理类
 * 
 * #### 方法
 * 
 * - `init()` 初始化内存卡
 * - `checkDirExists(const String& dir)` 检查目录是否存在，不存在则创建
 * - `saveImage(camera_fb_t *fb)` 保存图片到内存卡
 */
class SdCardManager {
public:
    void init();
    void checkDirExists(const String& dir);
    void saveImage(camera_fb_t *fb);

};

#endif // SDCARDMANAGER_H