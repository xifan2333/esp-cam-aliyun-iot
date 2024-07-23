/**
 * @file Camera.cpp
 * @author 稀饭
 * @brief 实现了 Camera 类的方法，包括摄像头模块的初始化和图像捕获功能。
 */

#include "Camera.h"

/**
 * ### 构造函数
 * 
 * 初始化 Camera 对象，并设置摄像头模块的配置参数。
 */
Camera::Camera()
{
    config = (camera_config_t){
        .pin_pwdn = PWDN_GPIO_NUM,
        .pin_reset = RESET_GPIO_NUM,
        .pin_xclk = XCLK_GPIO_NUM,
        .pin_sscb_sda = SIOD_GPIO_NUM,
        .pin_sscb_scl = SIOC_GPIO_NUM,
        .pin_d7 = Y9_GPIO_NUM,
        .pin_d6 = Y8_GPIO_NUM,
        .pin_d5 = Y7_GPIO_NUM,
        .pin_d4 = Y6_GPIO_NUM,
        .pin_d3 = Y5_GPIO_NUM,
        .pin_d2 = Y4_GPIO_NUM,
        .pin_d1 = Y3_GPIO_NUM,
        .pin_d0 = Y2_GPIO_NUM,
        .pin_vsync = VSYNC_GPIO_NUM,
        .pin_href = HREF_GPIO_NUM,
        .pin_pclk = PCLK_GPIO_NUM,
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_QVGA,
        .jpeg_quality = 10,
        .fb_count = 1,
    };
}

/**
 * ### 初始化摄像头模块
 * 
 * 初始化 ESP32 上的摄像头模块，并配置指定的参数。
 * 
 * #### 返回
 * 
 * - esp_err_t：ESP32 系统错误码，ESP_OK 表示成功
 */
esp_err_t Camera::init()
{
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        logger.error("相机初始化失败", "camera");
        return err;
    }

    sensor_t *s = esp_camera_sensor_get();
    if (s->id.PID == OV2640_PID)
    {
        // Camera initialized
    }

    logger.info("相机初始化成功", "camera");
    return ESP_OK;
}

/**
 * ### 捕获图像
 * 
 * 从摄像头模块捕获一帧图像。
 * 
 * #### 返回
 * 
 * - camera_fb_t*：指向捕获到的图像帧的指针，如果捕获失败，则返回 nullptr
 */
camera_fb_t *Camera::capture()
{
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
        logger.error("获取图像失败", "camera");
        return nullptr;
    }

    logger.info("获取图像成功", "camera");
    return fb;
}

/**
 * ### 返回图像帧缓冲区
 * 
 * 释放不再使用的图像帧缓冲区。
 * 
 * #### 参数
 * 
 * - `fb`：要释放的图像帧缓冲区的指针
 */
void Camera::returnFrameBuffer(camera_fb_t *fb)
{
    esp_camera_fb_return(fb);
}
