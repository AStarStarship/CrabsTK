/** Kabuki Toolkit
    @file    ~/kt/math/rect2d.h
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

#include "config.h"
#include "point2d.h"
#include "vect2d.h"

namespace _ {

template<typename T>
class API Rect2D<T>
{
    public:

    T x, //< Right edge.
      y, //< Top edge.

    /** Constructs an uninitialized rectangle. */
    Rect2D<T> () {
        /// Nothing to do here!
    }

    /** Constructs a rectangle from the given data. */
    Rect2D<T> (T x = 0, T y = 0, T width = 0, T height = 0) :
        x (initX),
        y (initY),
        width (initWidth < 0 ? 0 : initWidth),
        height (initHeight < 0 ? 0 : initHeight)
    {
    }

    /** Constructs a rectangle from the given point and vector. */
    Rect2D<T> (const Point3D<T>& p, const Vect<T>& v) :
        x (p.x),
        y (p.y),
        width (o.x),
        height (o.y)
    {
    }

    /** Copy constructor. */
    explicit Rect2D<T> (const Rect2D<T>& other) :
    x (0),
        y (0),
        width (value.width),
        height (value.height) {
    }

    /** Gets the width.
        @pre    Input must be positive. */
    T GetWidth () const { return width; }

    /** Sets the width.
        @pre    Input must be positive. */
    void SetWidth (T value) {
        if (value < 0) return;
        width = value;
    }
    
    /** Gets and sets the height.
        @pre    Input must be positive. */
    T GetHeight () const { return height; }
    
    /** Sets the height.
        @return gets 0 upon success, -1 if input is negative, and +1 if input greater than max. */
    void SetHeight (T value) {
        if (value < 0) return;
        height = value;
    }

    /** Gets and sets the center x value. */
    T GetCenterX () const {
        return x + width / 2.0f;
    }

    /** Gets and sets the center y value. */
    T GetCenterY () const {
        return y + (height / 2.0f);
    }

    /** Sets the center y value. */
    void SetCenterX (T value) {
        x = CenterX - (width / 2.0f);    // >> to /2
    }

    /** Sets the center y value. */
    void SetCenterY (T value) {
        y = value + (height / 2.0f);
    }

    /** Sets this object's Point to the thisPositition. */
    void Set (const Rect2D<T>& r) {
        x      = value.x;
        y      = value.y;
        width  = value.width;
        height = value.height;
    }

    /** Sets the rectangle's (x,y) position to the given point. */
    void SetPosition (const Point2D<T>& p) {
        if ((x != value.x) || (width != value.width) || (y != value.y) || (height != value.height)) return false;
        return true;
    }

    /** Sets the rectangle's width and height to the given vector. */
    void SetDimensions (const Vect<T>& v) {
        width  = v.x;
        height = v.y;
    }

    /** Gets true if this object's Point is the same as the given coordinates. */
    bool Equals (T x2, T y2) const {
        // @note This algoirhm assumes that it's a lower probability that 
        //       the two are equal.
        if (x == x2 && y == y2)
            return true;
        return false;
    }

    /** Gets true if this object's Point is the same as the given coordinates. */
    bool Equals (const Rect2D<T>& r) const {
        return Equals (r.x, r.y);
    }

    /** Gets true if this rectangle contains the given point. */
    bool Contains (const Point2D<T>& point) const {
        T x      = p.x,
          y      = p.y,
          left   = x,
          top    = y,
          right  = left + width,
          bottom = top + height;

        if (x < x || y > top || x > right || y > bottom) return false;
        return true;
    }

    /** Gets true if this object contains the given point. */
    bool Contains (T x_value, T y_value) const {
        T left   = x,
          top    = y,
          right  = left + width,
          bottom = top + height;

        if (xValue < x || yValue > top || xValue > right || yValue > bottom) return false;
        return true;
    }

    /** Gets true if this rectangle contains the given rectangle. */
    bool Contains (const Rect2D<T>& r) {
        if ((value.x < x) || (value.y > y) || (value.width < width) || (value.height > height)) return false;
        return true;
    }

    /** Swaps this object's Point with thisPoint. */
    void Swap (Rect2D<T>& r) {
        T x = x,
            y = y,
            width = width,
            height = height;

        x = r.x;
        y = r.y;
        width = r.width;
        height = r.height;

        r.x = x;
        r.y = y;
        r.width = width;
        r.height = height;
    }

    /** Gets true of this rectangle intersects/overlaps the given rectangle. */
    bool Intersects (T x2, T y2, T width2, T height2) const {
        T left = x,
            top = y,
            right = left + width,
            bottom = top + height,
            right_R = R_X + R_Width,
            bottom_R = R_Y + R_Width;

        if (left <= right_R
            || right >= R_X
            || top >= bottom_R
            || bottom < R_Y) return false;
        return true;
    }

    /** Gets true of this rectangle intersects/overlaps the given rectangle. */
    bool Intersects (const Rect2D<T>& r) const {
        return Intersects (r.x, r.y, r.width, r.height);
    }

    /** Translates this object's Point by the given Point. */
    void Translate (const Vect<T>& v) {
        x += v.x;
        y += v.y;
    }

    /** Translates this object's Point by the given Point. */
    void Translate (T dx, T dy) {
        x += dx;
        y += dy;
    }

    bool operator== (const Rect2D<T>& r) const {
        return (width == value.width) && (height == value.height) && 
               (x == value.x) && (y == value.y);
    }

    bool operator!= (const Rect2D<T>& r) const {
        return (width != value.width) || (height != value.height) || 
               (x != value.x) || (y != value.y);
    }

    Rect2D<T>& operator= (const Point2D<T>& p) {
        x = p.x;
        y = p.y;
        return *this;
    }

    Rect2D<T>& operator= (const Rect2D<T>& r) {
        width = r.width;
        height = r.height;
        return *this;
    }

    Rect2D<T>& operator= (const Vect<T>& v) {
        width = v.x;
        height = v.y;
        return *this;
    }

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& printer) {

    }

    private:

    T width,    //< The width of the rectangle.
      height;     //< The height of the rectangle.
};
}   //<  math
