#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "OLED.h"

OLED display;

void setup()
{
    Serial.begin(115200);

    if (!display.Begin())
    {
        Serial.println("OLED failed!");

        while (true)
        {
        }
    }

    display.Begin();
}

void loop() {}
