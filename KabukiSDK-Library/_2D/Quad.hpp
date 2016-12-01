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

#include "Rect.hpp"

namespace _2D {

struct _KabukiSDK Quad_f
/*< A 2D quadralateral storted as float types. */
{
    float x1,   //< The x1 coordinate.
        x2,     //< The x2 coordinate.
        x3,     //< The x3 coordinate.
        x4,     //< The x4 coordinate.
        y1,     //< The y1 coordinate.
        y2,     //< The y2 coordinate.
        y3,     //< The y3 coordinate.
        y4;     //< The y3 coordinate.

    Quad_f ();
    /*< Constructs an uninitialized quadralateral. */

    Quad_f (float initX1, float initY1, float initX2, float initY2, float initX3, float initY3, float initX4, float initY4);
    /*< Default constructor initializes with given Point. */

    inline void set (float newX1, float newY1, float newX2, float newY2, float newX3, float newY3, float newX4, float newY4);
    /*< Sets the X and Y Values to the new values. */

    inline void set (float x, float y, float width, float height);
    /*< Sets the quad to a rectangle with the given coordinates. */

    inline void set (const Rect_f& r);
    /*< Sets the xn and yn values to the given rectangle. */

    inline void set (const Point_f& p, const Vect_f& v);
    /*< Sets the X and Y values to the the given point and vector. */

    inline void set (const Quad_f& q);
    /*< Sets the */

    inline void translate (const Vect_f& v);
    /** translates the given vector to the point. */

    inline void translate (float dx, float dy);
    /** translates the given vector to the point. */

    inline bool equals (const Quad_f& p) const;
    /*< Compares this object to the other and returns true if the two are identical. */

    inline int compare (const Quad_f& p) const;
    /*< Compares this object to the other and returns a numberetical zero true value. */

    inline void swap (Point_f p);
    /*< Swaps this object's Point with a. */

    inline void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

struct _KabukiSDK Quad_d
/*< A 2D quadralateral. */
{
    double x1,  //< The x1 coordinate.
        x2,     //< The x2 coordinate.
        x3,     //< The x3 coordinate.
        x4,     //< The x4 coordinate.
        y1,     //< The y1 coordinate.
        y2,     //< The y2 coordinate.
        y3,     //< The y3 coordinate.
        y4;     //< The y3 coordinate.

    Quad_d ();
    /*< Constructs an uninitialized quadralateral. */

    Quad_d (double initX1 = 0, double initY1 = 0, double initX2 = 0, double initY2 = 0, 
        double initX3 = 0, double initY3 = 0, double initX4 = 0, double initY4 = 0);
    /*< Default constructor initializes with given Point. 
        @param initX1 The x1 value.
        @param initY1 The y1 value.
        @param initX2 The x2 value.
        @param initY2 The y2 value.
        @param initX3 The x3 value.
        @param initY3 The y3 value.
        @param initX4 The x4 value.
        @param initY4 The y4 value.

        @code
        @endcode
    */

    inline void set (double newX = 0, double newY = 0);
    /*< Sets the X and Y Values to the new Values. */

    inline void set (const Quad_d& p);
    /*< Sets the X and Y Values to the this Point's X and Y. */

    inline void translate (const Vect_i& v);
    /** translates the given vector to the point.
        @param v The vector to translate the point by. */

    inline void translate (double dx, double dy);
    /** translates the given vector to the point. */

    inline bool equals (const Quad_d& p) const;
    /*< Compares this Point to the other and returns true if the two Positions are identical. */

    inline double compare (const Quad_d& p) const;
    /*< Compares this Point to the other and the quadrant this point is in relation to the other. */

    inline void swap (Point_d& p);
    /*< Swaps this object's Point with a. */

    inline void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace _2D
