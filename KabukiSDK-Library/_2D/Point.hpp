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
    /*< Default constructor initializes with given Point. */

    Point_i  (int initX = 0, int initY = 0);
    /*< Default constructor initializes with given Point. 
        @param initX The x value.
        @param initY The y value.

        @code
        find_package(KF5Archive)
target_link_libraries(yourapp KF5::Archive)
        @endcode
        */

    int GetX () const;
    /*< Gets the X component. */

    int GetY () const;
    /*< Gets the Y component. */

    void SetX (int Value);
    /*< Sets the X component to the new Value. */

    void SetY (int Value);
    /*< Sets the Y component to the new Value. */

    void Set (int newX = 0, int newY = 0);
    /*< Sets the X and Y Values to the new Values. */

    void Set (const Point_i& P);
    /*< Sets the X and Y Values to the this Point's X and Y. */

    void Translate (const Vect_i& V);
    /** Translates the given vector to the point.
        @param V The vector to translate the point by. */

    void Translate (int DeltaX, int DeltaY);
    /** Translates the given vector to the point. */

    bool Equals (const Point_i& P) const;
    /*< Compares this Point to the other and returns true if the two Positions are identical. */

    int Compare (const Point_i& P) const;
    /*< Compares this Point to the other and the quadrant this point is in relation to the other. */

    void Swap (Point_i& P);
    /*< Swaps this object's Point with a. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

vector<_2D::Point_i> points;

Array points (sizeof (_2D::Point_i))

struct _KabukiSDK Point_f
/*< A 2D point storted as float types. */
{
    float X,    //< The X position.
        Y;      //< The Y position.

    Point_f  (float initX = 0.0f, float initY = 0.0f);
    /*< Default constructor initializes with given Point. */

    float GetX () const;
    /*< Gets the X component. */

    float GetY () const;
    /*< Gets the Y component. */

    void SetX (float Value);
    /*< Sets the X component to the new Value. */

    void SetY (float Value);
    /*< Sets the Y component to the new Value. */

    void Set (float newX = 0.0f, float newY = 0.0f);
    /*< Sets the X and Y Values to the new Values. */

    void Set (const Point_f& P);
    /*< Sets the X and Y Values to the this Point's X and Y. */

    void Translate (const Vect_f& V);
    /** Translates the given vector to the point. */

    void Translate (float DeltaX, float DeltaY);
    /** Translates the given vector to the point. */

    bool Equals (const Point_f& P) const;
    /*< Compares this Point to the other and returns true if the two Positions are identical. */

    int Compare (const Point_f& P) const;
    /*< Compares this Point to the other and the quadrant this point is in relation to the other. */

    void Swap (Point_f P);
    /*< Swaps this object's Point with a. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace _2D
