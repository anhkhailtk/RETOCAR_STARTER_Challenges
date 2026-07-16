#include "SirenController.h"

// Định nghĩa biến toàn cục
SirenController siren;

void SirenController::begin() {
    pinMode(PIN_BTN_START, INPUT);
    pinMode(PIN_LED_LEFT, OUTPUT);
    pinMode(PIN_LED_RIGHT, OUTPUT);
    
    // --- Setup LED RGB ---
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    digitalWrite(PIN_LED_R, HIGH); // Tắt (common anode)
    digitalWrite(PIN_LED_G, HIGH);
    digitalWrite(PIN_LED_B, HIGH);

    // --- Setup PWM buzzer ---
    pinMode(PIN_BUZZER, OUTPUT);
    ledcSetup(CH_BUZZ, 2000, 8); 
    ledcAttachPin(PIN_BUZZER, CH_BUZZ);
    ledcWrite(CH_BUZZ, 0);
}

void SirenController::toggleSiren() {
    isSirenActive = !isSirenActive;
    if (!isSirenActive) {
        ledcWrite(CH_BUZZ, 0);
        ledcSetup(CH_BUZZ, 2000, 8); 
        ledcAttachPin(PIN_BUZZER, CH_BUZZ);
    }
}

void SirenController::toggleLight() {
    isLedFlashing = !isLedFlashing;
    if (!isLedFlashing) {
        digitalWrite(PIN_LED_LEFT, LOW);
        digitalWrite(PIN_LED_RIGHT, LOW);
    }
}

void SirenController::handlePhysicalButton() {
    int currentButtonState = digitalRead(PIN_BTN_START);
    if (lastButtonState == HIGH && currentButtonState == LOW) {
        toggleSiren();
        delay(50); // Chống dội phím (debounce)
    }
    lastButtonState = currentButtonState;
}

void SirenController::update() {
    handlePhysicalButton();

    // ---------------------------------------------------------
    // CẬP NHẬT KIỂU CHỚP/HÚ (Theo thời gian 4 giây)
    // ---------------------------------------------------------
    if (isSirenActive || isLedFlashing) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousPatternMillis >= patternDuration) {
            previousPatternMillis = currentMillis;
            sirenMode = (sirenMode == 0) ? 1 : 0;
        }
        interval = (sirenMode == 0) ? 600 : 150;
    } else {
        previousPatternMillis = millis();
    }

    // ---------------------------------------------------------
    // XỬ LÝ ĐÈN CỨU THƯƠNG (CẢ LED TRÁI/PHẢI VÀ RGB)
    // ---------------------------------------------------------
    if (isLedFlashing) {
        unsigned long currentMillis = millis();
        // 1. Nháy đèn LED trái/phải
        if (currentMillis - previousLedMillis >= interval) {
            previousLedMillis = currentMillis;
            ledState = !ledState;
            digitalWrite(PIN_LED_LEFT, ledState ? HIGH : LOW);
            digitalWrite(PIN_LED_RIGHT, ledState ? LOW : HIGH);
        }
        
        // 2. Nháy LED RGB 3 màu
        if (currentMillis - previousRgbMillis >= 200) {
            previousRgbMillis = currentMillis;
            rgbColorIndex = (rgbColorIndex + 1) % 3;
            // Tắt hết trước (common anode: HIGH = tắt)
            digitalWrite(PIN_LED_R, HIGH);
            digitalWrite(PIN_LED_G, HIGH);
            digitalWrite(PIN_LED_B, HIGH);
            // Bật màu tương ứng (LOW = bật)
            if (rgbColorIndex == 0) { digitalWrite(PIN_LED_R, LOW); }                          // Đỏ
            else if (rgbColorIndex == 1) { digitalWrite(PIN_LED_B, LOW); }                     // Xanh dương
            else { digitalWrite(PIN_LED_R, LOW); digitalWrite(PIN_LED_G, LOW); digitalWrite(PIN_LED_B, LOW); } // Trắng
        }
    } else {
        // Tắt RGB khi không bật đèn
        digitalWrite(PIN_LED_R, HIGH);
        digitalWrite(PIN_LED_G, HIGH);
        digitalWrite(PIN_LED_B, HIGH);
    }

    // ---------------------------------------------------------
    // XỬ LÝ CÒI CỨU THƯƠNG
    // ---------------------------------------------------------
    if (isSirenActive) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            isHighTone = !isHighTone;

            if (isHighTone) ledcWriteTone(CH_BUZZ, 1000);
            else ledcWriteTone(CH_BUZZ, 700);
        }
    }
}
