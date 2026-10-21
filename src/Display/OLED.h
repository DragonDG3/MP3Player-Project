#pragma once

#include "Adafruit_SSD1306.h"

#include "Config.h"

class OLED
{
  public:
    void println(const String &text) { display.println(text); }
    void print(const String &text) { display.print(text); }
    void clearDisplay() { display.clearDisplay(); }
    void setTextSize(int size) { display.setTextSize(size); }
    void setTextColor(uint16_t color) { display.setTextColor(color); }
    void setTextColor(uint16_t color, uint16_t bgcolor) { display.setTextColor(color, bgcolor); }
    void setCursor(int x, int y) { display.setCursor(x, y); }
    void refresh() { display.display(); }
};
