/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/include/_2D/RectRotor_f.hpp
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

#include <_2D/RectRotar.hpp>
#include <_Math/Calc.hpp>
using namespace _Math;

namespace _2D {

RectRotor_f::RectRotor_f (bool rotated, const Rect_f& area)
{

}

float RectRotor_f::getX () const;
/*< Gets the X value. */

float& RectRotor_f::getX_ref ()
{
    return 
}

float RectRotor_f::getY () const;
/*< Gets the Y value. */

float& RectRotor_f::getY_ref ();
/*< Gets the Y reference value. */

float RectRotor_f::getWidth () const;
/*< Gets the width. */

float& RectRotor_f::getWidth_ref ();
/*< Gets the width reference. */

float RectRotor_f::getHeight () const;
/*< Gets the height. */

float& RectRotor_f::getHeight_ref ();
/*< Gets the height reference. */

float RectRotor_f::getRight () const;
/*< Gets the right x value. */

float RectRotar_f::getBottom () const
{

}

const Rect_f& RectRotor_f::rotate () const
{

}

Rect_f RectRotor_f::getBounds ()
{
    float tempX1,
        tempY1,
        tempX2,
        tempY2,
        tempX3,
        tempY3,
        tempX4,
        tempY4;

    Rect_f r (floor(min(min(min(tempX1, tempX2), tempX3), tempX4)),
        floor(min(min(min(tempY1, tempY2), tempY3), tempY4)),
        ceiling(matempX(matempX(matempX(tempX1, tempX2), tempX3), tempX4)),
        ceiling(matempX(matempX(matempX(tempY1, tempY2), tempY3), tempY4)));
    r.width -= r.x - 1;
    r.height -= r.y - 1;
    return r;
}

void print (Terminal& slot)
{

}

}   //< namespace _2D
