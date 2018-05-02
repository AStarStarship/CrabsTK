/** Kabuki Toolkit
    @file    ~/kt/math/poly2d.h
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

#include "rect2d.h"
#include "affine_transform.h"

namespace math {
    
template<typename T>
class KABUKI_TOOLKIT Poly2D
/*< A 2D polygon that uses 32-bit floating point values. 
    This class is a C++ implementation of Java's polygon interface that does not 
    use polymorphism.

    @todo Write/find OpenGLES2 affine transform classes.
    @todo Find things that can be hardware accellorated.
*/
{
    public:

    enum {
        MinNumPoints = 3,           //< The minimum number of points in a Poly2D; which is a triangle.
        DefaultBufferSize = 16      //< The default pre-allocated buffer size.
    };
    
    Poly2D ();
    /*< Constructs a unit square.
        Unit square is similiar to a unit circle, and has coordinates 
        {0.0, 0.0}, { 1.0. 0.0}, { 1.0, 1.0}, { 0.0, 1.0}; */

    Poly2D (T* xPoints, T* yPoints, int aNumPoints, int aBufferSize);
    /*< Constructs a polygon from the given x and y points. */

    ~Poly2D ();
    /*< Deconstructs the dynamic memory. */

    void reset();
    /*< Resets the polygon to a triangle. */

    T* getXs ();
    /*< Gets a poniter to the x points array. */

    T* getYs ();
    /*< Gets a poniter to the y points array. */

    int getNumPoints ();
    /*< Gets the number of points in the polygon. */

    int getBufferSize ();
    /*< Gets the number of points the buffer can store.. */

    void translate (T dx, T dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void translate (int dx, int dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void updateBounds (T x, T y);
    /*< Gets the upper bounds of the given x and y coordinates. */

    void updateBounds (int x, int y);
    /*< Gets the upper bounds of the given x and y coordinates. */

    void translate (int dx, int dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void addPoint (T x, T y);
    /*< Adds the given x and y points to the polygon. */

    void addPoint (int x, int y);
    /*< Adds the given x and y points to the polygon. */

    const Rect2D& getBounds();
    /*< Gets a const reference to the bounding box. */

    bool contains (T x, T y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (T x, T y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (int x, int y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (Point<T> p);
    /*< Returns true if this polygon contains the given point. */

    bool intersects (T leftX, T topY, T width, T height);
    /*< Returns true if this polygon intersects the given rectangle. */

    bool intersects (int leftX, int topY, int width, int height);
    /*< Returns true if this polygon intersects the given rectangle. */

    bool intersects (Rect2D r);
    /*< Returns true if this polygon intersects the given rectangle. */

    Poly2DPathIterator<T> getPathIterator (AffineTransform<T> a);

    Poly2DPathIterator<T> getPathIterator (AffineTransform<T> a, T flatness);

    protected:

    Rect2D bounds;          //< The stored outter rectangular bounds.
    
    private:
	
	int numPoints,          //< The number of points in the polygon.
        bufferSize;         //< The size of the buffer.

    T* x,               //< Pointer to the x points array.
        * y;                //< Pointer to the y points array.

    //Crossings_f getCrossings (Type xlo, Type ylo, Type xhi, Type yhi);

    void UpdateBounds ();
};

class Poly2DPathIterator<T>
{
    public:

    Poly2DPathIterator<T> (Poly2D p, AffineTransform<T> a);

    bool isDone();

    void getNext();

    int calcSegment (T* coords);

    int calcSegment (T* coords);

    private:

    Poly2D polygon;

    AffineTransform<T> transform;

    int index;
};

}   //< math
