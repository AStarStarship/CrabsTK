/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_button.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#if SEAM == KABUKI_FEATURES_TOUCH_1
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
  kMinDoublePressTime = 100,    //< Min SI4 click time period in ms.
  kMaxDoublePressTime = 2000,   //< Max SI4 click time period in ms.
  kDefaultDoublePressTime = 0;  //< Default SI4 press time period in ms.

  const CH1 *
  getButtonActionString(ButtonAction type){
      static const CH1 * buttonActionStrings[] = {"Momentary", "Latching",
                                                   "Pressure Sensitive"},
      *invalidState = "Invalid state";
      if (type < 0) return invalidState; if (type > 3) return invalidState;
      return buttonActionStrings[type];}

SI4 GetMinDoublePressTime() { return double_press_time_min; }
SI4 GetMaxDoublePressTime() { return double_press_time_max; }
SI4 GetDefaultDoublePressTime() { return double_press_time_default; }

Button::Button(ButtonAction action, SI4 stepSize, SI4 doublePressTime)
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

  SI4 currentTime = _System::GetTimestamp();
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

BOL Button::IsPressed() const { return pressed; }

void Button::SetState(BOL state) { pressed = state; }

SI4 Button::GetLastTimePressed() const { return last_time_pressed_; }

SI4 Button::GetStepSize() const { return stepSize; }

void Button::SetStepSize(SI4 value) {
  if (value < 0)
    stepSize = 0;
  else if (value >= getMaxWordValue())
    stepSize = getMaxWordValue();
  else
    stepSize = value;
}

ButtonAction Button::GetButtonAction() const { return action; }

void Button::SetButtonAction(ButtonAction newAction) { action = newAction; }

const CH1* Button::GetActionString() const {
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
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
