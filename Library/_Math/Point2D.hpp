/** Kabuki Theater
    @file    /.../KabukiTheater/_Math/Point2D.hpp
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

#include <KabukiTheater-Config.hpp>

#include "Vect.hpp"

namespace _Math {

class _KabukiTheater Point2D_f
    /*< A 2D point storted as float types. */
{
    public:

    float x,    //< The X position.
        y;      //< The Y position.

    Point2D_f (float initX = 0, float initY = 0);
    /*< Default constructor initializes with given Point2D. */

    void set (float newX = 0, float newY = 0);
    /*< Sets the X and Y Values to the new Values. */

    void set (const Point2D_f& P);
    /*< Sets the X and Y Values to the this Point2D's X and Y. */

    void translate (const Vect_f& V);
    /** translates the given vector to the point. */

    void translate (float dx, float dy);
    /** translates the given vector to the point. */

    bool equals (const Point2D_f& P) const;
    /*< Compares this Point2D to the other and returns true if the two Positions are identical. */

    double compare (const Point2D_f& P) const;
    /*< Compares this Point2D to the other and the quadrant this point is in relation to the other. */

    void swap (Point2D_f P);
    /*< Swaps this object's Point2D with a. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

class _KabukiTheater Point2D_d
/*< A 2D point storted as double types. */
{
    public:

    double x,   //< The X position.
        y;      //< The Y position.

    Point2D_d ();
    /*< Constructs an uninitialized . */

    Point2D_d (double initX = 0, double initY = 0);
    /*< Default constructor initializes with given Point2D. 
        @param initX The x value.
        @param initY The y value.

        @code
        @endcode
    */

    void set (double newX = 0, double newY = 0);
    /*< Sets the X and Y Values to the new Values. */

    void set (const Point2D_d& P);
    /*< Sets the X and Y Values to the this Point2D's X and Y. */

    void translate (const Vect_d& V);
    /** translates the given vector to the point.
        @param V The vector to translate the point by. */

    void translate (double dx, double dy);
    /** translates the given vector to the point. */

    bool equals (const Point2D_d& P) const;
    /*< Compares this Point2D to the other and returns true if the two Positions are identical. */

    double compare (const Point2D_d& P) const;
    /*< Compares this Point2D to the other and the quadrant this point is in relation to the other. */

    void swap (Point2D_d& P);
    /*< Swaps this object's Point2D with a. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

#if _WordSize == 32
using Point2D = Point2D_f;
#elif _WordSize == 64
using Point2D = Point2D_d;
#endif

}   //  _Math
