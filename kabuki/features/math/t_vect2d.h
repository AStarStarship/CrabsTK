/* Kabuki Toolkit @version 0.x_
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_vect2d.h
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
#ifndef KABUKI_FEATURES_MATH_VECT2D_T
#define KABUKI_FEATURES_MATH_VECT2D_T 1


namespace _ {

/* A 2D vector. */
template<typename T>
struct SDK TVect2D {
    T x,  //< X component.
      y;  //< Y component.
    
    /* Constructs an uninitialized TArray. */
    TVect2D () {}
    
    /* Default constructor initializes with given component. */
    TVect2D (T x, T y) :
        x (x),
        y (y) {
    }

    /* Copy constructor. */
    TVect2D (const TVect2D<T>& v) :
        x (v.x),
        y (v.y) {

    }

    /* Sets the X and Y Values to the new Values. */
    void Set (T new_x = 0, T new_y = 0) {
        x = new_x;
        y = new_y;
    }

    /* Sets the X and Y Values to the this TVect2D's X and Y. */
    void Set (const TVect2D<T>& v) {
        x = v.x;
        y = v.y;
    }

    /* Compares this object's TVect2D<T> to a and returns true if the two
    Positions are identical. */
    BOL Equals (const TVect2D<T>& v) const {
        if (x != v.x || y != v.y)
            return false;
        return true;
    }

    /* Swaps this object's TVect2D<T> with thiscomponent. */
    void Swap (TVect2D<T> v) {
        T temp_x = v.x,
            temp_y = v.y;

        v.x = x;
        v.y = y;

        x = temp_x;
        y = temp_y;
    }

    /* C++ Operator== overloader. */
    BOL operator== (const TVect2D<T>& v) const {
        return ((x == v.x) && (y == v.y));
    }

    /* C++ Operator!= overloader. */
    BOL operator!= (const TVect2D<T>& v) const {
        return ((x != v.x) || (y != v.y));
    }

    /* C++ Operator- overloader. */
    TVect2D<T> operator- (const TVect2D<T>& v) const {
        return { x - v.x, y - v.y };
    }

    /* C++ Operator+ overloader. */
    TVect2D<T> operator+ (const TVect2D<T>& v) const {
        return { x + v.x, y + v.y };
    }

    /* C++ Operator-= overloader. */
    TVect2D<T>& operator-= (const TVect2D<T>& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    /* C++ Operator+= overloader. */
    TVect2D<T>& operator+= (const TVect2D<T>& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    /* Prints this object to the stdout. */
    Printer Print (Printer& print) const {
        print << "\nVect2D: x=" << x << ", y=" << y;
    }
};

}       //< namespace _
#endif
#endif
