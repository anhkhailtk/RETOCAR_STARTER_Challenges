#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==========================================
// TÙY CHỈNH MẠNG WI-FI
// ==========================================
// Tên và mật khẩu Wi-Fi mà xe sẽ phát ra. Sửa tùy ý bạn nhé!
const char* const WIFI_SSID = "ROBOT_CONTROLLER";
const char* const WIFI_PASS = "12345678";

// ==========================================
// TÙY CHỈNH CHÂN CẮM (PIN MAP)
// ==========================================
// Bạn có thể sửa đổi các chân này nếu nối mạch khác đi.

// Nút nhấn cứng trên board
const int PIN_BTN_START = 34;

// Còi báo động (Còi chíp)
const int PIN_BUZZER = 15;

// Đèn pha trái / phải
const int PIN_LED_LEFT = 2; 
const int PIN_LED_RIGHT = 4;

// Đèn LED RGB (Nháy 3 màu). 
// Chú ý: Code đang viết theo chuẩn Chung Dương (Common Anode), tức là HIGH = Tắt, LOW = Bật
const int PIN_LED_R = 27;
const int PIN_LED_G = 14;
const int PIN_LED_B = 13;

// Động cơ (Mạch MX1508)
// -- Bánh Trái (Motor A)
const int PIN_MA1 = 26; // IN1
const int PIN_MA2 = 25; // IN2
// -- Bánh Phải (Motor B)
const int PIN_MB1 = 33; // IN3
const int PIN_MB2 = 32; // IN4

// ==========================================
// TÙY CHỈNH THÔNG SỐ XUNG PWM ĐỘNG CƠ & CÒI
// ==========================================
// Kênh LEDC của ESP32 (0-15). Đã tránh xung đột nên HẠN CHẾ THAY ĐỔI
const int CH_MA1 = 0;
const int CH_MA2 = 1;
const int CH_MB1 = 5;
const int CH_MB2 = 9;
const int CH_BUZZ = 8; 

// Tần số và độ phân giải PWM
const int PWM_FREQ = 5000; // Tần số 5000Hz phù hợp cho MX1508
const int PWM_RES  = 10;   // 10-bit => Tốc độ chia thành 1024 mức (0-1023)
const int PWM_MAX  = 1023; // Tốc độ tối đa

#endif // CONFIG_H
