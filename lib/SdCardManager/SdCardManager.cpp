#include "SdCardManager.h"

/**
 * ### 初始化SD卡管理器
 * 
 * 此函数用于初始化SD卡管理器，并挂载SD卡。如果挂载失败或者SD卡类型错误，将会记录错误日志。
 * 
 *  在调用其他SD卡管理器函数之前，需要先调用此函数进行初始化。
 */
void SdCardManager::init()
{
    if (!SD_MMC.begin())
    {
        logger.error("内存卡挂载失败", "sdcard");
        return;
    }
    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE)
    {
        logger.error("内存卡类型错误", "sdcard");
        return;
    }

    logger.info("内存卡挂载成功", "sdcard");
}

/**
 * ### 检查目录是否存在，如果不存在则创建目录。
 * 
 * #### 参数
 * 
 * - `dir` 目录路径
 */
void SdCardManager::checkDirExists(const String &dir)
{
    if (!SD_MMC.exists(dir))
    {
        logger.info("创建目录 " + dir, "sdcard");
        if (SD_MMC.mkdir(dir))
        {
            logger.info("创建目录 " + dir + " 成功", "sdcard");
        }
        else
        {
            logger.error("创建目录 " + dir + " 失败", "sdcard");
        }
    }
}

/**
 * ### 保存图像到SD卡。
 * 
 * #### 参数
 * 
 * - `fb` 图像帧缓冲区
 */
void SdCardManager::saveImage(camera_fb_t *fb)
{
    if (!fb)
    {
        logger.error("保存图像失败：帧缓冲区为空", "sdcard");
        return;
    }

    checkDirExists("/pictures");

    String filename = "/pictures/image_" + String(timeManager.getFormattedDateAndTime())+ ".jpg";

    File file = SD_MMC.open(filename, FILE_WRITE);
    if (!file)
    {
        logger.error("无法打开文件 " + filename + " 进行写入", "sdcard");
        return;
    }

    u_int8_t written = file.write(fb->buf, fb->len);

    if (written != fb->len)
    {
        logger.error("写入文件 " + filename + " 时发生错误", "sdcard");
    }
    else
    {
        logger.info("图像保存成功: " + filename, "sdcard");
    }

    file.close();
}
