#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include <SD.h>

#include "Screens.h"

class FileBrowser
{
  private:
    String folders[50];
    String files[100];
    String displayItems[151];

    int folderCount = 0;
    int fileCount = 0;
    int displayItemsCount = 0;

    String currentPath;
    int cursor = 0;
    Screens screen;

    void Refresh()
    {
        if (!LittleFS.begin(false))
        {
            Serial.println("LittleFS mount failed!");
            return;
        }

        folderCount = 0;
        fileCount = 0;

        File file = LittleFS.open("/musiclist.json", FILE_READ);

        if (!file)
        {
            Serial.println("Coudldnt open musiclist.json");
            return;
        }

        JsonDocument doc;

        DeserializationError error = deserializeJson(doc, file);
        file.close();

        if (error)
        {
            Serial.print("JSON error: ");
            Serial.println(error.c_str());
            return;
        }

        JsonObject root = doc.as<JsonObject>();

        for (JsonPair artistPair : root)
        {
            if (folderCount >= 50)
                break;

            JsonObject artist = artistPair.value().as<JsonObject>();

            folders[folderCount++] = artist["name"] | "Unknown";

            JsonObject songs = artist["Songs"].as<JsonObject>();

            for (JsonPair songPair : songs)
            {
                if (fileCount >= 100)
                    break;

                JsonObject song = songPair.value().as<JsonObject>();

                files[fileCount++] = song["name"] | "Unknown";
            }
        }

        Serial.printf("Folders found: %d\n", folderCount);
        Serial.printf("MP3 files found: %d\n", fileCount);
    }

    void NormalGetDI()
    {
        Refresh();

        displayItemsCount = 0;

        if (currentPath != "/" && displayItemsCount < 151)
            displayItems[displayItemsCount++] = "..";

        for (int i = 0; i < folderCount && displayItemsCount < 151; i++)
            displayItems[displayItemsCount++] = GetFileName(folders[i]);

        for (int i = 0; i < fileCount && displayItemsCount < 151; i++)
            displayItems[displayItemsCount++] = GetFileName(files[i]);
    }

    String GetFileName(const String &path)
    {
        int slash = path.lastIndexOf('/');
        if (slash == -1)
            return path;

        return path.substring(slash + 1);
    }

  public:
    FileBrowser(String filePath, Screens screen)
    {
        currentPath = filePath;
        this->screen = screen;
    }

    String *GetDisplayItems()
    {
        switch (screen)
        {
        case Screens::Normal:
            NormalGetDI();
            break;

        default:
            displayItemsCount = 0;
            break;
        }

        return displayItems;
    }

    int GetDisplayItemsCount() { return displayItemsCount; }

    void MoveUp()
    {
        cursor--;
        if (cursor < 0)
            cursor = GetDisplayItemsCount() - 1;
    }

    Screens Screen() { return screen; }

    int Cursor() { return cursor; }
};
