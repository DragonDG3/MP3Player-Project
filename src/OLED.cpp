#include "OLED.h"

OLED::OLED() : display(128, 64, &Wire, -1) {}

bool OLED::Begin() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    return false;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.display();

  return true;
}

void OLED::clearDisplay() { display.clearDisplay(); }

void OLED::displayU() { display.display(); }

void OLED::setTextSize(int size) { display.setTextSize(size); }

void OLED::setTextColor(uint16_t color) { display.setTextColor(color); }

void OLED::setCursor(int x, int y) { display.setCursor(x, y); }

void OLED::print(const char *text) { display.print(text); }

void OLED::fillRect(int x, int y, int width, int height, uint16_t color) {
  display.fillRect(x, y, width, height, color);
}
