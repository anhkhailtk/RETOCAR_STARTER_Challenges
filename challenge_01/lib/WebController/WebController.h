#ifndef WEB_CONTROLLER_H
#define WEB_CONTROLLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "Config.h"
#include "MotorController.h"
#include "SirenController.h"

class WebController {
public:
    // Constructor mặc định (Không truyền con trỏ nữa)
    WebController();

    // Khởi tạo Wi-Fi và các đường dẫn Web
    void begin();

    // Gọi liên tục trong loop() để xử lý các request từ điện thoại
    void update();

private:
    WebServer server;

    // Các biến lưu trạng thái điều khiển từ Web
    int speedRatioLeft = 100;
    int speedRatioRight = 100;
    String currentDirection = "STOP";

    // Khai báo các hàm xử lý request (endpoint)
    void handleRoot();
    void handleNotFound();
    void handleToggleSiren();
    void handleToggleLight();
    void handleCmd();
    void handleSpeed();

    // Khai báo giao diện HTML (PROGMEM để tiết kiệm RAM)
    static const char index_html[] PROGMEM;
};

#endif // WEB_CONTROLLER_H
