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
