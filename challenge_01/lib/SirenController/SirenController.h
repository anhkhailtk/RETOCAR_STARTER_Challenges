#ifndef SIREN_CONTROLLER_H
#define SIREN_CONTROLLER_H

#include <Arduino.h>
#include "Config.h"

class SirenController {
public:
    void begin();

    // Gọi liên tục trong loop() để xử lý hiệu ứng chớp/hú theo thời gian
    void update();

    // Bật/tắt còi
    void toggleSiren();
    bool getSirenState() const { return isSirenActive; }

    // Bật/tắt đèn chớp & RGB
    void toggleLight();
    bool getLightState() const { return isLedFlashing; }

    // Xử lý nút nhấn vật lý
    void handlePhysicalButton();

private:
    bool isSirenActive = false;
    bool isLedFlashing = false;

    // Các biến trạng thái thời gian
    unsigned long previousMillis = 0;
    long interval = 600;
    bool isHighTone = false;

    unsigned long previousPatternMillis = 0;
    const long patternDuration = 4000;
    int sirenMode = 0;

    unsigned long previousLedMillis = 0;
    bool ledState = false;

    int rgbColorIndex = 0;
    unsigned long previousRgbMillis = 0;

    int lastButtonState = HIGH;
};

// Khai báo biến toàn cục (global) để các file khác có thể gọi trực tiếp
extern SirenController siren;

#endif // SIREN_CONTROLLER_H
