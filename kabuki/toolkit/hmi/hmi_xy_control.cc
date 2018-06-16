/* Kabuki Toolkit
@file    ~/source/hmi/hmi_xy_control.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
          All right reserved (R). Licensed under the Apache License, Version
          2.0 (the "License"); you may not use this file except in
          compliance with the License. You may obtain a copy of the License
          [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
          required by applicable law or agreed to in writing, software
          distributed under the License is distributed on an "AS IS" BASIS,
          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
          implied. See the License for the specific language governing
          permissions and limitations under the License.
*/

#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#if MAJOR_SEAM == 2 && MINOR_SEAM == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)   \
  printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

namespace _ {

#include "xy_ontrol.h"

XyControl::XyControl(int initX, int initY, int initLeftBounds,
                     int initRightBounds, int initTopBounds,
                     int initBottomBounds)
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

int XyControl::GetX() { return x; }
int XyControl::GetY() { return y; }

void XyControl::SetX(int value) {
  if (value <= leftBounds)
    x = leftBounds;
  else if (value >= rightBounds)
    x = rightBounds;
  else
    x = value;
}
void XyControl::SetY(int value) {
  if (value <= bottomBounds)
    y = bottomBounds;
  else if (value >= topBounds)
    y = topBounds;
  else
    y = value;
}

int XyControl::GetLeftBounds() { return leftBounds; }

int XyControl::GetRightBounds() { return rightBounds; }

int XyControl::topBounds() { return topBounds; }

int XyControl::GetBottomBound() { return bottomBounds; }

bool XyControl::SetLeftBounds(int newBounds) {
  if (newBounds >= rightBounds) return false;

  leftBounds = newBounds;

  return true;
}

bool XyControl::SetRightBounds(int newBounds) {
  if (newBounds <= rightBounds) return false;

  rightBounds = newBounds;

  return true;
}

bool XyControl::SetTopBounds(int newBounds) {
  if (newBounds <= bottomBounds) return false;

  topBounds = newBounds;

  return true;
}

bool XyControl::SetBottomBound(int newBounds) {
  if (newBounds >= topBounds) return false;

  bottomBounds = newBounds;

  return true;
}

Printer& XyControl::Print() {
  string temp = "XyControl:\nx: " + x + "  y: " + y + "\n";
  slot.prints(temp);
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
