# 🌿 AIoT – Hệ thống phát hiện bệnh cây trồng (ESP32 Edge Node)

![ESP32](https://img.shields.io/badge/ESP32-S3-blue)
![Language](https://img.shields.io/badge/Ngôn_ngữ-C%2FC%2B%2B-orange)
![Firebase](https://img.shields.io/badge/Firebase-RTDB%20%7C%20Storage-yellow)

## 📌 Giới thiệu
Repository này chứa mã nguồn cho **thiết bị ESP32 Edge Node** trong hệ thống **AIoT giám sát và nhận diện bệnh cây trồng**.

ESP32-S3 đóng vai trò là thiết bị thu thập dữ liệu tại hiện trường, chịu trách nhiệm:
- Nhận lệnh điều khiển từ xa
- Chụp ảnh cây trồng
- Đồng bộ dữ liệu lên Cloud
- Hiển thị trạng thái hệ thống qua màn hình OLED

Hệ thống được xây dựng theo kiến trúc phân tán, trong đó ESP32 giao tiếp với **Server AI (Python)** và **Ứng dụng di động (Flutter)** thông qua **Firebase**.

---

## 🧠 Kiến trúc hệ thống
Luồng hoạt động của ESP32 Edge Node:

1. **Ứng dụng Flutter**
   - Người dùng gửi lệnh kiểm tra cây trồng
   - Lệnh và thời gian được đẩy lên Firebase Realtime Database

2. **ESP32 Edge Node (repo này)**
   - Lắng nghe thay đổi trên RTDB
   - Kích hoạt camera và chụp ảnh
   - Upload ảnh lên Firebase Storage
   - Cập nhật trạng thái hệ thống thành `waiting`

3. **Server AI (Python)**
   - Phát hiện trạng thái `waiting`
   - Tải ảnh từ Cloud
   - Phân tích bệnh bằng mô hình AI
   - Gửi kết quả về Firebase cho ứng dụng hiển thị

---

## ✨ Tính năng chính
- **Thiết kế Edge AI ổn định**
  - Tối ưu hoạt động camera, hạn chế quá nhiệt
  - Cơ chế tắt camera sau khi chụp
  - Cân chỉnh chất lượng ảnh phục vụ AI

- **Quản lý WiFi thông minh**
  - Tích hợp WiFiManager với Captive Portal
  - Không cần hardcode thông tin WiFi

- **Tích hợp Cloud (Firebase)**
  - Realtime Database cho điều khiển
  - Firebase Storage cho lưu trữ ảnh
  - Kiểm tra timestamp tránh lặp lệnh

- **Giao diện cục bộ**
  - Hiển thị trạng thái hệ thống qua OLED SSD1306
  - UI module hóa, tự động căn chỉnh nội dung

- **Cấu trúc code rõ ràng**
  - Module hóa:
    - `camera_module`
    - `firebase_module`
    - `wifi_module`
    - `oled_module`
  - Dễ bảo trì và mở rộng

---

## 🧰 Yêu cầu phần cứng
- **Vi điều khiển:** ESP32-S3 (WROOM / Freenove)
- **Camera:** OV5640
- **Màn hình:** OLED SSD1306 0.96" (I2C)
- **Nguồn:** 5V / 2A

---

## 🔌 Sơ đồ chân kết nối

| Thành phần | Chân ESP32-S3 | Chức năng |
|----------|---------------|----------|
| OLED SDA | GPIO 8 | I2C Data |
| OLED SCL | GPIO 9 | I2C Clock |
| Camera SIOD | GPIO 4 | SCCB Data |
| Camera SIOC | GPIO 5 | SCCB Clock |
| Camera Data | Nhiều GPIO | Khai báo trong `camera_pins.h` |

---

## 📚 Thư viện sử dụng
- `esp_camera.h`
- `Firebase_ESP_Client` (Mobizt)
- `WiFiManager`
- `Adafruit_SSD1306`
- `Adafruit_GFX`

---



👨‍💻 Tác giả
Nguyễn Quốc Khánh

🎯 Lĩnh vực quan tâm: IoT, Embedded Systems, Ứng dụng AI.

🔗 GitHub: @Khanhcodee-lor
