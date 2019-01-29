/* Kabuki Toolkit @version 0.x_
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_vect3d.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_MATH_1
#ifndef KABUKI_FEATURES_MATH_VECT3D_T
#define KABUKI_FEATURES_MATH_VECT3D_T 1

namespace _ {

 /* A 3D TArray. */
template<typename T>
struct SDK TVect3D
{
    T x,    //< The x component.
      y,    //< The y component.
      z;    //< THe z component.
    
    /* Constructs an uninitialized TArray. */
    TVect3D () {
        /// Nothing to do here!
    }

    /* Default constructor initializes with given component. */
    TVect3D (T x, T y, T z) :
      x (x),
      y (y),
      z (z) {}

    /* Copy constructor. */
    TVect3D (const TVect3D<T>& v) :
        x (v.x),
        y (v.y),
        z (v.z) {}

    /* Sets the X and Y Values to the new Values. */
    void Set (T new_x = 0, T new_y = 0, T new_z = 0) {
        x = new_x;
        y = new_y;
        z = new_z;
    }

    /* Sets the X and Y Values to the this TVect3D's X and Y. */
    void Set (const TVect3D<T>& v) {
        x = v.x;
        y = v.y;
        y = v.y;
    } 

    /* Compares this object's TVect3D to a and returns true if the two Positions are identical. */
    BOL Equals (const TVect3D& v) const {
        if (x != v.x)
            return false;
        if (z != v.z)
            return false;
        if (z != v.z)
            return false;
        return true;
    }

    /* Swaps this object's TVect3D with thiscomponent. */
    void Swap (TVect3D v) {
        T temp_x = v.x,
            temp_y = v.y,
            temp_z = v.z;

        v.x = x;
        v.y = y;
        v.z = z;

        x = temp_x;
        y = temp_y;
        z = temp_z;
    }

    /* C== Operator== overloader. */
    BOL operator== (const TVect3D& v) const {
        if (x != v.x) return false;
        if (y != v.y) return false;
        if (z != v.z) return false;
        return true;
    }

    /* C== Operator!= overloader. */
    BOL operator!= (const TVect3D& v) const {
        if (x != v.x) return true;
        if (y != v.y) return true;
        if (z != v.z) return true;
        return false;
    }

    /* C== Operator- overloader. */
    TVect3D operator- (const TVect3D& v) const {
        return { x - v.x, y - v.y, z - v.z };
    }

    /* C== Operator+ overloader. */
    TVect3D operator+ (const TVect3D& v) const {
        return { x + v.x, y + v.y, z + v.z };
    }

    /* C== Operator-= overloader. */
    TVect3D& operator-= (const TVect3D& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    /* C== Operator+= overloader. */
    TVect3D& operator+= (const TVect3D& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    /* Prints this object to the stdout. */
    Printer& Print (Printer& printer) const {
        cout << "TVect3D: x=" << x << ", y=" << y << ", z=" << z << '\n';
    }
};

}       //< namespace _
#endif
#endif
