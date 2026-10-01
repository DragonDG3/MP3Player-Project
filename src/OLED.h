#pragma once

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <cstdint>

class OLED {
private:
  Adafruit_SSD1306 display;

public:
  OLED();

  bool Begin();

  void clearDisplay();
  void displayU();

  void setTextSize(int size);
  void setTextColor(uint16_t color);
  void setCursor(int x, int y);

  void print(const char *text);

  void fillRect(int x, int y, int width, int height,
                uint16_t color = SSD1306_WHITE);
};
