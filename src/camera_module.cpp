#include "camera_module.h"
#include <Arduino.h>

esp_err_t init_camera()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;

    // Fix lỗi Warning (sscb -> sccb)
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    // Xung nhịp 10MHz để MÁT MÁY
    config.xclk_freq_hz = 10000000;

    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_SVGA; // 800x600
    config.jpeg_quality = 10;           // 10-12 là đẹp
    config.fb_count = 1;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
        return err;
    sensor_t *s = esp_camera_sensor_get();
    if (s != NULL)
    {
        // 1. Tắt chế độ Auto White Balance Mode (WB Mode)
        // Thay vì để 0 (Auto), ta set thành 2 (Cloudy) hoặc 3 (Office)
        // Mức 3 (Office) sẽ khử mạnh màu xanh lá cây.
        s->set_wb_mode(s, 3);

        // 2. Vẫn bật Auto Gain Control để chỉnh độ sáng
        s->set_awb_gain(s, 1);
        s->set_aec2(s, 1);     // Bật DSP Exposure
        s->set_ae_level(s, 2); // Tăng mức độ sáng lên (+2) để ảnh không bị tối (ảnh tối hay bị xanh)

        // 3. Tăng độ rực màu lên cao để lấn át màu xanh
        s->set_saturation(s, 2);

        // 4. Giảm Contrast một chút để màu mềm hơn, ít bị gắt
        s->set_contrast(s, 0);
    }

    return ESP_OK;
}

camera_fb_t *capture_photo()
{
    Serial.println("Đang chụp ảnh ngay lập tức...");

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
        Serial.println("Lỗi: Không chụp được ảnh");
        return NULL;
    }
    Serial.println("Chụp thành công: " + String(fb->len) + " bytes");
    return fb;
}

// Hàm tắt camera chuẩn (Giảm nhiệt tối đa)
void stop_camera()
{
    sensor_t *s = esp_camera_sensor_get();
    if (s != NULL && s->id.PID == OV5640_PID)
    {
        s->set_reg(s, 0x3008, 0xff, 0x42);
    }
    esp_camera_deinit();
    pinMode(XCLK_GPIO_NUM, OUTPUT);
    digitalWrite(XCLK_GPIO_NUM, LOW);
    Serial.println(">> Camera Sleep");
}

