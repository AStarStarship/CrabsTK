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

    int GetWidth () const;
    /*< Gets the width.
    @pre    Input must be positive.
    */

    int SetWidth  (int value);
    /*< Sets the width.
    @pre    Input must be positive. */

    int GetHeight () const;
    /*< Gets and sets the height.
    @pre    Input must be positive.
    */

    int SetHeight  (int value);
    /*< Sets the height.
    @return Gets 0 upon success, -1 if input is negative, and +1 if input greater than max. */

    int GetCenterX () const;
    /*< Gets and sets the center x value. */

    int GetCenterY () const;
    /*< Gets and sets the center y value. */

    void SetCenterX  (int Value);
    /*< Sets the center y value. */

    void SetCenterY  (int Value);
    /*< Sets the center y value. */

    void Set  (const Rect_i& R);
    /*< Sets this object's Point to the thisPositition. */

    bool Equals  (const Rect_i& R) const;
    /*< Gets true if this object's Point is the same as the given coordinates. */

    bool Contains  (const Point_i& P) const;
    /*< Gets true if this rectangle contains the given point. */

    bool Contains  (int XValue, int YValue) const;
    /*< Gets true if this object contains the given point. */

    bool Contains  (const Rect_i& R);
    /*< Gets true if this rectangle contains the given rectangle. */

    void Swap  (Rect_i& R);
    /*< Swaps this object's Point with thisPoint. */

    bool Intersects  (int R_X, int R_Y, int R_Width, int R_Height) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    bool Intersects  (const Rect_i& R) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    void Translate  (const Vect_i& V);
    /*< Translates this object's Point by the given Point. */

    void Translate  (int DeltaX, int DeltaY);
    /*< Translates this object's Point by the given Point. */

    void SetPosition  (const Point_i& P);

    void SetDimensions  (const Vect_i& S);

    Rect_i& PareOff  (int pixels); 
    /*< Pares the specified pixels off the Rect_i. 
    It's equal to x += pixels; y + pixels; width -=  (pixels << 1); height -=  (pixels << 1); */

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

    float GetWidth () const;
    /*< Gets the width.
    @pre    Input must be positive.
    */

    float SetWidth  (float value);
    /*< Sets the width.
    @pre    Input must be positive. */

    float GetHeight () const;
    /*< Gets and sets the height.
    @pre    Input must be positive.
    */

    float SetHeight  (float value);
    /*< Sets the height.
    @return Gets 0 upon success, -1 if input is negative, and +1 if input greater than max. */

    float GetCenterX () const;
    /*< Gets and sets the center x value. */

    float GetCenterY () const;
    /*< Gets and sets the center y value. */

    void SetCenterX  (float Value);
    /*< Sets the center y value. */

    void SetCenterY  (float Value);
    /*< Sets the center y value. */

    void Set  (const Rect_f& R);
    /*< Sets this object's Point to the thisPositition. */

    bool Equals  (const Rect_f& R) const;
    /*< Gets true if this object's Point is the same as the given coordinates. */

    bool Contains  (const Point_f& P) const;
    /*< Gets true if this rectangle contains the given point. */

    bool Contains  (float XValue, float YValue) const;
    /*< Gets true if this object contains the given point. */

    bool Contains  (const Rect_f& R);
    /*< Gets true if this rectangle contains the given rectangle. */

    void Swap  (Rect_f& R);
    /*< Swaps this object's Point with thisPoint. */

    bool Intersects  (float R_X, float R_Y, float R_Width, float R_Height) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    bool Intersects  (const Rect_f& R) const;
    /*< Gets true of this Rectangle intersects/overlaps the given rectangle. */

    void Translate  (const Vect_f& V);
    /*< Translates this object's Point by the given Point. */

    void Translate  (float DeltaX, float DeltaY);
    /*< Translates this object's Point by the given Point. */

    void SetPosition  (const Point_f& P);

    void SetDimensions  (const Vect_f& S);

    bool operator==  (const Rect_f& R) const;

    bool operator!=  (const Rect_f& R) const;

    Rect_f& operator=  (const Point_f& R);

    Rect_f& operator=  (const Rect_f& R);

    Rect_f& operator=  (const Vect_f&);

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //< namespace  _2D
