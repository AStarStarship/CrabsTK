/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_button.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "button.h"

namespace _ {

enum {
  kMinDoublePressTime = 100,    //< Min int click time period in ms.
  kMaxDoublePressTime = 2000,   //< Max int click time period in ms.
  kDefaultDoublePressTime = 0;  //< Default int press time period in ms.

  const char *
  getButtonActionString(ButtonAction type){
      static const char * buttonActionStrings[] = {"Momentary", "Latching",
                                                   "Pressure Sensitive"},
      *invalidState = "Invalid state";
      if (type < 0) return invalidState; if (type > 3) return invalidState;
      return buttonActionStrings[type];}

int GetMinDoublePressTime() { return double_press_time_min; }
int GetMaxDoublePressTime() { return double_press_time_max; }
int GetDefaultDoublePressTime() { return double_press_time_default; }

Button::Button(ButtonAction action, int stepSize, int doublePressTime)
    : action(action),
      pressed(false),
      stepSize(stepSize),
      doublePressTime(doublePressTime),
      last_time_pressed_(0) {
  SetButtonAction(action);
}

Button::Button(const Button& o)
    : action(o.action),
      stepSize(o.stepSize),
      pressed(o.pressed),
      doublePressTime(o.doublePressTime),
      last_time_pressed_(o.last_time_pressed_) {
  // Nothing to do here :-)
}

void Button::Press() {
  pressed = true;

  int currentTime = _System::GetTimestamp();
  if (currentTime - last_time_pressed_ <= doublePressTime) {
    doublePress();
    return;
  }
  press();
}

void Button::Depress() {
  pressed = false;
  Depress();
}

bool Button::IsPressed() const { return pressed; }

void Button::SetState(bool state) { pressed = state; }

int Button::GetLastTimePressed() const { return last_time_pressed_; }

int Button::GetStepSize() const { return stepSize; }

void Button::SetStepSize(int value) {
  if (value < 0)
    stepSize = 0;
  else if (value >= getMaxWordValue())
    stepSize = getMaxWordValue();
  else
    stepSize = value;
}

ButtonAction Button::GetButtonAction() const { return action; }

void Button::SetButtonAction(ButtonAction newAction) { action = newAction; }

const char* Button::GetActionString() const {
  return getButtonActionString(action);
}

void Button::Toggle() {
  if (pressed)
    pressed = false;
  else
    pressed = true;
}

Utf& Button::Print(Utf& printer) const {}

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
