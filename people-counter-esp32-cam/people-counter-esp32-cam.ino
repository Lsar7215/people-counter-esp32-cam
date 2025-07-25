#define CAMERA_MODEL_AI_THINKER

#include <WiFi.h>
#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "img_converters.h"

#include "camera_index.h"
#include "camera_pins.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define FRAME_SIZE FRAMESIZE_QQVGA
#define WIDTH 160
#define HEIGHT 120
#define BLOCK_SIZE 10
#define W (WIDTH / BLOCK_SIZE)
#define H (HEIGHT / BLOCK_SIZE)
#define BLOCK_DIFF_THRESHOLD 0.2
#define IMAGE_DIFF_THRESHOLD 0.2
#define DEBUG 1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

#define I2C_SDA 13
#define I2C_SCL 14
#define FLASH_PIN 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


/*
      Type in your Wi-Fi ssid and password
*/
const char *ssid = "****************";
const char *password = "************";

uint16_t prev_frame[H][W] = { 0 };
uint16_t current_frame[H][W] = { 0 };
int list[2] = { 0, 0 };
int counter = -1, prevCounter = -1;
int inCounter = 0;
int outCounter = 0;

void startCameraServer();
bool setup_camera(framesize_t);
bool capture_still();
int motion_detect();
void update_frame();
void print_frame(uint16_t frame[H][W]);
bool direction_detection(uint16_t frame[H][W]); 
int freq(uint16_t frame[H][W], uint16_t a);

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  pinMode(FLASH_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  digitalWrite(FLASH_PIN, HIGH);
  delay(1000);
  digitalWrite(FLASH_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  startCameraServer();
  Serial.print("Camera Stream Ready! Go to http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.println(setup_camera(FRAME_SIZE) ? "Camera Init OK" : "Camera Init ERR");

  digitalWrite(FLASH_PIN, HIGH);
  delay(1000);
  digitalWrite(FLASH_PIN, LOW);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Total people in: ");
  display.println("0");
  display.print("Total people out: ");
  display.println("0");
  display.display();
  delay(1000);
  ESP.getFreeHeap();

    if (!capture_still()) {
      Serial.println("Failed to capture image for motion detection.");
      return;
    }

  switch (motion_detect()) {
    case 0:
      Serial.println("No Motion");
      list[1] = 0;
      break;
    case 1:
      Serial.println("Entering ");
      list[1] = 1;
      delay(1000);
      break;
    case -1:
      Serial.println("Leaving ");
      list[1] = -1;
      delay(1000);
      break;
  }

  if ((list[0] == 0) && (list[1] == 1)) {
    counter++;
    inCounter++;

  } else if ((list[0] == 0) && (list[1] == -1)) {
    counter--;
    outCounter++;
  }
  list[0] = list[1];

  if (counter != prevCounter) {
    // Serial.print("Total people in: ");
    // Serial.println(inCounter);
    // Serial.print("Total people out: ");
    // Serial.println(outCounter);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Total people in: ");
    display.println(inCounter);
    display.print("Total people out: ");
    display.println(outCounter);
    display.display();
    delay(1000);
  }
  prevCounter = counter;
  update_frame();
  delay(1000);
}

bool setup_camera(framesize_t frameSize) {
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = frameSize;
  config.jpeg_quality = 15;
  config.fb_count = 1;

  bool ok = esp_camera_init(&config) == ESP_OK;
  if (!psramFound()) {
    Serial.println("⚠️ PSRAM not found! Camera performance may be limited.");
    if (!ok) {
      Serial.println("Camera Init failed without PSRAM.");
      return false;
    }
  }
  sensor_t *sensor = esp_camera_sensor_get();

  ESP.getFreeHeap();
  return ok;
}

bool capture_still() {
  camera_fb_t *frame_buffer = esp_camera_fb_get();

  if (!frame_buffer) {
    Serial.println("Error: Failed to get camera frame buffer!");
    return false;
  }

  for (int y = 0; y < H; y++)
    for (int x = 0; x < W; x++)
      current_frame[y][x] = 0;

  for (uint32_t i = 0; i < (uint32_t)frame_buffer->width * frame_buffer->height; i++) {
    const uint16_t x = i % frame_buffer->width;
    const uint16_t y = floor(i / frame_buffer->width);
    const uint8_t block_x = floor(x / BLOCK_SIZE);
    const uint8_t block_y = floor(y / BLOCK_SIZE);

    if (block_x < W && block_y < H) {
      const uint8_t pixel = frame_buffer->buf[i];
      current_frame[block_y][block_x] += pixel;
    }
  }

  for (int y = 0; y < H; y++)
    for (int x = 0; x < W; x++)
      current_frame[y][x] /= (BLOCK_SIZE * BLOCK_SIZE);

  esp_camera_fb_return(frame_buffer);
  return true;
}

bool direction_detection(uint16_t frame[H][W]) {
  uint16_t direc_left[H][W] = { 0 };
  uint16_t direc_right[H][W] = { 0 };
  //   memset(direc_left, 0, sizeof(direc_left));
  //   memset(direc_right, 0, sizeof(direc_right));

  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      if (x < W / 2) {
        direc_left[y][x] = frame[y][x];
      } else {
        direc_right[y][x] = frame[y][x];
      }
    }
  }

  if (freq(direc_right, { 99 }) > freq(direc_left, { 99 })) {
    return false;
  } else if (freq(direc_right, { 99 }) < freq(direc_left, { 99 })) {
    return true;
  }
  return false;
}

int motion_detect() {
  uint16_t changes = { 0 };
  const uint16_t blocks = (WIDTH * HEIGHT) / (BLOCK_SIZE * BLOCK_SIZE);
  uint16_t direc[H][W] = { 0 };
  //   memset(direc, 0, sizeof(direc));
  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      float current = (float)current_frame[y][x];
      float prev = (float)prev_frame[y][x];
      float delta = 0;
      if (prev != 0) {
        delta = abs(current - prev) / prev;
      } else if (current != 0) {
        delta = 1.0;
      }

      if (delta >= BLOCK_DIFF_THRESHOLD) {
#if DEBUG
        changes += 1;
        direc[y][x] = { 99 };
#endif
      }
    }
  }


  if ((1.0 * changes / blocks) > IMAGE_DIFF_THRESHOLD) {
    if (direction_detection(direc)) {
      return 1;
    } else {
      return -1;
    }
  } else {
    return 0;
  }
}


void update_frame() {
  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      prev_frame[y][x] = current_frame[y][x];
    }
  }
}

int freq(uint16_t matrix[H][W], uint16_t a) {
  int freq = 0;
  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      if (matrix[y][x] == a) {
        freq = freq + 1;
      }
    }
  }
  return freq;
}

void print_frame(uint16_t frame[H][W]) {
  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      Serial.print(frame[y][x]);
      Serial.print('\t');
    }

    Serial.println();
  }
}