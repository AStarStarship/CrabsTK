/* Kabuki Toolkit
@file    ~/source/hmi/hmi_xy_control.h
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

#pragma once
#include <pch.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_KT_HMI_XYCONTROL
#define HEADER_FOR_KT_HMI_XYCONTROL

#include "config.h"

namespace _ {

class KABUKI XyTouchpad : public Component {
 public:
  /* Constructor. */
  XyTouchpad(int init_x = 0, int init_y = 0, int init_left_bounds = 0,
             int init_right_bounds = 0, int initTopBounds = 0,
             int initBottomBounds = 0);

  /* Copy constructor. */
  XyTouchpad(const XyTouchpad& O);

  /* Gets thisX value. */
  int GetX();

  /* Gets thisY value. */
  int GetY();

  /* Sets thisX to the value. */
  void SetX(int value);

  /* Sets thisY to the value. */
  void SetY(int value);

  /* Gets th left_bounds. */
  int GetLeftBounds();

  /* Gets th right_bounds. */
  int GetRightBounds();

  /* Gets th top_bounds. */
  int GetTopBounds();

  /* Gets th bottom_bounds. */
  int GetBottomBound();

  /* Sets the left_bounds to the newBouds. */
  @pre Value < right_bounds.* / bool SetLeftBounds(int Value);

  /* Sets the right_bounds to the newBouds
      @pre Value > left_bounds    */
  bool SetRightBounds(int Value);

  /* Sets the top_bounds to the newBouds
      @pre Value > bottom_bounds. */
  bool SetTopBounds(int Value);

  /* Sets the bottom_bounds to the newBouds
      @pre Value < top_bounds. */
  bool SetBottomBound(int Value);

  /* Prints this object to a terminal. */
  void Print() const;

 private:
  int x_,              //< X value.
      y_,              //< Y value.
      left_bounds_,    //< Lower x bounds.
      right_bounds_,   //< Upper x bounds.
      top_bounds_,     //< Lower y bounds.
      bottom_bounds_;  //< Upper y bounds.
};
}       // namespace _
#endif  //< HEADER_FOR_KT_HMI_XYCONTROL
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
