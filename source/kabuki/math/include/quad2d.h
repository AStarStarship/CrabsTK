/** Kabuki Starship
    @file    /.../Source/_Math/Point.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include "Rect.h"

namespace _Math {

template<typename Type>
struct _KabukiTheater_ Quad
/*< A 2D quadralateral storted as Type types. */
{
    Type x0,   //< The x0 coordinate.
        x1,     //< The x1 coordinate.
        x2,     //< The x2 coordinate.
        x3,     //< The x3 coordinate.
        y0,     //< The y0 coordinate.
        y1,     //< The y1 coordinate.
        y2,     //< The y2 coordinate.
        y3;     //< The y2 coordinate.

    Quad ();
    /*< Constructs an uninitialized quadralateral. */

    Quad (Type initX0, Type initY0, Type initX1, Type initY1, Type initX2, Type initY2, Type initX3, 
        Type initY3);
    /*< Default constructor initializes with given Point. */

    inline void set (Type newX0, Type newY0, Type newX1, Type newY1, Type newX2, Type newY2, Type newX3, 
        Type newY3);
    /*< Sets the X and Y Values to the new values. */

    inline void set (Type x, Type y, Type width, Type height);
    /*< Sets the quad to a rectangle with the given coordinates. */

    inline void set (const Rect_f& r);
    /*< Sets the xn and yn values to the given rectangle. */

    inline void set (const Point_f& p, const Vect_f& v);
    /*< Sets the X and Y values to the the given point and vector. */

    inline void set (const Quad& q);
    /*< Sets the */

    inline void translate (Type dx, Type dy);
    /** translates the given vector to the point. */

    inline void translate (const Vect_f& v);
    /** translates the given vector to the point. */

    inline bool equals (const Quad& p) const;
    /*< Compares this object to the other and returns true if the two are identical. */

    inline int compare (const Quad& p) const;
    /*< Compares this object to the other and returns a numberetical zero true value. */

    inline void swap (Quad p);
    /*< Swaps this object's Point with a. */

    inline void print (Expression& io);
    /*< Prints this object to the terminal. */
};

}   //< _Math
