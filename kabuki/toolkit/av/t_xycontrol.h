/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_xycontrol.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_XYCONTROL
#define KABUKI_TOOLKIT_AV_XYCONTROL

#include "t_config.h"

namespace _ {

class SDK XyTouchpad : public Component {
 public:
  /* Constructor. */
  XyTouchpad(SI4 x = 0, SI4 y = 0, SI4 left_bounds = 0,
             SI4 right_bounds = 0, SI4 top_bounds = 0,
             SI4 bottom_bounds = 0)
    : x_ (x),
    y_ (y),
    left_bounds_ (left_bounds),
    right_bounds_ (right_bounds),
    top_bounds_ (top_bounds),
    bottom_bounds_ (bottom_bounds) {
  }

  /* Copy constructor. */
  XyTouchpad(const XyTouchpad& other)
    : x_ (other.x_),
    y_ (other.y_),
    left_bounds_ (other.left_bounds_),
    right_bounds_ (other.right_bounds_),
    top_bounds_ (other.top_bounds_),
    bottom_bounds_ (other.bottom_bounds_) {
  }

  /* Gets thisX value. */
  SI4 GetX() { return x_; }

  /* Gets thisY value. */
  SI4 GetY() { return y_; }

  /* Sets thisX to the value. */
  void SetX(SI4 value) {
    SI4 bounds = left_bounds_;
    if (value <= left_bounds_)
      x_ = bounds;
    else if (value >= (bounds = right_bounds_))
      x_ = bounds;
    else
      x_ = value;
  }

  /* Sets thisY to the value. */
  void SetY(SI4 value) {
    SI4 bounds = bottom_bounds_;
    if (value <= bounds)
      y_ = bounds;
    else if (value >= (bounds = top_bounds_))
      y_ = bounds;
    else
      y_ = value;
  }

  /* Gets th left_bounds. */
  SI4 GetLeftBounds() { return left_bounds_; }

  /* Gets th right_bounds. */
  SI4 GetRightBounds() { return right_bounds_; }

  /* Gets th top_bounds. */
  SI4 GetTopBounds() { return top_bounds_; }

  /* Gets th bottom_bounds. */
  SI4 GetBottomBound() { return bottom_bounds_; }

  /* Sets the left_bounds to the newBouds. */ 
    BOL SetLeftBounds(SI4 value) {
      if (value >= right_bounds_) return false;
      left_bounds_ = value;
      return true;
    }

  /* Sets the right_bounds to the new value. */
  BOL SetRightBounds(SI4 value) {
    if (value <= right_bounds_) return false;
    right_bounds_ = value;
    return true;
  }

  /* Sets the top_bounds to the newBouds
  @pre Value > bottom_bounds. */
  BOL SetTopBounds(SI4 value) {
    if (value >= top_bounds_) return false;
    top_bounds_ = value;
    return true;
  }

  /* Sets the bottom_bounds to the newBouds
  @pre Value < top_bounds. */
  BOL SetBottomBounds(SI4 value) {
    if (value >= bottom_bounds_) return false;
    bottom_bounds_ = value;
    return true;
  }

  /* Prints this object to a terminal. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nXYControl:\nx: " << x_ << "  y_: " << y_;
  }

 private:
  SI4 x_,              //< X value.
      y_,              //< Y value.
      left_bounds_,    //< Lower x bounds.
      right_bounds_,   //< Upper x bounds.
      top_bounds_,     //< Lower y bounds.
      bottom_bounds_;  //< Upper y bounds.
};
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_XYCONTROL
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
