#include "wifi_module.h"
#include <WiFi.h>
#include <WiFiManager.h> // Đừng quên include thư viện này
#include "oled_module.h"

void configModeCallback(WiFiManager *myWiFiManager)
{
    Serial.println("Vào chế độ cấu hình AP");
    // Hiện lên OLED để người dùng biết
    show_status("Cau hinh Wifi: IP", "192.168.4.1");
}

void init_wifi()
{
    WiFiManager wm;

    // Cài đặt callback để báo lên màn hình
    wm.setAPCallback(configModeCallback);

    // Tăng timeout lên 180s cho thoải mái
    wm.setConfigPortalTimeout(180);

    // Báo lên màn hình đang kết nối
    show_status("WiFi...", "Connecting");

    // Tự động kết nối hoặc tạo AP tên "ESP32-Camera-Setup"
    bool res = wm.autoConnect("ESP32-Camera-Setup");

    if (!res)
    {
        show_status("Loi WiFi", "Restarting...");
        delay(3000);
        ESP.restart();
    }
    else
    {
        // Kết nối thành công, hiện IP
        String ipAddr = WiFi.localIP().toString();
        show_status("WiFi OK", ipAddr);
        delay(2000); // Đợi 2s cho người dùng kịp nhìn IP
    }
}