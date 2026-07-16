#include "WebController.h"

// Hàm khởi tạo, KHÔNG dùng con trỏ nữa
WebController::WebController() : server(80) {}

void WebController::begin() {
    IPAddress apIP(192, 168, 4, 1);
    
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    
    Serial.println("Da phat Wi-Fi: " + String(WIFI_SSID));
    Serial.println("Truy cap vao trinh duyet voi IP: 192.168.4.1");

    // Đăng ký các endpoints
    server.on("/", HTTP_GET, [this]() { handleRoot(); });
    server.on("/toggleSiren", HTTP_GET, [this]() { handleToggleSiren(); });
    server.on("/toggleLight", HTTP_GET, [this]() { handleToggleLight(); });
    server.on("/cmd", HTTP_GET, [this]() { handleCmd(); });
    server.on("/speed", HTTP_GET, [this]() { handleSpeed(); });
    server.onNotFound([this]() { handleNotFound(); });

    server.begin();
}

void WebController::update() {
    server.handleClient();
}

void WebController::handleRoot() {
    server.send(200, "text/html", index_html);
}

void WebController::handleNotFound() {
    server.send(404, "text/plain", "Not found");
}

void WebController::handleToggleSiren() {
    // Gọi trực tiếp biến toàn cục 'siren'
    siren.toggleSiren();
    server.send(200, "text/plain", siren.getSirenState() ? "ON" : "OFF");
}

void WebController::handleToggleLight() {
    // Gọi trực tiếp biến toàn cục 'siren'
    siren.toggleLight();
    server.send(200, "text/plain", siren.getLightState() ? "ON" : "OFF");
}

void WebController::handleCmd() {
    if (server.hasArg("action")) {
        currentDirection = server.arg("action");
        Serial.println("Received Command: " + currentDirection);
        // Gọi trực tiếp biến toàn cục 'motor'
        motor.updateMotorState(currentDirection, speedRatioLeft, speedRatioRight);
    }
    server.send(200, "text/plain", "OK");
}

void WebController::handleSpeed() {
    if (server.hasArg("l") && server.hasArg("r")) {
        speedRatioLeft = server.arg("l").toInt();
        speedRatioRight = server.arg("r").toInt();
        Serial.println("Speed L: " + String(speedRatioLeft) + " R: " + String(speedRatioRight));
        // Gọi trực tiếp biến toàn cục 'motor'
        motor.updateMotorState(currentDirection, speedRatioLeft, speedRatioRight);
    }
    server.send(200, "text/plain", "OK");
}

