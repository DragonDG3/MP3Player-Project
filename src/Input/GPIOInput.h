#pragma once

#include <Arduino.h>

#include "Config.h"
#include "InputAction.h"

class GPIOInput
{
  private:
    const int BTN_UP = 3;

  public:
    void Begin() { pinMode(BTN_UP, INPUT_PULLUP); }

    InputAction Read()
    {
        bool up = digitalRead(BTN_UP) == LOW;

        if (up)
            return InputAction::Up;

        return InputAction::None;
    }
};
