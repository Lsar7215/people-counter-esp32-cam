# People Counter with ESP32-CAM

Count the number of people entering and leaving a space using an ESP32-CAM with an OV2640 camera module. The current count is displayed on a 0.91" I2C OLED screen.

## 📷 Hardware Used

- ESP32-CAM (AI Thinker) with OV2640 camera
- ESP32-CAM MB (USB to serial adapter board)
- 0.91" OLED display (I2C, 128x32 or 128x64)
- Micro USB cable 
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
2. Go to **File > Preferences**
3. In the “Additional Board Manager URLs” field, add the following:
  - https://espressif.github.io/arduino-esp32/package_esp32_index.json
  - https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Go to **Tools > Board > Boards Manager**
5. Search for `esp32` and install the latest version by **Espressif Systems**
