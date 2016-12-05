/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/Rect.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>
#include "Point.hpp"
#include "Vect.hpp"

namespace _2D {

class _KabukiSDK Rect_f
{
    public:

    float x,        //< The left edge.
        y,          //< The top edge.

    Rect_f ();
    /*< Constructs an uninitialized rectangle. */

    Rect_f (float initX = 0, float initY = 0, float initWidth = 0, float initHeight = 0);
    /*< Constructs a rectangle from the given data. */

    Rect_f (const Point_f& p, const Vect_f& v);
    /*< Constructs a rectangle from the given point and vector. */

    explicit Rect_f (const Rect_f& r); 
    /*< Copy constructor. */

    float getWidth () const;
    /*< Gets the width.
        @pre    Input must be positive. */

    void setWidth (float value);
    /*< Sets the width.
    @pre    Input must be positive. */

    float getHeight () const;
    /*< Gets and sets the height.
        @pre    Input must be positive. */

    void setHeight (float value);
    /*< Sets the height.
        @return gets 0 upon success, -1 if input is negative, and +1 if input greater than max. */

    float getCenterX () const;
    /*< Gets and sets the center x value. */

    float getCenterY () const;
    /*< Gets and sets the center y value. */

    void setCenterX (float value);
    /*< Sets the center y value. */

    void setCenterY (float value);
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

    bool contains (float xValue, float yValue) const;
    /*< Gets true if this object contains the given point. */

    bool contains (const Rect_f& r);
    /*< Gets true if this rectangle contains the given rectangle. */

    void swap (Rect_f& r);
    /*< Swaps this object's Point with thisPoint. */

    bool intersects (float thisX, float thisY, float thisWdith, float thisHeight) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    bool intersects (const Rect_f& r) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    void translate (const Vect_f& v);
    /*< Translates this object's Point by the given Point. */

    void translate (float dx, float dy);
    /*< Translates this object's Point by the given Point. */

    bool operator== (const Rect_f& r) const;

    bool operator!= (const Rect_f& r) const;

    Rect_f& operator= (const Point_f& r);

    Rect_f& operator= (const Rect_f& r);

    Rect_f& operator= (const Vect_f& v);

    inline void print (Terminal& slot);
    /*< Prints this object to the terminal. */

    private:

    float width,    //< The width of the rectangle.
        height;     //< The height of the rectangle.
};

class _KabukiSDK Rect_d
{
    private:

    Rect_d ();
    /*< Constructs an uninitialized rectangle. */

    Rect_d (double initX = 0, double initY = 0, double initWidth = 0, double initHeight = 0);
    /*< Constructs a rectangle from the given data. */

    Rect_d (const Point_d& p, const Vect_d& v);
    /*< Constructs . */

    explicit Rect_d (const Rect_d& r); 
    /*< Copy constructor. */

    double getWidth () const;
    /*< Gets the width.
        @pre    Input must be positive.
    */

    double setWidth (double value);
    /*< Sets the width.
        @pre    Input must be positive. */

    double getHeight () const;
    /*< Gets and sets the height.
        @pre    Input must be positive.
    */

    double setHeight (double value);
    /*< Sets the height.
        @return gets 0 upon success, -1 if input is negative, and +1 if input greater than max.
    */

    double getCenterX () const;
    /*< Gets and sets the center x value. */

    double getCenterY () const;
    /*< Gets and sets the center y value. */

    void setCenterX (double value);
    /*< Sets the center y value. */

    void setCenterY (double value);
    /*< Sets the center y value. */

    void set (const Rect_d& r);
    /*< Sets this object's Point to the thisPositition. */

    void setPosition (const Point_d& p);
    /*< sets the rectangle's (x,y) position to the given point. */

    void setDimensions (const Vect_d& v);
    /*< sets the rectangle's width and height to the given vector. */

    bool equals (const Rect_d& r) const;
    /*< Gets true if this object's Point is the same as the given coordinates. */

    bool contains (const Point_d& p) const;
    /*< Gets true if this rectangle contains the given point. */

    bool contains (double xValue, double yValue) const;
    /*< Gets true if this object contains the given point. */

    bool contains (const Rect_d& r);
    /*< Gets true if this rectangle contains the given rectangle. */

    void swap (Rect_d& r);
    /*< Swaps this object's Point with thisPoint. */

    bool intersects (double thisX, double thisY, double thisWidth, double thisHeight) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    bool intersects (const Rect_d& r) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    void translate (const Vect_d& v);
    /*< Translates this object's Point by the given Point. */

    void translate (double dx, double dy);
    /*< Translates this object's Point by the given Point. */

    Rect_d& PareOff (int pixels); 
    /*< Pares the specified pixels off the Rect_d. 
        It's equal to x += pixels; y + pixels; width -= (pixels << 1); height -= (pixels << 1); 
    */

    inline void print (Terminal& slot);
    /*< Prints this object to the terminal. */

    bool operator== (const Rect_d& r) const;

    bool operator!= (const Rect_d& r) const;

    Rect_d& operator= (const Point_d& r);

    Rect_d& operator= (const Rect_d& r);

    Rect_d& operator= (const Vect_d&);

    inline void print (Terminal& slot);
    /*< Prints this object to the terminal. */

    private:

    double x,       //< The left edge.
        y,          //< The top edge.
        rx,         //< The radius of the rectangle in the x direction.
        ry;         //< The radius of the rectangle in the x direction.
};

}   //<  _2D
