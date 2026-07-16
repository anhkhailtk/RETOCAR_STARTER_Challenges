#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "Config.h"

class MotorController {
public:
    // Khởi tạo các chân PWM cho động cơ
    void begin();

    // Điều khiển trực tiếp 2 bánh bằng giá trị (-PWM_MAX đến PWM_MAX)
    void setMotors(int leftSpeed, int rightSpeed);

    // Xử lý hướng (UP, DOWN, LEFT, RIGHT, STOP) kết hợp với hệ số tốc độ (0-100%)
    void updateMotorState(String direction, int speedRatioLeft, int speedRatioRight);
};

// Khai báo biến toàn cục (global) để các file khác có thể gọi trực tiếp
extern MotorController motor;

#endif // MOTOR_CONTROLLER_H
