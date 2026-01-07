#include "oled_module.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void init_oled()
{
    // Khởi tạo I2C
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("❌ Lỗi OLED!"));
        return;
    }

    display.clearDisplay();
    display.setTextColor(WHITE);
    // Tắt chế độ tự xuống dòng để mình tự kiểm soát
    display.setTextWrap(false);

    // Test
    show_status("HE THONG", "Khoi dong...");
}

// Hàm hỗ trợ căn giữa văn bản
void printCenteredText(String text, int y, int size)
{
    int16_t x1, y1;
    uint16_t w, h;

    display.setTextSize(size);
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    // Nếu chữ quá dài so với màn hình (128px), tự động giảm size
    if (w > SCREEN_WIDTH && size > 1)
    {
        size = 1;
        display.setTextSize(size);
        display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    }

    // Tính toạ độ X để căn giữa
    int16_t x = (SCREEN_WIDTH - w) / 2;

    // Đảm bảo không bị lẹm ra ngoài lề trái
    if (x < 0)
        x = 0;

    display.setCursor(x, y);
    display.print(text);
}

void show_status(String title, String status)
{
    display.clearDisplay();

    // --- PHẦN 1: HEADER (Thanh tiêu đề) ---
    // Vẽ hình chữ nhật màu trắng ở trên cùng
    display.fillRect(0, 0, SCREEN_WIDTH, 18, WHITE);

    // In tiêu đề màu ĐEN nằm trên nền TRẮNG
    display.setTextColor(BLACK);
    printCenteredText(title, 5, 1); // Căn giữa tiêu đề

    // --- PHẦN 2: NỘI DUNG ---
    display.setTextColor(WHITE); // Chuyển lại màu chữ trắng

    // Tự động chọn cỡ chữ cho nội dung
    // Nếu nội dung ngắn (<10 ký tự) thì dùng chữ to (Size 2)
    // Nếu dài (như IP hoặc thông báo lỗi) thì dùng chữ nhỏ (Size 1)
    int contentSize = (status.length() <= 10) ? 2 : 1;

    // Căn chỉnh vị trí Y tuỳ theo cỡ chữ cho cân đối
    int yPos = (contentSize == 2) ? 35 : 40;

    printCenteredText(status, yPos, contentSize);

    // Vẽ đường kẻ ngang trang trí dưới chân (tuỳ chọn)
    display.drawFastHLine(10, 55, 108, WHITE);

    display.display();
}

void clear_screen()
{
    display.clearDisplay();
    display.display();
}