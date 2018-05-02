/** Kabuki Toolkit
    @file    ~/kt/math/rect_rotor2d.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#include "quad2d.h"

namespace math {

/** Utility class for rotating rectangles.
*/
template<typename T>
class KABUKI_TOOLKIT Rect2DRotor
{
    public:

    /** Constructs a RectRotar with an unrotated rectangle. */
    Rect2DRotor (const Rect2D<T>& area) {
        // @todo Write me!
    }

    /** Gets the X value. */
    T GetX () const;

    /** Gets the X reference value. */
    T& GetX_ref ();

    /** Gets the Y value. */
    T GetY () const;

    /** Gets the Y reference value. */
    T& GetY_ref ();

    /** Gets the width. */
    T GetWidth () const;

    /** Gets the width reference. */
    T& GetWidth_ref ();

    /** Gets the height. */
    T GetHeight () const;

    /** Gets the height reference. */
    T& GetHeight_ref ();

    /** Gets the right x value. */
    T GetRight () const;

    /** Gets the  */
    Quad2D<T>& GetQuad_ref () const;

    /** Gets the bottom y value. */
    T GetBottom () const;

    /** Rotates the original vector and returns the outter bounds. */
    const Rect2D<T>& Rotate () const;

    /** Returns the outer bounds. */
    Rect2D<T> GetBounds () {
        T tempX1,
          tempY1,
          tempX2,
          tempY2,
          tempX3,
          tempY3,
          tempX4,
          tempY4;

        Rect2D<T> r (Floor (min (min (min (tempX1, tempX2), tempX3), tempX4)),
                  floor (min (min (min (tempY1, tempY2), tempY3), tempY4)),
                  ceiling (matempX (matempX (matempX (tempX1, tempX2), tempX3), tempX4)),
                  ceiling (matempX (matempX (matempX (tempY1, tempY2), tempY3), tempY4)));
        r.width -= r.x - 1;
        r.height -= r.y - 1;
        return r;
    }

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& print) {
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

}   //< math
