/**
 * @file Camera.h
 * @author 稀饭
 * @brief 定义了 Camera 类，用于控制摄像头模块的初始化和图像捕获。
 * 
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <esp_camera.h>
#include "Logger.h"

extern Logger logger; ///< 外部定义的日志记录器对象

#define PWDN_GPIO_NUM 32    ///< 摄像头模块的电源控制 GPIO 引脚
#define RESET_GPIO_NUM -1   ///< 摄像头模块的复位 GPIO 引脚
#define XCLK_GPIO_NUM 0     ///< 摄像头模块的时钟信号 GPIO 引脚
#define SIOD_GPIO_NUM 26    ///< 摄像头模块的数据线 GPIO 引脚
#define SIOC_GPIO_NUM 27    ///< 摄像头模块的控制线 GPIO 引脚

#define Y9_GPIO_NUM 35      ///< 摄像头模块的 Y9 数据线 GPIO 引脚
#define Y8_GPIO_NUM 34      ///< 摄像头模块的 Y8 数据线 GPIO 引脚
#define Y7_GPIO_NUM 39      ///< 摄像头模块的 Y7 数据线 GPIO 引脚
#define Y6_GPIO_NUM 36      ///< 摄像头模块的 Y6 数据线 GPIO 引脚
#define Y5_GPIO_NUM 21      ///< 摄像头模块的 Y5 数据线 GPIO 引脚
#define Y4_GPIO_NUM 19      ///< 摄像头模块的 Y4 数据线 GPIO 引脚
#define Y3_GPIO_NUM 18      ///< 摄像头模块的 Y3 数据线 GPIO 引脚
#define Y2_GPIO_NUM 5       ///< 摄像头模块的 Y2 数据线 GPIO 引脚
#define VSYNC_GPIO_NUM 25   ///< 摄像头模块的 VSYNC 信号 GPIO 引脚
#define HREF_GPIO_NUM 23    ///< 摄像头模块的 HREF 信号 GPIO 引脚
#define PCLK_GPIO_NUM 22    ///< 摄像头模块的像素时钟 GPIO 引脚

/**
 * ### 摄像头控制类
 * 
 * 该类负责控制摄像头模块的初始化和图像捕获。
 * 
 * ### 属性
 * 
 * - `config`：摄像头配置信息
 * 
 * ### 方法
 * 
 * - `Camera()`：构造函数
 * - `init()`：初始化摄像头模块
 * - `capture()`：捕获图像
 * 
 */
class Camera {
public:
    /**
     * ### 构造函数
     * 
     * 初始化 Camera 对象。
     */
    Camera();

    /**
     * ### 初始化摄像头模块
     * 
     * 初始化 ESP32 上的摄像头模块。
     * 
     * #### 返回
     * 
     * - esp_err_t：ESP32 系统错误码，ESP_OK 表示成功
     */
    esp_err_t init();

    /**
     * ### 捕获图像
     * 
     * 从摄像头模块捕获图像帧。
     * 
     * #### 返回
     * 
     * - camera_fb_t*：指向捕获到的图像帧的指针
     */
    camera_fb_t* capture();

    /**
     * ### 返回图像帧缓冲区
     * 
     * 释放不再使用的图像帧缓冲区。
     * 
     * #### 参数
     * 
     * - `fb`：要释放的图像帧缓冲区的指针
     */
    void returnFrameBuffer(camera_fb_t *fb);

private:
    camera_config_t config; ///< 摄像头配置信息
};

#endif // CAMERA_H
