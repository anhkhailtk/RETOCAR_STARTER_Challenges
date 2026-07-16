#include "MotorController.h"

// Định nghĩa biến toàn cục
MotorController motor;

void MotorController::begin() {
    // Khởi tạo PWM cho động cơ theo thông số trong Config.h
    ledcSetup(CH_MA1, PWM_FREQ, PWM_RES); ledcAttachPin(PIN_MA1, CH_MA1);
    ledcSetup(CH_MA2, PWM_FREQ, PWM_RES); ledcAttachPin(PIN_MA2, CH_MA2);
    ledcSetup(CH_MB1, PWM_FREQ, PWM_RES); ledcAttachPin(PIN_MB1, CH_MB1);
    ledcSetup(CH_MB2, PWM_FREQ, PWM_RES); ledcAttachPin(PIN_MB2, CH_MB2);

    // Đảm bảo xe đứng yên khi khởi động
    setMotors(0, 0);
}

void MotorController::setMotors(int leftSpeed, int rightSpeed) {
    // Tách điện áp dương/âm thành 2 chân tín hiệu cho mạch MX1508
    int valMA1 = leftSpeed > 0 ? leftSpeed : 0;
    int valMA2 = leftSpeed < 0 ? -leftSpeed : 0;
    int valMB1 = rightSpeed > 0 ? rightSpeed : 0;
    int valMB2 = rightSpeed < 0 ? -rightSpeed : 0;

    Serial.printf("Motor L: (%d, %d), Motor R: (%d, %d)\n", valMA1, valMA2, valMB1, valMB2);

    ledcWrite(CH_MA1, valMA1);
    ledcWrite(CH_MA2, valMA2);
    ledcWrite(CH_MB1, valMB1);
    ledcWrite(CH_MB2, valMB2);
}

void MotorController::updateMotorState(String direction, int speedRatioLeft, int speedRatioRight) {
    // Tính toán tốc độ tối đa sau khi áp dụng thanh trượt (0-100%)
    int maxLeft = (PWM_MAX * speedRatioLeft) / 100;
    int maxRight = (PWM_MAX * speedRatioRight) / 100;
  
    // Giải mã lệnh hướng di chuyển
    if (direction == "UP") {
        setMotors(-maxLeft, -maxRight);
    } else if (direction == "DOWN") {
        setMotors(maxLeft, maxRight);
    } else if (direction == "LEFT") {
        setMotors(maxLeft, -maxRight);
    } else if (direction == "RIGHT") {
        setMotors(-maxLeft, maxRight);
    } else {
        setMotors(0, 0);
    }
}
