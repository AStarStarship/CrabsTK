/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_point2d.h
@author  Cale McCollough <calemccollough.github.io>
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
#if SEAM >= KABUKI_FEATURES_MATH_1
#ifndef KABUKI_FEATURES_MATH_POINT3D_T
#define KABUKI_FEATURES_MATH_POINT3D_T 1

#include "t_vect2d.h"

namespace _ {

/* A 2D point. */
template<typename T>
class TPoint2D {
public:

  T x,    //< X position.
    y;    //< Y position.

  /* Default constructor initializes with given TPoint2D. */
  TPoint2D (T x = 0, T y = 0) 
    : x (x),
      y (y) {}

  /* Sets the X and Y Values to the new Values. */
  void Set (T x = 0, T y = 0) {
    x = x;
    y = y;
  }

  /* Sets the X and Y Values to the this TPoint2D's X and Y. */
  void Set (const TPoint2D& p) {
    Set (o.X (), o.Y ())
  }

  /* Translates the given TArray to the point. */
  void Translate (const Vect<T>& v) {
    y += v.y;
    x += v.x;
  }

  /* Translates the given TArray to the point. */
  void Translate (T dx, T dy) {
    y += dy;
    x += dx;
  }

  /* Compares this TPoint2D to the other and returns true if the two Positions
  are identical. */
  BOL Equals (const TPoint2D& p) const {
    if (x != p.x || y != p.y)
      return false;
    return true;
  }

  /* Compares this TPoint2D to the other and the quadrant this point is in
  relation to the other.
  @return The quadrand p is in in relation to. */
  SIN Compare (const TPoint3D& o) const {
    if (x >= o.x)
      if (y >= o.y)
        return 1;
      else if (y < o.y)
        return 4;
    if (x < o.x)
      if (y > o.y)
        return 2;
      else if (y < o.y)
        return 3;
    return 0;   //< There are equal.
  }

  /* Swaps this object's TPoint2D with a. */
  void Swap (TPoint2D o) {
    T temp_x = o.x,
      temp_y = o.y;
    o.x = x;
    o.y = y;
    x = temp_x;
    y = temp_y;
  }

  /* Prints this object to the terminal. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    return o << "\nPoint2D: {x:" << x << ",y:" << y << '}';
  }
};

} //< namespace _
#endif
#endif
