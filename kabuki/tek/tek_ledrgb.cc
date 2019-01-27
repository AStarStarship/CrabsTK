/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/tek_ledrgb.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include "c_ledrgb.h"
//#include "../script/color.h"

namespace _ {

/* Creates an LEDRGB in the off position. */
LEDRGB::LEDRGB(color_t color, offset_t red_offset, offset_t green_offset,
               offset_t blue_offset)
    : color_(color),
      red_offset_(red_offset),
      green_offset_(green_offset),
      blue_offset_(blue_offset) {}

color_t LEDRGB::GetColor() { return color_; }

void LEDRGB::SetColor(color_t value) { color_ = value; }

UI1 LEDRGB::GetRed() { return (UI1)color_; }

void LEDRGB::SetRed(color_t value) {
  color_t color = 0xffff00ff;  //< Mask for removing red channel.
  color &= color_;             //< Mask off the red value.
  color |= value << 8;         //< Add the new red channel.
}

UI1 LEDRGB::GetGreen() { return (UI1)(color_ >> 8); }

void LEDRGB::SetGreen(color_t value) {
  color_t color = 0xffff00ff;  //< Mask for removing green channel.
  color &= color_;             //< Mask off the green value.
  color |= value << 8;         //< Add the new green channel.
}

UI1 LEDRGB::GetBlue() { return (UI1)(color_ >> 16); }

void LEDRGB::SetBlue(color_t value) {
  color_t color = 0xff00ffff;  //< Mask for removing blue channel.
  color &= color_;             //< Mask off the blue value.
  color |= value << 8;         //< Add the new blue channel.
}

UI1 LEDRGB::GetBrightness() { return (UI1)(color_ >> 24); }

void LEDRGB::SetBrightness(color_t value) {
  color_t color = 0x00ffffff;  //< Mask for removing brightness channel.
  color &= color_;             //< Mask off the brightness value.
  color |= value << 8;         //< Add the new brightness channel.
}

offset_t LEDRGB::GetRedOffset() { return red_offset_; }

offset_t LEDRGB::GetGreenOffset() { return green_offset_; }

offset_t LEDRGB::GetBlueOffset() { return blue_offset_; }

}  // namespace _
