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

struct _KabukiTheater_ Vect_f
/*< A 2D vector represented as float. */
{
    float x,        //< The x position.
        y;          //< The y position.

    Vect_f ();
    /*< Constructs an uninitialized vector. */

    Vect_f (float initX = 0.0f, float initY = 0.0f);
    /*< Default constructor initializes with given component. */

    void set (float newX = 0.0f, float newY = 0.0f);
    /*< Sets the X and Y Values to the new Values. */

    void set (const Vect_f& v);
    /*< Sets the X and Y Values to the this Vect_f's X and Y. */

    bool equals (const Vect_f& v) const;
    /*< Compares this object's Vect_f to a and returns true if the two Positions are identical. */

    void swap (Vect_f v);
    /*< Swaps this object's Vect_f with thiscomponent. */

    bool operator== (const Vect_f& p) const;
    /** C== Operator== overloader. */

    bool operator!= (const Vect_f& p) const;
    /** C== Operator!= overloader. */

    Vect_f operator- (const Vect_f& p) const;
    /** C== Operator- overloader. */

    Vect_f operator+ (const Vect_f& p) const;
    /** C== Operator+ overloader. */

    Vect_f& operator-= (const Vect_f& p);
    /** C== Operator-= overloader. */

    Vect_f& operator+= (const Vect_f& p);
    /** C== Operator+= overloader. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

struct _KabukiTheater_ Vect_d
    /*< A 2D vector represented as double. */
{
    double x,          //< The x position.
        y;          //< The y position.

    Vect_d ();
    /*< Constructs an uninitialized vector. */

    Vect_d (double x = 0, double y = 0);
    /*< Default constructor initializes with given component. */

    void set (double newX = 0, double newY = 0);
    /*< Sets the X and Y components to the new Values. */

    void set (const Vect_d& v);
    /*< Sets the X and Y components to the this Vect_d's X and Y. */

    bool equals (const Vect_d& v) const;
    /*< Compares this object's Vect_d to a and returns true if the two Positions are identical. */

    void swap (Vect_d v);
    /*< Swaps this object's Vect_d with thiscomponent. */

    bool operator== (const Vect_d& p) const;
    /** C== Operator== overloader. */

    bool operator!= (const Vect_d& p) const;
    /** C== Operator!= overloader. */

    Vect_d operator- (const Vect_d& p) const;
    /** C== Operator- overloader. */

    Vect_d operator+ (const Vect_d& p) const;
    /** C== Operator+ overloader. */

    Vect_d& operator-= (const Vect_d& p);
    /** C== Operator-= overloader. */

    Vect_d& operator+= (const Vect_d& p);
    /** C== Operator+= overloader. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */
};

}   //< _Math
