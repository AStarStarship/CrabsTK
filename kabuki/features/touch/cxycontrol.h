/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_xycontrol.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#ifndef HEADER_FOR_KT_HMI_XYCONTROL
#define HEADER_FOR_KT_HMI_XYCONTROL

#include "config.h"

namespace _ {

class SDK XyTouchpad : public Component {
 public:
  /* Constructor. */
  XyTouchpad(SI4 init_x = 0, SI4 init_y = 0, SI4 init_left_bounds = 0,
             SI4 init_right_bounds = 0, SI4 initTopBounds = 0,
             SI4 initBottomBounds = 0);

  /* Copy constructor. */
  XyTouchpad(const XyTouchpad& O);

  /* Gets thisX value. */
  SI4 GetX();

  /* Gets thisY value. */
  SI4 GetY();

  /* Sets thisX to the value. */
  void SetX(SI4 value);

  /* Sets thisY to the value. */
  void SetY(SI4 value);

  /* Gets th left_bounds. */
  SI4 GetLeftBounds();

  /* Gets th right_bounds. */
  SI4 GetRightBounds();

  /* Gets th top_bounds. */
  SI4 GetTopBounds();

  /* Gets th bottom_bounds. */
  SI4 GetBottomBound();

  /* Sets the left_bounds to the newBouds. */
  @pre Value < right_bounds.*/ BOL SetLeftBounds(SI4 Value);

  /* Sets the right_bounds to the newBouds
      @pre Value > left_bounds    */
  BOL SetRightBounds(SI4 Value);

  /* Sets the top_bounds to the newBouds
      @pre Value > bottom_bounds. */
  BOL SetTopBounds(SI4 Value);

  /* Sets the bottom_bounds to the newBouds
      @pre Value < top_bounds. */
  BOL SetBottomBound(SI4 Value);

  /* Prints this object to a terminal. */
  void Print() const;

 private:
  SI4 x_,              //< X value.
      y_,              //< Y value.
      left_bounds_,    //< Lower x bounds.
      right_bounds_,   //< Upper x bounds.
      top_bounds_,     //< Lower y bounds.
      bottom_bounds_;  //< Upper y bounds.
};
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_XYCONTROL
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
