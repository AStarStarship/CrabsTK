/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/include/_2D/RectRotor_f.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>
#include "_2D/Rect.hpp"

namespace _2D {

class _KabukiSDK RectRotor_f
/*< Utility class for rotating rectangles. 
*/
{
    public:
    
    RectRotor_f  (bool rotated, const Rect_f& area);
    /*< Constructs a RectRotar with an unrotated rectangle. */

    float getX () const;
    /*< Gets the X value. */

    float& getX_ref ();
    /*< Gets the X reference value. */

    float getY () const;
    /*< Gets the Y value. */

    float& getY_ref ();
    /*< Gets the Y reference value. */

    unsigned getWidth () const;
    /*< Gets the width. */

    unsigned& getWidth_ref ();
    /*< Gets the width reference. */

    unsigned getHeight () const;
    /*< Gets the height. */

    unsigned& getHeight_ref ();
    /*< Gets the height reference. */

    float getRight () const;
    /*< Gets the right x value. */

    float getBottom () const;
    /*< Gets the bottom y value. */

    const Rect_f& rotate () const;
    /*< Rotates the original vector and returns the outter bouncs

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */

    private:

    float angle;        //< The angle rotated by.

    Vect_f vector;      //< The unrotated vector.

    Rect_f area;        //< The rotated rectangle.
};  //< class _KabukiSDK RectRotor_f

}   //< namespace _2D
