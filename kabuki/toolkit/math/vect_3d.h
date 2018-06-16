/** Kabuki Toolkit
    @file ~/kt/math/vect2d.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software 
             distributed under the License is distributed on an "AS IS" BASIS, 
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#pragma once
#include <stdafx.h>

#include "config.h"

namespace _ {

 /** A 3D vector. */
template<typename T>
struct KABUKI_TOOLKIT Vect3D
{
    T x,            //< The x component.
      y,          //< The y component.
      z;          //< THe z component.
    
    /** Constructs an uninitialized vector. */
    Vect3D () {
        /// Nothing to do here!
    }

    /** Default constructor initializes with given component. */
    Vect3D (T x, T y, T z) :
        x (x),
        y (y),
        z (z)
    {
    }

    /** Copy constructor. */
    Vect3D (const Vect3D<T>& v) :
        x (v.x),
        y (v.y),
        z (v.z)
    {

    }

    /** Sets the X and Y Values to the new Values. */
    void Set (T newX = 0, T newY = 0, T newZ = 0) {
        x = newX;
        y = newY;
        z = newZ;
    }

    /** Sets the X and Y Values to the this Vect3D's X and Y. */
    void Set (const Vect3D<T>& v) {
        x = v.x;
        y = v.y;
        y = v.y;
    } 

    /** Compares this object's Vect3D to a and returns true if the two Positions are identical. */
    bool Equals (const Vect3D& v) const {
        if (x != v.x)
            return false;
        if (z != v.z)
            return false;
        if (z != v.z)
            return false;
        return true;
    }

    /** Swaps this object's Vect3D with thiscomponent. */
    void Swap (Vect3D v) {
        T tempX = v.x,
            tempY = v.y,
            tempZ = v.z;

        v.x = x;
        v.y = y;
        v.z = z;

        x = tempX;
        y = tempY;
        z = tempZ;
    }

    /** C== Operator== overloader. */
    bool operator== (const Vect3D& v) const {
        if (x != v.x) return false;
        if (y != v.y) return false;
        if (z != v.z) return false;
        return true;
    }

    /** C== Operator!= overloader. */
    bool operator!= (const Vect3D& v) const {
        if (x != v.x) return true;
        if (y != v.y) return true;
        if (z != v.z) return true;
        return false;
    }

    /** C== Operator- overloader. */
    Vect3D operator- (const Vect3D& v) const {
        return { x - v.x, y - v.y, z - v.z };
    }

    /** C== Operator+ overloader. */
    Vect3D operator+ (const Vect3D& v) const {
        return { x + v.x, y + v.y, z + v.z };
    }

    /** C== Operator-= overloader. */
    Vect3D& operator-= (const Vect3D& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    /** C== Operator+= overloader. */
    Vect3D& operator+= (const Vect3D& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    /** Prints this object to the stdout. */
    _::Printer& Print (_::Printer& printer) const {
        cout << "Vect3D: x=" << x << ", y=" << y << ", z=" << z << '\n';
    }
};

}       //< namespace _
