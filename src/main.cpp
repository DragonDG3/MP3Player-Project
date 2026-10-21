#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include "Config.h"
#include "Display/OLED.h"
#include "FileSystem/FileBrowser.h"
#include "FileSystem/Screens.h"
#include "Input/GPIOInput.h"
#include "Input/InputAction.h"

void ScreenSetup();
void NormalScreen();

OLED oled = OLED();
GPIOInput input = GPIOInput();
FileBrowser browser = FileBrowser("/Music", Screens::Normal);

void setup()
{
    Serial.begin(115200);

    input.Begin();

    SPI.begin();

    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
    {
        Serial.println("OLED initialization failed!");
        while (true)
            delay(100);
    }

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);

    Serial.println("OLED initialized!");
}

void loop()
{

    switch (browser.Screen())
    {
    case Screens::Normal:
        NormalScreen();
        break;
    }
}

void ScreenSetup()
{
    String *displayItems = browser.GetDisplayItems();
    int displayItemsCount = browser.GetDisplayItemsCount();
    int cursor = browser.Cursor();

    int start = fmax(0, cursor - 3);

    oled.clearDisplay();

    for (int row = 0; row < 4; row++)
    {
        int index = start + row;

        if (index > displayItemsCount)
            break;

        oled.setCursor(0, row * 8);

        bool selected = (index == cursor);

        oled.setTextColor(selected ? SSD1306_BLACK : SSD1306_WHITE, selected ? SSD1306_WHITE : SSD1306_BLACK);

        oled.println(displayItems[row]);
    }

    oled.refresh();
}

void NormalScreen()
{
    ScreenSetup();

    InputAction key = input.Read();

    switch (key)
    {
    case InputAction::Up:
        browser.MoveUp();
        break;
    }
}
