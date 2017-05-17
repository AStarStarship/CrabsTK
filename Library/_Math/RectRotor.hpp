/** Kabuki Theater
    @file    /.../KabukiTheater/include/_Math/RectRotor_f.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include "_Math/Quad.hpp"

namespace _Math {

class _KabukiTheater RectRotor_f
/*< Utility class for rotating rectangles.
*/
{
    public:
    
    RectRotor_f (const Rect_f& area);
    /*< Constructs a RectRotar with an unrotated rectangle. */

    float getX () const;
    /*< Gets the X value. */

    float& getX_ref ();
    /*< Gets the X reference value. */

    float getY () const;
    /*< Gets the Y value. */

    float& getY_ref ();
    /*< Gets the Y reference value. */

    float getWidth () const;
    /*< Gets the width. */

    float& getWidth_ref ();
    /*< Gets the width reference. */

    float getHeight () const;
    /*< Gets the height. */

    float& getHeight_ref ();
    /*< Gets the height reference. */

    float getRight () const;
    /*< Gets the right x value. */

    Quad_f& getQuad_ref () const;
    /*< Gets the  */

    float getBottom () const;
    /*< Gets the bottom y value. */

    const Rect_f& rotate () const;
    /*< Rotates the original vector and returns the outter bounds. */

    Rect_f getBounds ();
    /*< Returns the outer bounds. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */

    private:

    float angle,        //< The angle rotated by.
        x,              //< The unrotated x coordinate.
        y,              //< The unrotated y coordinate.
        width,          //< The unrotated width.
        height,         //< The unrotated height.
        a,              //< The rotated x coordinate.
        b,              //< The rotated y coordinate.
        c,              //< The rotated x + width coordinate.
        d;              //< The rotated y + height coordinate.
};

}   //< _Math
