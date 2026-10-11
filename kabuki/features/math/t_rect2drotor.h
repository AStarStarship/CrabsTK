/* Kabuki Toolkit @version 0.x_
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_rect2drotar.h
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
#ifndef KABUKI_FEATURES_MATH_RECT2DROTOR_T
#define KABUKI_FEATURES_MATH_RECT2DROTOR_T 1

#include "t_quad2d.h"

namespace _ {

/* Utility class for rotating rectangles. */
template<typename T>
class TRect2DRotor {
public:

  /* Constructs a RectRotar with an unrotated rectangle. */
  TRect2DRotor (const TRect2D<T>& area) {}

  /* Gets the X value. */
  T GetX () const;

  /* Gets the X reference value. */
  T& GetX_ref ();

  /* Gets the Y value. */
  T GetY () const;

  /* Gets the Y reference value. */
  T& GetY_ref ();

  /* Gets the width. */
  T Width () const;

  /* Gets the width reference. */
  T& GetWidth_ref ();

  /* Gets the height. */
  T Height () const;

  /* Gets the height reference. */
  T& GetHeight_ref ();

  /* Gets the right x value. */
  T GetRight () const;

  /* Gets the  */
  Quad2D<T>& GetQuad_ref () const;

  /* Gets the bottom y value. */
  T GetBottom () const;

  /* Rotates the original TArray and returns the outter bounds. */
  const TRect2D<T>& Rotate () const;

  /* Returns the outer bounds. */
  TRect2D<T> Bounds () {
    T tempX1,
      tempY1,
      tempX2,
      tempY2,
      tempX3,
      tempY3,
      tempX4,
      tempY4;

    TRect2D<T> r (Floor (min (min (min (tempX1, tempX2), tempX3), tempX4)),
      floor (min (min (min (tempY1, tempY2), tempY3), tempY4)),
      ceiling (matempX (matempX (matempX (tempX1, tempX2), tempX3), tempX4)),
      ceiling (matempX (matempX (matempX (tempY1, tempY2), tempY3), tempY4)));
    r.width -= r.x - 1;
    r.height -= r.y - 1;
    return r;
  }

  /* Prints this object to the terminal. */
  Printer& Print (Printer& print) {
    return print;
  }

private:

  T angle,        //< The angle rotated by.
    x,              //< The unrotated x coordinate.
    y,              //< The unrotated y coordinate.
    width,          //< The unrotated width.
    height,         //< The unrotated height.
    a,              //< The rotated x coordinate.
    b,              //< The rotated y coordinate.
    c,              //< The rotated x + width coordinate.
    d;              //< The rotated y + height coordinate.
};

}       //< namespace _
#endif
#endif
