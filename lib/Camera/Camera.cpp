#include "Camera.h"

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

void Camera::returnFrameBuffer(camera_fb_t *fb)
{
    esp_camera_fb_return(fb);
}