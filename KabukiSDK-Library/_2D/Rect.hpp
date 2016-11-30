/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/Rect.hpp
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
#include "Point.hpp"
#include "Vect.hpp"

namespace _2D {

struct _KabukiSDK Rect_i
{
    int X,          //< The left edge.
        Y,          //< The top edge.
        Width,      //< The width of the rectangle.
        Height;     //< The height of the rectangle.

    Rect_i ();
    /*< Constructs an uninitialized rectangle. */

    Rect_i  (int InitX = 0, int InitY = 0, int InitWidth = 0, int InitHeight = 0);
    /*< Constructs a rectangle from the given data. */

    Rect_i  (const Point_i& P, const Vect_i& V);
    /*< Constructs . */

    explicit Rect_i  (const Rect_i& R); 
    /*< Copy constructor. */

    int getWidth () const;
    /*< gets the width.
        @pre    Input must be positive.
    */

    int setWidth  (int value);
    /*< sets the width.
    @pre    Input must be positive. */

    int getHeight () const;
    /*< gets and sets the height.
        @pre    Input must be positive.
    */

    int setHeight  (int value);
    /*< sets the height.
        @return gets 0 upon success, -1 if input is negative, and +1 if input greater than max.
    */

    int getCenterX () const;
    /*< gets and sets the center x value. */

    int getCenterY () const;
    /*< gets and sets the center y value. */

    void setCenterX  (int Value);
    /*< sets the center y value. */

    void setCenterY  (int Value);
    /*< sets the center y value. */

    void set  (const Rect_i& R);
    /*< sets this object's Point to the thisPositition. */

    bool equals  (const Rect_i& R) const;
    /*< gets true if this object's Point is the same as the given coordinates. */

    bool contains  (const Point_i& P) const;
    /*< gets true if this rectangle contains the given point. */

    bool contains  (int XValue, int YValue) const;
    /*< gets true if this object contains the given point. */

    bool contains  (const Rect_i& R);
    /*< gets true if this rectangle contains the given rectangle. */

    void swap  (Rect_i& R);
    /*< Swaps this object's Point with thisPoint. */

    bool intersects  (int R_X, int R_Y, int R_Width, int R_Height) const;
    /*< gets true of this Rectangle intersects/overlaps the given rectangle. */

    bool intersects  (const Rect_i& R) const;
    /*< gets true of this Rectangle intersects/overlaps the given rectangle. */

    void translate  (const Vect_i& V);
    /*< Translates this object's Point by the given Point. */

    void translate  (int dx, int dy);
    /*< Translates this object's Point by the given Point. */

    void setPosition  (const Point_i& P);
    /*< Sets the x, y coordinates to the given point. */

    void setDimensions  (const Vect_i& S);
    /*< Sets the dimensions to the given vector. */

    Rect_i& PareOff  (int pixels); 
    /*< Pares the specified pixels off the Rect_i. 
        It's equal to x += pixels; y + pixels; width -=  (pixels << 1); height -=  (pixels << 1); 
    */

    bool operator==  (const Rect_i& R) const;

    bool operator!=  (const Rect_i& R) const;

    Rect_i& operator=  (const Point_i& R);

    Rect_i& operator=  (const Rect_i& R);

    Rect_i& operator=  (const Vect_i&);

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

struct _KabukiSDK Rect_f
{
    float X,          //< The left edge.
        Y,          //< The top edge.
        Width,      //< The width of the rectangle.
        Height;     //< The height of the rectangle.

    Rect_f ();
    /*< Constructs an uninitialized rectangle. */

    Rect_f  (float InitX = 0, float InitY = 0, float InitWidth = 0, float InitHeight = 0);
    /*< Constructs a rectangle from the given data. */

    Rect_f  (const Point_f& P, const Vect_f& V);
    /*< Constructs . */

    explicit Rect_f  (const Rect_f& R); 
    /*< Copy constructor. */

    float getWidth () const;
    /*< gets the width.
    @pre    Input must be positive.
    */

    float setWidth  (float value);
    /*< sets the width.
    @pre    Input must be positive. */

    float getHeight () const;
    /*< gets and sets the height.
    @pre    Input must be positive.
    */

    float setHeight  (float value);
    /*< sets the height.
    @return gets 0 upon success, -1 if input is negative, and +1 if input greater than max. */

    float getCenterX () const;
    /*< gets and sets the center x value. */

    float getCenterY () const;
    /*< gets and sets the center y value. */

    void setCenterX  (float Value);
    /*< sets the center y value. */

    void setCenterY  (float Value);
    /*< sets the center y value. */

    void set  (const Rect_f& R);
    /*< sets this object's Point to the thisPositition. */

    bool equals  (const Rect_f& R) const;
    /*< gets true if this object's Point is the same as the given coordinates. */

    bool contains  (const Point_f& P) const;
    /*< gets true if this rectangle contains the given point. */

    bool contains  (float XValue, float YValue) const;
    /*< gets true if this object contains the given point. */

    bool contains  (const Rect_f& R);
    /*< gets true if this rectangle contains the given rectangle. */

    void swap  (Rect_f& R);
    /*< Swaps this object's Point with thisPoint. */

    bool intersects  (float R_X, float R_Y, float R_Width, float R_Height) const;
    /*< gets true of this Rectangle intersects/overlaps the given rectangle. */

    bool intersects  (const Rect_f& R) const;
    /*< gets true of this Rectangle intersects/overlaps the given rectangle. */

    void translate  (const Vect_f& V);
    /*< Translates this object's Point by the given Point. */

    void translate  (float dx, float dy);
    /*< Translates this object's Point by the given Point. */

    void setPosition  (const Point_f& P);

    void setDimensions  (const Vect_f& S);

    bool operator==  (const Rect_f& R) const;

    bool operator!=  (const Rect_f& R) const;

    Rect_f& operator=  (const Point_f& R);

    Rect_f& operator=  (const Rect_f& R);

    Rect_f& operator=  (const Vect_f&);

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace  _2D
