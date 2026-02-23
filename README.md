## ⚙️ Kiến trúc hệ thống

Hệ thống được thiết kế theo mô hình **Client - Edge Server - Cloud** để tối ưu hóa băng thông và tài nguyên xử lý:

1. **Thu thập dữ liệu (ESP32-CAM):** Thiết bị liên tục giám sát và chụp ảnh lá cây theo chu kỳ hoặc khi có tín hiệu trigger.
2. **Truyền tải (HTTP/Local Network):** Ảnh được nén và gửi qua giao thức HTTP POST về Edge Server nội bộ (Raspberry Pi / PC).
3. **Xử lý AI (Edge Server - Python):**
   - Server nhận ảnh, tiền xử lý bằng OpenCV.
   - Đưa ảnh qua mô hình Deep Learning (ví dụ: YOLO, PyTorch / .tflite) để nhận diện vùng lá bệnh và phân loại bệnh.
4. **Lưu trữ Cloud (Firebase):**
   - **Firebase Storage:** Lưu trữ hình ảnh gốc và hình ảnh đã được vẽ bounding box.
   - **Firebase Realtime Database / Firestore:** Cập nhật trạng thái cây trồng, loại sâu bệnh phát hiện, độ tin cậy (confidence score) và timestamp.
5. **Giám sát trực quan (Mobile App):** Ứng dụng di động (Flutter) lắng nghe thay đổi từ Firebase, hiển thị cảnh báo thời gian thực và lịch sử tình trạng cây trồng.

---

## 🚀 Tính năng nổi bật

- **Nhận diện chính xác cao:** Ứng dụng mô hình Computer Vision (YOLO) tiên tiến để phát hiện sớm các loại bệnh phổ biến trên lá cây.
- **Xử lý tại biên (Edge Computing):** Việc chạy mô hình AI trên Local Server (như Raspberry Pi) giúp giảm thiểu độ trễ, tiết kiệm băng thông mạng và đảm bảo hệ thống vẫn hoạt động nhận diện ngay cả khi kết nối internet chập chờn.
- **Cảnh báo thời gian thực:** Dữ liệu được đồng bộ ngay lập tức lên Cloud (Firebase), cho phép theo dõi mọi lúc, mọi nơi qua Mobile App.
- **Dễ dàng mở rộng:** Kiến trúc tách biệt rõ ràng giữa Hardware (C/C++), Backend/AI (Python) và Database cho phép dễ dàng thêm các module mới (như điều khiển bơm tưới tiêu tự động).

---

## 🧠 Chi tiết mô hình AI (Computer Vision)

- **Framework sử dụng:** PyTorch / TensorFlow Lite (Tối ưu hóa cho thiết bị Edge như Raspberry Pi).
- **Thư viện xử lý ảnh:** OpenCV.
- **Luồng xử lý (Pipeline):**
  `Raw Image -> Resize & Normalize -> AI Model Inference -> Bounding Box & Labeling -> Export Result`

---

## 📱 Ứng dụng quản lý (Smath_tree - Sắp ra mắt)

Hệ thống được tích hợp liền mạch với ứng dụng di động quản lý trang trại thông minh:
- **Nền tảng:** Flutter (Hỗ trợ cả iOS & Android).
- **Kiến trúc App:** Clean Architecture kết hợp State Management bằng Riverpod, quản lý routing bằng GoRouter.
- **Tính năng chính:** Xem ảnh cây trồng trực tiếp, nhận notification khi phát hiện sâu bệnh, xem biểu đồ thống kê tình trạng sức khỏe của vườn.

---

## 💻 Cài đặt & Chạy thử nghiệm

### 1. Cấu hình ESP32
- Mở source code bằng Arduino IDE hoặc PlatformIO.
- Thay đổi thông tin `WIFI_SSID`, `WIFI_PASSWORD` và `SERVER_URL` (IP của Raspberry Pi / PC).
- Nạp firmware cho board ESP32-CAM.

### 2. Cài đặt Python Server & AI
```bash
# Clone repository
git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
cd your-repo-name/server

# Cài đặt các thư viện cần thiết (OpenCV, PyTorch, Firebase Admin...)
pip install -r requirements.txt

# Thêm file Firebase Admin SDK JSON vào thư mục config
# Chạy server
python main.py
```

### 3. Cấu hình Firebase
- Tạo project mới trên Firebase Console.
- Bật Realtime Database và Storage.
- Cấp quyền truy cập trong `Rules` và lấy file JSON service account cấp cho Python Server.

---

## 🤝 Đóng góp (Contributing)

Mọi ý tưởng đóng góp để cải thiện độ chính xác của mô hình AI hoặc tối ưu hóa luồng truyền dữ liệu đều được hoan nghênh. Vui lòng tạo Pull Request hoặc mở Issue để thảo luận.
