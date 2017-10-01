/** Kabuki Theater
    @file    /.../Source/_Math/Rect.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (r).

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


#include <KabukiTheater-Config.h>
#include "Point.h"
#include "Vect.h"

namespace _Math {

template<typename Type>
class _KabukiTheater_ Rect
{
    public:

    Type x,        //< The left edge.
        y,          //< The top edge.

    Rect_f ();
    /*< Constructs an uninitialized rectangle. */

    Rect_f (Type initX = 0, Type initY = 0, Type initWidth = 0, Type initHeight = 0);
    /*< Constructs a rectangle from the given data. */

    Rect_f (const Point_f& p, const Vect_f& v);
    /*< Constructs a rectangle from the given point and vector. */

    explicit Rect_f (const Rect_f& r); 
    /*< Copy constructor. */

    Type getWidth () const;
    /*< Gets the width.
        @pre    Input must be positive. */

    void setWidth (Type value);
    /*< Sets the width.
    @pre    Input must be positive. */

    Type getHeight () const;
    /*< Gets and sets the height.
        @pre    Input must be positive. */

    void setHeight (Type value);
    /*< Sets the height.
        @return gets 0 upon success, -1 if input is negative, and +1 if input greater than max. */

    Type getCenterX () const;
    /*< Gets and sets the center x value. */

    Type getCenterY () const;
    /*< Gets and sets the center y value. */

    void setCenterX (Type value);
    /*< Sets the center y value. */

    void setCenterY (Type value);
    /*< Sets the center y value. */

    void set (const Rect_f& r);
    /*< Sets this object's Point to the thisPositition. */

    void setPosition (const Point_f& p);
    /*< sets the rectangle's (x,y) position to the given point. */

    void setDimensions (const Vect_f& S);
    /*< sets the rectangle's width and height to the given vector. */

    bool equals (const Rect_f& r) const;
    /*< Gets true if this object's Point is the same as the given coordinates. */

    bool contains (const Point_f& p) const;
    /*< Gets true if this rectangle contains the given point. */

    bool contains (Type xValue, Type yValue) const;
    /*< Gets true if this object contains the given point. */

    bool contains (const Rect_f& r);
    /*< Gets true if this rectangle contains the given rectangle. */

    void swap (Rect_f& r);
    /*< Swaps this object's Point with thisPoint. */

    bool intersects (Type thisX, Type thisY, Type thisWdith, Type thisHeight) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    bool intersects (const Rect_f& r) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    void translate (const Vect_f& v);
    /*< Translates this object's Point by the given Point. */

    void translate (Type dx, Type dy);
    /*< Translates this object's Point by the given Point. */

    bool operator== (const Rect_f& r) const;

    bool operator!= (const Rect_f& r) const;

    Rect_f& operator= (const Point_f& r);

    Rect_f& operator= (const Rect_f& r);

    Rect_f& operator= (const Vect_f& v);

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */

    private:

    Type width,    //< The width of the rectangle.
        height;     //< The height of the rectangle.
};
}   //<  _Math
