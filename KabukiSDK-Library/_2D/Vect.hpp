/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/Vect.hpp
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

namespace _2D {

struct _KabukiSDK Vect_i
/*< A 2D vector represented as int. */
{
    int X,          //< The x position.
        Y;          //< The y position.

    Vect_i ();
    /*< Constructs an uninitialized vector. */

    Vect_i  (int x = 0, int y = 0);
    /*< Default constructor initializes with given component. */

    int getX ();
    /*< gets the X component. */

    int getY ();
    /*< gets the Y component. */

    void setX (int Value);
    /*< sets the X component to the new Value. */

    void setY (int Value);
    /*< sets the Y component to the new Value. */

    void set (int NewX = 0, int NewY = 0);
    /*< sets the X and Y components to the new Values. */

    void set (const Vect_i& V);
    /*< sets the X and Y components to the this Vect_i's X and Y. */

    bool equals (const Vect_i& V) const;
    /*< Compares this object's Vect_i to a and returns true if the two Positions are identical. */

    void swap (Vect_i V);
    /*< Swaps this object's Vect_i with thiscomponent. */

    bool operator== (const Vect_i& P) const;
    /** C== Operator== overloader. */

    bool operator!= (const Vect_i& P) const;
    /** C== Operator!= overloader. */

    Vect_i operator- (const Vect_i& P) const;
    /** C== Operator- overloader. */

    Vect_i operator+ (const Vect_i& P) const;
    /** C== Operator+ overloader. */

    Vect_i& operator-= (const Vect_i& P);
    /** C== Operator-= overloader. */

    Vect_i& operator+= (const Vect_i& P);
    /** C== Operator+= overloader. */
};

struct _KabukiSDK Vect_f
/*< A 2D vector represented as float. */
{
    float X,        //< The x position.
        Y;          //< The y position.

    Vect_f ();
    /*< Constructs an uninitialized vector. */

    Vect_f (float InitX = 0.0f, float InitY = 0.0f);
    /*< Default constructor initializes with given component. */

    float getX ();
    /*< gets the X component. */

    float getY ();
    /*< gets the Y component. */

    void setX (float Value);
    /*< sets the X component to the new Value. */

    void setY (float Value);
    /*< sets the Y component to the new Value. */

    void set (float NewX = 0.0f, float NewY = 0.0f);
    /*< sets the X and Y Values to the new Values. */

    void set (const Vect_f& V);
    /*< sets the X and Y Values to the this Vect_f's X and Y. */

    bool equals (const Vect_f& V) const;
    /*< Compares this object's Vect_f to a and returns true if the two Positions are identical. */

    void swap (Vect_f V);
    /*< Swaps this object's Vect_f with thiscomponent. */

    bool operator== (const Vect_f& P) const;
    /** C== Operator== overloader. */

    bool operator!= (const Vect_f& P) const;
    /** C== Operator!= overloader. */

    Vect_f operator- (const Vect_f& P) const;
    /** C== Operator- overloader. */

    Vect_f operator+ (const Vect_f& P) const;
    /** C== Operator+ overloader. */

    Vect_f& operator-= (const Vect_f& P);
    /** C== Operator-= overloader. */

    Vect_f& operator+= (const Vect_f& P);
    /** C== Operator+= overloader. */
};

}   //< namespace _2D
