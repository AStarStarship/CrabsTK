/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/Point.hpp
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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

#include "Vect.hpp"

namespace _2D {

struct _KabukiSDK Point_f
    /*< A 2D point storted as float types. */
{
    float x,    //< The X position.
        y;      //< The Y position.

    Point_f (float initX = 0, float initY = 0);
    /*< Default constructor initializes with given Point. */

    void set (float newX = 0, float newY = 0);
    /*< Sets the X and Y Values to the new Values. */

    void set (const Point_f& P);
    /*< Sets the X and Y Values to the this Point's X and Y. */

    void translate (const Vect_f& V);
    /** translates the given vector to the point. */

    void translate (float dx, float dy);
    /** translates the given vector to the point. */

    bool equals (const Point_f& P) const;
    /*< Compares this Point to the other and returns true if the two Positions are identical. */

    double compare (const Point_f& P) const;
    /*< Compares this Point to the other and the quadrant this point is in relation to the other. */

    void swap (Point_f P);
    /*< Swaps this object's Point with a. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

struct _KabukiSDK Point_d
/*< A 2D point storted as double types. */
{
    double x,   //< The X position.
        y;      //< The Y position.

    Point_d ();
    /*< Constructs an uninitialized . */

    Point_d (double initX = 0, double initY = 0);
    /*< Default constructor initializes with given Point. 
        @param initX The x value.
        @param initY The y value.

        @code
        @endcode
    */

    void set (double newX = 0, double newY = 0);
    /*< Sets the X and Y Values to the new Values. */

    void set (const Point_d& P);
    /*< Sets the X and Y Values to the this Point's X and Y. */

    void translate (const Vect_i& V);
    /** translates the given vector to the point.
        @param V The vector to translate the point by. */

    void translate (double dx, double dy);
    /** translates the given vector to the point. */

    bool equals (const Point_d& P) const;
    /*< Compares this Point to the other and returns true if the two Positions are identical. */

    double compare (const Point_d& P) const;
    /*< Compares this Point to the other and the quadrant this point is in relation to the other. */

    void swap (Point_d& P);
    /*< Swaps this object's Point with a. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace _2D
