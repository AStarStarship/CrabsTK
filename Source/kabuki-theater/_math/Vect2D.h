/** Kabuki Theater
    @file    /.../Source/_Math/Vect.hpp
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

namespace _Math {

template<typename T>
struct _KabukiTheater_ Vect2D
/*< A 2D vector. */
{
    T x,            //< The x component.
        y;          //< The y component.

    Vect2D ();
    /*< Constructs an uninitialized vector. */

    Vect2D (T x, T y);
    /*< Default constructor initializes with given component. */

    Vect2D (const Vect2D<T>& v);
    /*< Copy constructor. */

    void set (T newX = 0, T newY = 0);
    /*< Sets the X and Y Values to the new Values. */

    void set (const Vect2D& v);
    /*< Sets the X and Y Values to the this Vect2D's X and Y. */

    bool equals (const Vect2D& v) const;
    /*< Compares this object's Vect2D to a and returns true if the two Positions are identical. */

    void swap (Vect2D v);
    /*< Swaps this object's Vect2D with thiscomponent. */

    bool operator== (const Vect2D& v) const;
    /** C== Operator== overloader. */

    bool operator!= (const Vect2D& v) const;
    /** C== Operator!= overloader. */

    Vect2D operator- (const Vect2D& v) const;
    /** C== Operator- overloader. */

    Vect2D operator+ (const Vect2D& v) const;
    /** C== Operator+ overloader. */

    Vect2D& operator-= (const Vect2D& v);
    /** C== Operator-= overloader. */

    Vect2D& operator+= (const Vect2D& v);
    /** C== Operator+= overloader. */

    void print () const;
    /*< Prints this object to the stdout. */
};

}   //< _Math
