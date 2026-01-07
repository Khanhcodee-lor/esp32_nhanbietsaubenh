#include <Arduino.h>
#include "camera_module.h"
#include "wifi_module.h"
#include "firebase.h"
#include "oled_module.h" // Nhớ include module OLED

void setup()
{
    Serial.begin(115200);

    // 1. Khởi động OLED đầu tiên
    init_oled();
    show_status("HE THONG", "Dang khoi dong");
    delay(1000);

    // 2. Kết nối WiFi (Màn hình sẽ hiện IP ở trong hàm này)
    init_wifi();

    // 3. Kết nối Firebase
    show_status("KET NOI", "Firebase...");
    init_firebase();

    stop_camera();

    Serial.println("✅ Hệ thống sẵn sàng...");
    // Trạng thái chờ mặc định
    show_status("SAN SANG", "Cho lenh...");
}

void loop()
{
    if (Firebase.ready())
    {
        // Kiểm tra lệnh chụp
        if (check_capture_command())
        {
            Serial.println("📸 Phát hiện lệnh chụp mới!");

            // --- [THÊM MỚI] Báo đã nhận lệnh ---
            show_status("THONG BAO", "Da nhan lenh!");
            delay(2000); // Dừng 2s để bạn kịp nhìn thấy dòng chữ này

            // Báo đang bật camera
            show_status("CAMERA", "Dang bat...");

            if (init_camera() == ESP_OK)
            {
                // Chụp ảnh
                show_status("CAMERA", "Dang chup...");
                delay(1500); // Chờ cân bằng sáng

                camera_fb_t *fb = capture_photo();

                if (fb != NULL)
                {
                    // Báo đang gửi ảnh
                    show_status("UPLOAD", "Dang gui anh...");

                    // Hàm này sẽ gửi ảnh và update database
                    send_photo_to_firebase(fb);

                    // --- [THÊM MỚI] Báo đã gửi xong cho Server Python ---
                    show_status("SERVER", "Da gui xu ly!");
                    delay(3000); // Giữ thông báo này lâu chút (3s) để biết là đã xong
                }
                else
                {
                    show_status("LOI", "Khong chup duoc");
                    delay(2000);
                }

                // Tắt camera
                stop_camera();
                Serial.println("zzz Đã tắt Camera.");

                // Quay về trạng thái chờ
                show_status("SAN SANG", "Cho lenh...");

                // (Tùy chọn) Tắt màn hình sau 5 giây để tiết kiệm điện
                // delay(5000);
                // clear_screen();
            }
            else
            {
                show_status("LOI", "Loi Camera!");
                delay(2000);
            }
        }
    }

    // Giảm delay loop xuống để phản hồi nhanh hơn
    delay(100);
}