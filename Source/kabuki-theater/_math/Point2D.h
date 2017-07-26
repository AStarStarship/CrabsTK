/** Kabuki Theater
    @file    /.../Source/_Math/Point2D.hpp
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

#include <KabukiTheater-Config.hpp>

#include "Vect.hpp"

namespace _Math {

template<typename Type>
class _KabukiTheater_ Point2D
    /*< A 2D point storted as Type types. */
{
    public:

    Type x,    //< The X position.
        y;      //< The Y position.

    Point2D (Type initX = 0, Type initY = 0);
    /*< Default constructor initializes with given Point2D. */

    void set (Type newX = 0, Type newY = 0);
    /*< Sets the X and Y Values to the new Values. */

    void set (const Point2D& P);
    /*< Sets the X and Y Values to the this Point2D's X and Y. */

    void translate (const Vect_f& V);
    /** translates the given vector to the point. */

    void translate (Type dx, Type dy);
    /** translates the given vector to the point. */

    bool equals (const Point2D& P) const;
    /*< Compares this Point2D to the other and returns true if the two Positions are identical. */

    double compare (const Point2D& P) const;
    /*< Compares this Point2D to the other and the quadrant this point is in relation to the other. */

    void swap (Point2D P);
    /*< Swaps this object's Point2D with a. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

}   //  _Math
