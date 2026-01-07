#include "firebase.h"
#include "wifi_module.h"
#include "camera_module.h"
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// [MỚI] Biến lưu thời gian của lệnh chụp cuối cùng
double last_trigger_time = 0;

void init_firebase()
{
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;
    fbdo.setResponseSize(4096);
    Serial.println("Initializing Firebase...");
    Firebase.begin(&config, &auth);
    Serial.println("Firebase initialized");
    Firebase.reconnectWiFi(true);

    // --- [CODE MỚI THÊM VÀO] ---
    // Mục đích: Đồng bộ thời gian ngay khi khởi động để tránh chụp oan
    Serial.println("Đang đồng bộ trạng thái server...");

    // Thử tối đa 5 lần để lấy thời gian hiện tại
    int retries = 0;
    while (retries < 5)
    {
        if (Firebase.ready())
        {
            if (Firebase.RTDB.getDouble(&fbdo, "/DieuKhien/thoi_gian_trigger"))
            {
                if (fbdo.dataType() == "double" || fbdo.dataType() == "float" || fbdo.dataType() == "int" || fbdo.dataType() == "integer")
                {
                    // Cập nhật mốc thời gian ngay lập tức
                    last_trigger_time = fbdo.doubleData();
                    Serial.println(">> Đã đồng bộ xong! Mốc thời gian: " + String(last_trigger_time, 0));
                    break; // Thoát vòng lặp khi thành công
                }
            }
        }
        retries++;
        delay(500); // Đợi một chút rồi thử lại
    }
    // ---------------------------
}

// Hàm gửi ảnh: Nhận đầu vào là dữ liệu ảnh (fb)
void send_photo_to_firebase(camera_fb_t *fb)
{
    if (!fb)
    {
        Serial.println("Lỗi: Không có dữ liệu ảnh để gửi.");
        return;
    }

    if (!Firebase.ready())
    {
        Serial.println("Lỗi: Mất kết nối Firebase.");
        esp_camera_fb_return(fb);
        return;
    }

    Serial.println("Bắt đầu upload lên Firebase...");

    // 2. Tạo tên file (Dùng millis để tránh trùng tên)
    String fileName = "Data/" + String(millis()) + ".jpg";

    // 3. Upload
    if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, fb->buf, fb->len, fileName, "image/jpeg"))
    {
        String downloadURL = fbdo.downloadURL();
        Serial.println("Link ảnh: " + downloadURL);

        // 4. Cập nhật Database để báo Server Python biết
        FirebaseJson json;
        json.set("image_url", downloadURL);
        json.set("status", "waiting"); // Server Python sẽ bắt cái này

        if (Firebase.RTDB.updateNode(&fbdo, "/KiemTraBenh", &json))
        {
            Serial.println("Đã cập nhật Database thành công!");
            // [QUAN TRỌNG] Không cần reset lệnh chụp nữa!
        }
        else
        {
            Serial.println("Lỗi Database: " + fbdo.errorReason());
        }
    }
    else
    {
        Serial.println("Lỗi Upload: " + fbdo.errorReason());
    }

    // 5. Giải phóng bộ nhớ Camera
    esp_camera_fb_return(fb);
    Serial.println("Đã giải phóng bộ nhớ camera.");
}

// [ĐÃ SỬA] Hàm kiểm tra lệnh chụp theo Timestamp
bool check_capture_command()
{
    if (Firebase.ready())
    {
        // Đọc giá trị từ đường dẫn mới khớp với Flutter
        if (Firebase.RTDB.getDouble(&fbdo, "/DieuKhien/thoi_gian_trigger"))
        {
            // Kiểm tra kiểu dữ liệu trả về có phải số không
            if (fbdo.dataType() == "double" || fbdo.dataType() == "float" || fbdo.dataType() == "int" || fbdo.dataType() == "integer")
            {
                double current_time = fbdo.doubleData();

                // Nếu thời gian mới > thời gian đã xử lý -> Có lệnh mới
                if (current_time > last_trigger_time)
                {
                    // Cập nhật ngay lập tức để không chụp lại
                    last_trigger_time = current_time;
                    return true;
                }
            }
        }
    }
    return false;
}