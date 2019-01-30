/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cledrgb.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_LEDS_RGBLED
#define INCLUDED_KABUKI_FEATURE_TEK_LEDS_RGBLED 1

namespace _ {

/* A Red-Green-Blue LED.
Type UI1 is going to be a integer type used for storing
@todo There needs to be a camera setup that scans the LEDs on the board and
stores an offset, like +/-16, that allows for adjusting brighter LEDs.
Its not a problem till we've determined it is a problem. */
class LEDRGB {
 public:
  /* Creates an LEDRGB in the off position. */
  LEDRGB(CRGBAUI4 color, offset_t r, offset_t g, offset_t b);

  /* Gets the color. */
  CRGBAUI4 GetColor();

  /* Sets the color to the new value. */
  void SetColor(CRGBAUI4 rgba);

  /* Gets the red channel. */
  UI1 GetRed();

  /* Sets the red channel to the new value. */
  void SetRed(CRGBAUI4 value);

  /* Gets the green channel. */
  UI1 GetGreen();

  /* Sets the green channel to the new value. */
  void SetGreen(CRGBAUI4 value);

  /* Gets the blue channel. */
  UI1 GetBlue();

  /* Sets the blue channel to the new value. */
  void SetBlue(CRGBAUI4 value);

  /* Gets the brightness channel. */
  UI1 GetBrightness();

  /* Sets the brightness channel to the new value. */
  void SetBrightness(CRGBAUI4 value);

  /* Gets the red LED offset. */
  offset_t GetRedOffset();

  /* Gets the green LED offset. */
  offset_t GetGreenOffset();

  /* Gets the blue LED offset. */
  offset_t GetBlueOffset();

 private:
  CRGBAUI4 color_;     //< RGBA color where A is additive brightness.
  ch_t red_offset_,   //< Red LED bit offset.
      green_offset_,  //< Green LED bit offset.
      blue_offset_,   //< Blue LED bit offset.
      reserved_;      //< Reserved for memory alignment.
};
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_LEDS_RGBLED
