/** Kabuki Toolkit
    @file    ~/kt/math/point2d.h
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
#include "vect2d.h"

namespace _ {

/** A 2D point storted as Type types. */
template<typename T>
class KABUKI_TOOLKIT Point2D {
    public:

    T x,    //< The X position.
      y;    //< The Y position.

    /** Default constructor initializes with given Point2D. */
    Point2D (T initX = 0, T initY = 0) :
        x (x),
        y (y)
    {
        /// Nothing to do here!
    }

    /** Sets the X and Y Values to the new Values. */
    void Set (T newX = 0, T newY = 0) {
        x = x;
        y = y;
    }

    /** Sets the X and Y Values to the this Point2D's X and Y. */
    void Set (const Point2D& P) {
        x = P.x;
        y = P.y;
    }

    /** Translates the given vector to the point. */
    void Translate (const Vect<T>& V) {
        y += V.y;
        x += V.x;
    }

    /** Translates the given vector to the point. */
    void Translate (T dx, T dy) {
        y += dy;
        x += dx;
    }

    /** Compares this Point2D to the other and returns true if the two Positions are identical. */
    bool Equals (const Point2D& P) const {
        if (x != P.x || y != P.y)
            return false;
        return true;
    }

    /** Compares this Point2D to the other and the quadrant this point is in relation to the other. */
    double Compare (const Point2D& P) const {
        if (x >= P.x)
            if (y >= P.y)
                return 1;
            else if (y < P.y)
                return 4;
        if (x < P.x)
            if (y > P.y)
                return 2;
            else if (y < P.y)
                return 3;
        return 0;   //< There are equal.
    }

    /** Swaps this object's Point2D with a. */
    void Swap (Point2D P) {
        T tempX = P.x,
            tempY = P.y;

        P.x = x;
        P.y = y;

        x = tempX;
        y = tempY;
    }

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& printer) {
        cout << "Point2D: x=" << x << ", y=" << y << '\n';
    }
};

}       //< namespace _
