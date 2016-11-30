/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/Point.hpp
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

#include "Vect.hpp"

namespace _2D {

struct _KabukiSDK Point_i
/*< A 2D point storted as int types. */
{
    int X,      //< The X position.
        Y;      //< The Y position.

    Point_i  ();
    /*< Constructs an uninitialized . */

    Point_i  (int initX = 0, int initY = 0);
    /*< Default constructor initializes with given Point. 
        @param initX The x value.
        @param initY The y value.

        @code
        @endcode
    */

    int getX () const;
    /*< gets the X component. */

    int getY () const;
    /*< gets the Y component. */

    void setX (int Value);
    /*< sets the X component to the new Value. */

    void setY (int Value);
    /*< sets the Y component to the new Value. */

    void set (int newX = 0, int newY = 0);
    /*< sets the X and Y Values to the new Values. */

    void set (const Point_i& P);
    /*< sets the X and Y Values to the this Point's X and Y. */

    void translate (const Vect_i& V);
    /** translates the given vector to the point.
        @param V The vector to translate the point by. */

    void translate (int dx, int dy);
    /** translates the given vector to the point. */

    bool equals (const Point_i& P) const;
    /*< Compares this Point to the other and returns true if the two Positions are identical. */

    int compare (const Point_i& P) const;
    /*< Compares this Point to the other and the quadrant this point is in relation to the other. */

    void swap (Point_i& P);
    /*< Swaps this object's Point with a. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

struct _KabukiSDK Point_f
/*< A 2D point storted as float types. */
{
    float X,    //< The X position.
        Y;      //< The Y position.

    Point_f  (float initX = 0.0f, float initY = 0.0f);
    /*< Default constructor initializes with given Point. */

    float getX () const;
    /*< gets the X component. */

    float getY () const;
    /*< gets the Y component. */

    void setX (float Value);
    /*< sets the X component to the new Value. */

    void setY (float Value);
    /*< sets the Y component to the new Value. */

    void set (float newX = 0.0f, float newY = 0.0f);
    /*< sets the X and Y Values to the new Values. */

    void set (const Point_f& P);
    /*< sets the X and Y Values to the this Point's X and Y. */

    void translate (const Vect_f& V);
    /** translates the given vector to the point. */

    void translate (float dx, float dy);
    /** translates the given vector to the point. */

    bool equals (const Point_f& P) const;
    /*< Compares this Point to the other and returns true if the two Positions are identical. */

    int compare (const Point_f& P) const;
    /*< Compares this Point to the other and the quadrant this point is in relation to the other. */

    void swap (Point_f P);
    /*< Swaps this object's Point with a. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace _2D
