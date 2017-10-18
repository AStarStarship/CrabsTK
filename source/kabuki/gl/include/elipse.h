/** Kabuki Starship
    @file    /.../Source/KabukiTheater-Impl/_G/Elipse.h
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

#include "Cell.h"
#include <_Math/Point2D.h>

namespace _G {

template<typename Type>
class _KabukiTheater_ Elipse
/*< A graphical eliplse. */
{
    public:

    Type x,         //< The left edge.
        y,          //< The top edge.
    
    Elipse_f (_2D::Point_f relativePosition, _2D::Vect_f dimensions);
    /*< Constructor. */

    Type getRX ();
    /*< Gets the radius of the rectangle in the x direction. */

    void setRX (Type value);
    /*< Sets the radius of the rectangle in the x direction. 
        @warning If value < 0.0, value will be multiplied by -1.0. */

    Type getRY ();
    /*< Gets the radius of the rectangle in the x direction. */

    void setRY (Type value);
    /*< Sets the radius of the rectangle in the x direction.
        @warning If value < 0.0, value will be multiplied by -1.0. */

    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    Type rx,        //< The radius of the rectangle in the x direction.
        ry;         //< The radius of the rectangle in the y direction.
};

}   //< _G
