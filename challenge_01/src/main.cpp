#include <Arduino.h>
#include "MotorController.h"
#include "SirenController.h"
#include "WebController.h"

// Biến motor và siren đã được khởi tạo tự động nhờ từ khóa extern trong các thư viện

// Khởi tạo WebController (không truyền con trỏ)
WebController web;

void setup() {
    Serial.begin(115200);
    Serial.println("\n[ESP32] Khởi động hệ thống xe điều khiển OOP (No Pointers)...");

    // 1. Khởi tạo động cơ
    motor.begin();

    // 2. Khởi tạo còi, đèn chớp, LED RGB
    siren.begin();

    // 3. Khởi tạo Wi-Fi và Web Server
    web.begin();

    Serial.println("[ESP32] Hệ thống đã sẵn sàng!");
}

void loop() {
    // Duy trì hoạt động của máy chủ Web (nhận lệnh từ điện thoại)
    web.update();

    // Duy trì các hiệu ứng nháy đèn, hú còi, và đọc nút nhấn vật lý
    siren.update();
}