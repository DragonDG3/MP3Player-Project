#pragma once

#include <Arduino.h>
#include <FS.h>
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
        folderCount = 0;
        fileCount = 0;

        File dir = SD.open(currentPath);

        if (!dir || !dir.isDirectory())
        {
            dir.close();
            return;
        }

        File entry = dir.openNextFile();

        while (entry)
        {
            String path = entry.name();

            if (!path.startsWith("/")) // makes sure that the path is spelled correctly
            {
                path = currentPath;
                if (!path.endsWith("/"))
                    path += "/";
                path += entry.name(); // }
            }

            String name = GetFileName(path);

            if (entry.isDirectory()) // Adds all the folders from the sd card to the folders list
            {
                if (!name.startsWith("&") && name != "System Volume Information" &&
                    folderCount < 50) // does not add system files
                    folders[folderCount++] = path;
            }
            else if ((name.endsWith(".mp3") || name.endsWith(".MP3")) &&
                     fileCount < 100) // Adds all the files to files list
                files[fileCount++] = path;

            entry.close();
            entry = dir.openNextFile();
        }
        entry.close();

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
