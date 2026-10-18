#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <string>
#include <vector>

#include "Screens.h"

class FileBrowser
{
  private:
    std::vector<String> folders;
    std::vector<String> files;

    String currentPath;
    int cursor = 0;
    Screens screen;

    void Refresh()
    {
        folders.clear();
        files.clear();

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
                if (!name.startsWith("&") && name != "System Volume Information") // does not add system files
                    folders.push_back(path);
            }
            else if (name.endsWith(".mp3") || name.endsWith(".MP3")) // Adds all the files to files list
                files.push_back(path);

            entry.close();
            entry.openNextFile();
        }
        entry.close();
    }

    std::vector<String> NormalGetDI()
    {
        std::vector<String> displayItems;

        for (const String &path : folders)
            displayItems.push_back(GetFileName(path));

        for (const String &path : files)
            displayItems.push_back(GetFileName(path));

        if (currentPath != "/")
            displayItems.insert(displayItems.begin(), "..");

        return displayItems;
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

    std::vector<String> GetDisplayItems()
    {
        switch (screen)
        {
        case Screens::Normal:
            return NormalGetDI();

        default:
            return {};
        }
    }

    Screens Screen() { return screen; }
};
