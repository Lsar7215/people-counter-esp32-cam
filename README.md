# People Counter with ESP32-CAM

Track and count the number of people entering and leaving a space using an ESP32-CAM with an OV2640 camera module. The current count is displayed on a I2C OLED screen.

## 📷 Hardware Used

- ESP32-CAM (AI Thinker) with OV2640 camera
- ESP32-CAM-MB (USB to serial adapter board) or
- FT232RL (USB to serial adapter board)
- OLED display (I2C, 128x32 or 128x64)
- Micro USB cable (Only if using ESP32-CAM-MB)
- Mini USB cable (Only if using FT232RL)
- Jumper wires

## 🧠 Features

- Detects motion and determines entry/exit direction
- Counts number of people entering and leaving
- Displays real-time count on OLED screen

## 📦 Libraries Required

- [`Adafruit_SSD1306`](https://github.com/adafruit/Adafruit_SSD1306)
- [`Adafruit_GFX`](https://github.com/adafruit/Adafruit-GFX-Library)

## ⚙️ ESP32 Board Installation (Arduino IDE)

To program the ESP32-CAM, install the ESP32 board support:

1. Open **Arduino IDE**            
2. Go to **Tools > Board > Boards Manager**
3. Search for `esp32` and install the latest version by **Espressif Systems**
- [`Espressif Systems`](https://github.com/espressif/arduino-esp32)

---

## 🔌 Hardware Installation Guide

### 1. Wiring ESP32-CAM to FT232RL (for code upload)

Since the ESP32-CAM does not have a USB port, you need the FT232RL USB-to-serial adapter for programming.

**Connections:**
- **FT232RL 5V → ESP32-CAM 5V**
- **FT232RL GND → ESP32-CAM GND**
- **FT232RL TX → ESP32-CAM U0R (RX)**
- **FT232RL RX → ESP32-CAM U0T (TX)**
- **ESP32-CAM IO0 → GND** (only while uploading)

> ⚠️ Important: IO0 must be connected to GND when uploading code.  
> After upload, disconnect it so the ESP32-CAM can boot normally.

📷 Wiring Diagram:  
<img width="756" height="286" alt="image" src="https://github.com/user-attachments/assets/17956815-3c64-4b6e-8cc3-fff99ed4a00d" />

---

### 2. Uploading the Code

1. Connect the FT232RL to your PC with a **Mini USB cable**.  
2. In Arduino IDE, select:
   - Board: **AI Thinker ESP32-CAM**
   - Correct COM port
3. Click **Upload**.  
   - If you get sync errors, press the **RESET** button on ESP32-CAM during upload.  
4. Once upload is complete:
   - Disconnect power  
   - Remove the **IO0 → GND** jumper  
   - Power on again (ESP32-CAM now runs your program)

---

### 3. Connecting the OLED Display

The OLED uses I²C protocol (SDA/SCL). Example wiring (check your code for pin definitions):

- **OLED VCC → ESP32-CAM 3.3V**
- **OLED GND → ESP32-CAM GND**
- **OLED SDA → ESP32-CAM GPIO 14**
- **OLED SCL → ESP32-CAM GPIO 15**

📷 OLED Wiring Diagram:  
<img width="381" height="256" alt="image" src="https://github.com/user-attachments/assets/0992cf50-20a1-4fee-8eeb-c801f92fcf67" />

---

### 4. Powering the Project

After programming, you no longer need the FT232RL connected.  
You can power the ESP32-CAM directly with a **5V USB adapter** (into the 5V pin).  

The OLED will display the people count as the ESP32-CAM runs your program.

---

### ✅ Quick Checklist

- [ ] FT232RL 5V ↔ ESP32-CAM 5V  
- [ ] FT232RL GND ↔ ESP32-CAM GND  
- [ ] FT232RL TX ↔ ESP32-CAM U0R  
- [ ] FT232RL RX ↔ ESP32-CAM U0T  
- [ ] IO0 ↔ GND (only during upload)  
- [ ] Remove IO0 jumper before running code  
- [ ] OLED wired correctly to SDA/SCL pins  

---
