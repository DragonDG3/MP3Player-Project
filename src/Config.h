#pragma once

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_SDA 6
#define OLED_SCL 7
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
