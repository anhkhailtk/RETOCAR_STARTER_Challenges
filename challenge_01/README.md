# Thử thách 01: Xe Cứu Thương Thông Minh Điều Khiển Bằng Web

## 🚑 Giới thiệu
Trong kỷ nguyên của Internet of Things (IoT) và Đô thị thông minh (Smart City), các phương tiện cứu hộ đang ngày càng được hiện đại hóa. Hãy tưởng tượng một chiếc xe cứu thương khẩn cấp có khả năng được điều khiển từ xa qua mạng không dây để tiếp cận những khu vực nguy hiểm (như hầm lò sập, khu vực có khí độc hoặc phóng xạ) mà con người không thể trực tiếp đi vào. 

Chiếc xe này không chỉ cần khả năng di chuyển linh hoạt, mà còn phải được trang bị hệ thống cảnh báo (đèn chớp và còi hú) chuẩn xác để mở đường hoặc báo hiệu vị trí cho đội cứu hộ. 

Với tư cách là một kỹ sư hệ thống nhúng, nhiệm vụ của bạn là chế tạo hệ thống điều khiển trung tâm cho nguyên mẫu (prototype) của chiếc **Xe Cứu Thương Điều Khiển Bằng Web** này.

---

## 🎯 Mục tiêu thử thách
Xây dựng thành công một chương trình điều khiển xe trên vi điều khiển **ESP32** với đầy đủ giao diện web (Web Server), cho phép điều khiển xe di chuyển đa hướng và kích hoạt hệ thống cảnh báo khẩn cấp bằng thao tác trên màn hình cảm ứng điện thoại.

---

## 📋 Yêu cầu của thử thách

Để vượt qua thử thách này, bạn cần phải hoàn thành các yêu cầu chức năng sau:

### 1. Hệ thống di chuyển (Động cơ)
* Tích hợp và điều khiển mạch công suất **MX1508** để vận hành 2 động cơ độc lập (Trái và Phải).
* Cấu hình xung PWM để có thể điều chỉnh tốc độ từ 0% đến 100%.
* Viết logic giải mã hướng đi (Tiến, Lùi, Rẽ Trái, Rẽ Phải, Dừng) từ tay cầm Joystick ảo trên Web.

### 2. Hệ thống cảnh báo (Còi và Đèn)
* **Còi hú (Siren):** Lập trình tạo ra tiếng còi hụ khẩn cấp 2 âm vực (1000Hz và 700Hz) đan xen nhau. (Lưu ý phải dùng kênh PWM độc lập để không xung đột với động cơ).
* **Đèn pha:** Hai đèn LED chớp tắt luân phiên (Trái tắt - Phải bật và ngược lại) tương tự như xe cảnh sát/cứu thương.
* **Đèn RGB:** Khi bật đèn chớp, LED RGB phải nháy luân phiên 3 màu: **Đỏ -> Xanh Dương -> Trắng**.

### 3. Máy chủ Web & Giao diện (Wi-Fi Access Point)
* Biến ESP32 thành một điểm phát Wi-Fi (Access Point) độc lập.
* Cung cấp một trang Web (HTML/JS/CSS) hoạt động như một tay cầm điều khiển (Gamepad) trên trình duyệt.

### 4. Kiến trúc Mã nguồn (OOP)
* Mã nguồn không được viết gộp thành một khối khổng lồ (monolithic) trong `main.cpp`.
* Yêu cầu bắt buộc phải tái cấu trúc (refactor) theo phương pháp Lập trình Hướng đối tượng (OOP).
* Chia các chức năng thành các thư viện con độc lập nằm trong thư mục `lib/` (VD: `MotorController`, `SirenController`, `WebController`).
* **Tuyệt đối không sử dụng con trỏ (No Pointers)** khi giao tiếp giữa các module, ưu tiên sử dụng biến toàn cục (`extern`).

---

**Chúc bạn hoàn thành xuất sắc Thử thách 01!** 🚀