// ==============================================================
// GIAO DIỆN HTML/CSS/JS (Copy nguyên vẹn từ bản gốc)
// ==============================================================
const char WebController::index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no, viewport-fit=cover">
    <title>PRO GAMEPAD</title>
    <style>
        :root {
            --bg: #121418;
            --surface: #1e2229;
            --primary: #00e5ff;
            --text-muted: #6b7280;
        }
        * { box-sizing: border-box; -webkit-tap-highlight-color: transparent; user-select: none; }
        body { 
            margin: 0; padding: 0 40px; background: radial-gradient(circle at center, #262b35 0%, #0c0e12 100%); 
            color: white; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
            height: 100vh; display: flex; justify-content: space-between; align-items: center; overflow: hidden;
            touch-action: none;
        }

        .control-pad {
            background: linear-gradient(145deg, #20242c, #1b1e25);
            border-radius: 50%; padding: 25px;
            box-shadow: 20px 20px 40px #0a0b0e, -20px -20px 40px #22272e, inset 0 0 0 2px rgba(255,255,255,0.02);
            position: relative;
        }

        /* --- CỤM JOYSTICK --- */
        .joystick-area {
            position: relative; width: 160px; height: 160px;
            background: #1a1d24; border-radius: 50%;
            box-shadow: inset 5px 5px 15px #0a0b0e, inset -5px -5px 15px #22272e;
            display: flex; justify-content: center; align-items: center;
            touch-action: none;
        }
        .joystick-stick {
            position: absolute; width: 70px; height: 70px;
            background: radial-gradient(circle at 30% 30%, #3a4252, #1e2229);
            border-radius: 50%;
            box-shadow: 5px 5px 10px #0a0b0e, -5px -5px 10px #22272e, inset 0 0 0 2px rgba(255,255,255,0.05);
            transition: transform 0.1s ease-out; /* Trở về mượt mà khi thả tay */
            z-index: 10; cursor: grab;
        }
        .joystick-stick.active {
            box-shadow: 0 0 15px var(--primary), inset 0 0 0 1px var(--primary);
            transition: none; /* Xóa transition khi đang kéo để phản hồi tức thì */
            cursor: grabbing;
        }

        /* --- CỤM CHỨC NĂNG --- */
        .action-pad { position: relative; width: 140px; height: 140px; }
        .btn-action {
            position: absolute; width: 48px; height: 48px; border-radius: 50%;
            border: none; font-weight: 800; font-size: 18px; color: #fff;
            text-shadow: 0 2px 2px rgba(0,0,0,0.5);
            box-shadow: 0 8px 15px rgba(0,0,0,0.6), inset 0 2px 5px rgba(255,255,255,0.3);
            display: flex; justify-content: center; align-items: center;
            transition: all 0.05s ease; z-index: 10;
        }
        .btn-action::after { /* Tạo độ bóng kính cho nút */
            content: ''; position: absolute; top: 2px; left: 15%; width: 70%; height: 40%;
            background: linear-gradient(rgba(255,255,255,0.4), transparent); border-radius: 50%;
        }
        .btn-y { top: 0; left: 46px; background: #e6c300; --c: #ffdb1a; }
        .btn-b { top: 46px; right: 0; background: #e74c3c; --c: #ff6b59; }
        .btn-a { bottom: 0; left: 46px; background: #2ecc71; --c: #4ade80; }
        .btn-x { top: 46px; left: 0; background: #3498db; --c: #60a5fa; }
        
        .btn-action.pressed {
            transform: translateY(6px); background: var(--c);
            box-shadow: 0 2px 5px rgba(0,0,0,0.8), inset 0 0 15px rgba(0,0,0,0.5), 0 0 20px var(--c);
        }

        /* --- KHU VỰC TRUNG TÂM --- */
        .center-panel { display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 20px; }
        .brand { font-size: 12px; letter-spacing: 6px; color: var(--text-muted); font-weight: bold; text-transform: uppercase; text-shadow: 0 2px 2px black;}
        
        /* Đèn LED báo hiệu kết nối */
        .status-led { width: 8px; height: 8px; border-radius: 50%; background: #2ecc71; box-shadow: 0 0 10px #2ecc71, inset 0 1px 2px white; margin-bottom: 10px;}
        
        /* Thanh trượt tốc độ */
        .speed-container {
            display: flex; flex-direction: column; align-items: center; gap: 8px;
            background: rgba(0,0,0,0.3); padding: 10px 15px; border-radius: 12px;
            box-shadow: inset 0 2px 5px rgba(0,0,0,0.5);
        }
        .speed-label { font-size: 11px; font-weight: bold; color: var(--primary); letter-spacing: 1px; }
        .speed-slider {
            -webkit-appearance: none; width: 100px; height: 6px; border-radius: 3px;
            background: #2a2e35; outline: none; box-shadow: inset 0 1px 3px rgba(0,0,0,0.8);
        }
        .speed-slider::-webkit-slider-thumb {
            -webkit-appearance: none; appearance: none;
            width: 18px; height: 18px; border-radius: 50%; background: var(--primary);
            cursor: pointer; box-shadow: 0 0 10px var(--primary);
        }

        .btn-action.active {
            box-shadow: 0 0 20px var(--c), inset 0 0 10px rgba(255,255,255,0.5);
            animation: pulse-btn 0.8s infinite alternate;
        }
        @keyframes pulse-btn {
            from { box-shadow: 0 0 10px var(--c); }
            to { box-shadow: 0 0 30px var(--c), 0 0 10px white; }
        }
    </style>
</head>
<body>

    <!-- Cụm Joystick thay cho D-Pad -->
    <div class="control-pad">
        <div class="joystick-area" id="joystickBase">
            <div class="joystick-stick" id="joystickStick"></div>
        </div>
    </div>

    <!-- Bảng điều khiển trung tâm -->
    <div class="center-panel">
        <div class="brand">ROBOTICS</div>
        <div class="status-led"></div>
        
        <!-- Thanh trượt hiệu chỉnh tốc độ -->
        <div class="speed-container">
            <div class="speed-label">TRÁI: <span id="speedLVal">100</span>%</div>
            <input type="range" id="speedLSlider" class="speed-slider" min="0" max="100" value="100" oninput="updateSpeedUI('L', this.value)" onchange="sendSpeed()">
            
            <div class="speed-label" style="margin-top: 5px;">PHẢI: <span id="speedRVal">100</span>%</div>
            <input type="range" id="speedRSlider" class="speed-slider" min="0" max="100" value="100" oninput="updateSpeedUI('R', this.value)" onchange="sendSpeed()">
        </div>
    </div>

    <!-- Cụm XYAB -->
    <div class="control-pad">
        <div class="action-pad">
            <button class="btn-action btn-y" onpointerdown="toggleSiren(this)">📢</button>
            <button class="btn-action btn-x" onpointerdown="toggleLight(this)">🚨</button>
            <button class="btn-action btn-b" onpointerdown="startCmd('B', this)" onpointerup="stopCmd(this)" onpointerleave="stopCmd(this)" onpointercancel="stopCmd(this)">B</button>
            <button class="btn-action btn-a" onpointerdown="startCmd('A', this)" onpointerup="stopCmd(this)" onpointerleave="stopCmd(this)" onpointercancel="stopCmd(this)">A</button>
        </div>
    </div>

    <script>
        // Hàm Rung (Haptic Feedback) - Chạy mượt trên Android
        function vibratePhone(ms) {
            if (navigator.vibrate) { navigator.vibrate(ms); }
        }

        // --- XỬ LÝ JOYSTICK ẢO ---
        const stick = document.getElementById('joystickStick');
        const base = document.getElementById('joystickBase');
        const maxRadius = 50; // Giới hạn biên độ kéo của núm
        let isDragging = false;
        let currentJoyAction = 'STOP';

        function handleJoystickMove(e) {
            if (!isDragging) return;
            
            // Lấy tọa độ trung tâm của vòng tròn Joystick
            const rect = base.getBoundingClientRect();
            const centerX = rect.left + rect.width / 2;
            const centerY = rect.top + rect.height / 2;

            // Tính khoảng cách từ ngón tay đến tâm
            let dx = e.clientX - centerX;
            let dy = e.clientY - centerY;
            let distance = Math.sqrt(dx * dx + dy * dy);
            
            // Giới hạn núm xoay không văng ra khỏi vòng
            if (distance > maxRadius) {
                dx = (dx / distance) * maxRadius;
                dy = (dy / distance) * maxRadius;
            }

            // Di chuyển núm
            stick.style.transform = `translate(${dx}px, ${dy}px)`;

            // --- Chuyển đổi tọa độ thành Lệnh (UP/DOWN/LEFT/RIGHT) ---
            let newAction = 'STOP';
            if (distance > 15) { // Vùng chết (Deadzone) 15px ở giữa để tránh nhiễu
                if (Math.abs(dx) > Math.abs(dy)) {
                    newAction = dx > 0 ? 'RIGHT' : 'LEFT';
                } else {
                    newAction = dy > 0 ? 'DOWN' : 'UP';
                }
            }

            // Chỉ gửi lệnh về mạch ESP32 nếu hướng thay đổi (Tránh lag server)
            if (newAction !== currentJoyAction) {
                currentJoyAction = newAction;
                fetch('/cmd?action=' + currentJoyAction).catch(err => console.error(err));
                if(newAction !== 'STOP') vibratePhone(20); // Rung nhẹ khi chuyển hướng
            }
        }

        // Bắt sự kiện chạm vào núm
        base.addEventListener('pointerdown', (e) => {
            isDragging = true;
            stick.classList.add('active');
            handleJoystickMove(e);
            base.setPointerCapture(e.pointerId); // Khóa con trỏ vào Joystick kể cả khi kéo ra ngoài
        });

        // Bắt sự kiện kéo
        base.addEventListener('pointermove', handleJoystickMove);

        // Hàm thả tay (Dừng xe và đưa núm về giữa)
        function releaseJoystick(e) {
            if (!isDragging) return;
            isDragging = false;
            stick.classList.remove('active');
            stick.style.transform = `translate(0px, 0px)`; // Trả về giữa
            
            if (currentJoyAction !== 'STOP') {
                currentJoyAction = 'STOP';
                fetch('/cmd?action=STOP').catch(err => console.error(err));
            }
            if(e) base.releasePointerCapture(e.pointerId);
        }

        base.addEventListener('pointerup', releaseJoystick);
        base.addEventListener('pointercancel', releaseJoystick);

        // Bắt đầu gửi lệnh (Cho các phím XYAB)
        function startCmd(action, btnElement) {
            btnElement.classList.add('pressed');
            vibratePhone(30); // Rung nhẹ 30ms báo hiệu đã nhận thao tác
            fetch('/cmd?action=' + action).catch(err => console.error(err));
        }

        // Ngừng lệnh (Khi nhấc ngón tay lên, hoặc trượt ngón tay ra ngoài nút)
        function stopCmd(btnElement) {
            if (btnElement.classList.contains('pressed')) {
                btnElement.classList.remove('pressed');
                fetch('/cmd?action=STOP').catch(err => console.error(err));
            }
        }

        // Cập nhật hệ số tốc độ động cơ
        function updateSpeedUI(side, val) {
            document.getElementById('speed' + side + 'Val').innerText = val;
        }

        function sendSpeed() {
            let l = document.getElementById('speedLSlider').value;
            let r = document.getElementById('speedRSlider').value;
            fetch('/speed?l=' + l + '&r=' + r).catch(err => console.error(err));
        }

        // Xử lý bật tắt Còi
        function toggleSiren(btnElement) {
            vibratePhone([50, 50, 50]); // Rung 3 nhịp kịch tính
            fetch('/toggleSiren')
                .then(response => response.text())
                .then(state => {
                    if(state === "ON") {
                        btnElement.classList.add('active');
                    } else {
                        btnElement.classList.remove('active');
                    }
                })
                .catch(err => console.error("Lỗi kết nối:", err));
        }

        // Xử lý bật tắt Đèn chớp
        function toggleLight(btnElement) {
            vibratePhone([50, 50]); // Rung 2 nhịp
            fetch('/toggleLight')
                .then(response => response.text())
                .then(state => {
                    if(state === "ON") {
                        btnElement.classList.add('active');
                    } else {
                        btnElement.classList.remove('active');
                    }
                })
                .catch(err => console.error("Lỗi kết nối:", err));
        }

        // Ngăn chặn các thao tác vuốt làm phóng to/cuộn trang web
        window.addEventListener('contextmenu', e => { e.preventDefault(); e.stopPropagation(); return false; });
    </script>
</body>
</html>
)rawliteral";
