#include "SdCardManager.h"

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

void SdCardManager::saveImage(camera_fb_t *fb)
{
    if (!fb)
    {
        logger.error("保存图像失败：帧缓冲区为空", "sdcard");
        return;
    }

    checkDirExists("/images");

    String filename = "/images/image_" + String(millis()) + ".jpg";

    File file = SD_MMC.open(filename, FILE_WRITE);
    if (!file)
    {
        logger.error("无法打开文件 " + filename + " 进行写入", "sdcard");
        return;
    }

    file.write(fb->buf, fb->len);

    if (file.size() != fb->len)
    {
        logger.error("写入文件 " + filename + " 时发生错误", "sdcard");
    }
    else
    {
        logger.info("图像保存成功: " + filename, "sdcard");
    }

    file.close();
}
