/** Kabuki Theater
    @file    /.../Source/include/_Math/RectRotor.h
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

#include <_Math/RectRotar.h>
#include <_Math/Calc.h>
using namespace _Math;

namespace _Math {

RectRotor::RectRotor (bool rotated, const Rect_f& area)
{

}

Type RectRotor::getX () const;
/*< Gets the X value. */

Type& RectRotor::getX_ref ()
{
    return 
}

Type RectRotor::getY () const;
/*< Gets the Y value. */

Type& RectRotor::getY_ref ();
/*< Gets the Y reference value. */

Type RectRotor::getWidth () const;
/*< Gets the width. */

Type& RectRotor::getWidth_ref ();
/*< Gets the width reference. */

Type RectRotor::getHeight () const;
/*< Gets the height. */

Type& RectRotor::getHeight_ref ();
/*< Gets the height reference. */

Type RectRotor::getRight () const;
/*< Gets the right x value. */

Type RectRotar_f::getBottom () const
{

}

const Rect_f& RectRotor::rotate () const
{

}

Rect_f RectRotor::getBounds ()
{
    Type tempX1,
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

}   //  _Math
