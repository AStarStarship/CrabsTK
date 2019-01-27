/** Kabuki Toolkit
    @file    ~/kt/math/vect2d.h
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
#include <pch.h>

#include "config.h"

namespace _ {

/** A 2D TArray. */
template<typename T>
struct KABUKI_THEATER Vect2D {
    T x, //< X component.
      y; //< Y component.
    
    /*< Constructs an uninitialized TArray. */
    Vect2D () {
        /// Nothing to do here!
    }
    
    /*< Default constructor initializes with given component. */
    Vect2D (T x, T y) :
        x (x),
        y (y) {
    }

    /*< Copy constructor. */
    Vect2D (const Vect2D<T>& v) :
        x (v.x),
        y (v.y) {

    }

    /*< Sets the X and Y Values to the new Values. */
    void Set (T newX = 0, T newY = 0) {
        x = newX;
        y = newY;
    }

    /*< Sets the X and Y Values to the this Vect2D's X and Y. */
    void Set (const Vect2D<T>& v) {
        x = v.x;
        y = v.y;
    }

    /*< Compares this object's Vect2D<T> to a and returns true if the two Positions are identical. */
    BOL Equals (const Vect2D<T>& v) const {
        if (x != v.x || y != v.y)
            return false;
        return true;
    }

    /*< Swaps this object's Vect2D<T> with thiscomponent. */
    void Swap (Vect2D<T> v) {
        T tempX = v.x,
            tempY = v.y;

        v.x = x;
        v.y = y;

        x = tempX;
        y = tempY;
    }

    /** C++ Operator== overloader. */
    BOL operator== (const Vect2D<T>& v) const {
        return ((x == v.x) && (y == v.y));
    }

    /** C++ Operator!= overloader. */
    BOL operator!= (const Vect2D<T>& v) const {
        return ((x != v.x) || (y != v.y));
    }

    /** C++ Operator- overloader. */
    Vect2D<T> operator- (const Vect2D<T>& v) const {
        return { x - v.x, y - v.y };
    }

    /** C++ Operator+ overloader. */
    Vect2D<T> operator+ (const Vect2D<T>& v) const {
        return { x + v.x, y + v.y };
    }

    /** C++ Operator-= overloader. */
    Vect2D<T>& operator-= (const Vect2D<T>& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    /** C++ Operator+= overloader. */
    Vect2D<T>& operator+= (const Vect2D<T>& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    /** Prints this object to the stdout. */
    _::Printer Print (_::Printer& print) const {
        print << "\nVect2D: x=" << x << ", y=" << y;
    }
};

}       //< namespace _
