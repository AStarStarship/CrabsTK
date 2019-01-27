/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_xycontrol.cc
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

namespace _ {

#include "xy_ontrol.h"

XyControl::XyControl(SI4 initX, SI4 initY, SI4 initLeftBounds,
                     SI4 initRightBounds, SI4 initTopBounds,
                     SI4 initBottomBounds)
    : x(initX),
      y(initY),
      leftBounds(initLeftBounds),
      rightBounds(initRightBounds),
      topBounds(initTopBounds),
      bottomBounds(initBottomBounds) {
  // Nothing to do here yet.
}

XyControl::XyControl(const XyControl& other)
    : x(other.x),
      y(other.y),
      leftBounds(other.leftBounds),
      rightBounds(other.rightBounds),
      topBounds(other.topBounds),
      bottomBounds(other.bottomBounds) {
  // Nothing to do here yet.
}

SI4 XyControl::GetX() { return x; }
SI4 XyControl::GetY() { return y; }

void XyControl::SetX(SI4 value) {
  if (value <= leftBounds)
    x = leftBounds;
  else if (value >= rightBounds)
    x = rightBounds;
  else
    x = value;
}
void XyControl::SetY(SI4 value) {
  if (value <= bottomBounds)
    y = bottomBounds;
  else if (value >= topBounds)
    y = topBounds;
  else
    y = value;
}

SI4 XyControl::GetLeftBounds() { return leftBounds; }

SI4 XyControl::GetRightBounds() { return rightBounds; }

SI4 XyControl::topBounds() { return topBounds; }

SI4 XyControl::GetBottomBound() { return bottomBounds; }

BOL XyControl::SetLeftBounds(SI4 newBounds) {
  if (newBounds >= rightBounds) return false;

  leftBounds = newBounds;

  return true;
}

BOL XyControl::SetRightBounds(SI4 newBounds) {
  if (newBounds <= rightBounds) return false;

  rightBounds = newBounds;

  return true;
}

BOL XyControl::SetTopBounds(SI4 newBounds) {
  if (newBounds <= bottomBounds) return false;

  topBounds = newBounds;

  return true;
}

BOL XyControl::SetBottomBound(SI4 newBounds) {
  if (newBounds >= topBounds) return false;

  bottomBounds = newBounds;

  return true;
}

Utf& XyControl::Print() {
  string temp = "XyControl:\nx: " + x + "  y: " + y + "\n";
  slot.prints(temp);
}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
