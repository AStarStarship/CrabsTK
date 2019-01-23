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
#include <pch.h>

#include "rect2d.h"
#include "affine_transform.h"

namespace _ {
    
template<typename T>
class API Poly2D
/* A 2D polygon that uses 32-bit floating point values. 
This class is a C++ implementation of Java's polygon interface that does not 
use polymorphism.

@todo Write/find OpenGLES2 affine transform classes.
@todo Find things that can be hardware accelerated.
*/
{
    public:

    enum {
        MinNumPoints = 3,           //< The minimum number of points in a Poly2D; which is a triangle.
        DefaultBufferSize = 16      //< The default pre-allocated buffer size.
    };
    
    /* Constructs a unit square.
        Unit square is similar to a unit circle, and has coordinates 
        {0.0, 0.0}, { 1.0. 0.0}, { 1.0, 1.0}, { 0.0, 1.0}; */
    Poly2D ()
	:   numPoints (MinBufferSize),
		bufferSize (MinBufferSize),
		x (new float[MinNumPoints]),
		y (new float[MinNumPoints])
	{
		/// Nothing to do here! :-)
	}

    /* Constructs a polygon from the given x and y points. */
    Poly2D (T* xPoints, T* yPoints, int aNumPoints, int aBufferSize);

    /* Destroys the dynamic memory. */
    ~Poly2D ();

    /* Resets the polygon to a triangle. */
    void Reset();

    /* Gets a pointer to the x points array. */
    T* GetXs ();

    /* Gets a pointer to the y points array. */
    T* GetYs ();

    /* Gets the number of points in the polygon. */
    int PointCount ();

    /* Gets the number of points the buffer can store. */
    int BufferCount ();

    /* Translates the polygon by the given delta x and delta y. */
    void Translate (T dx, T dy);

    /* Translates the polygon by the given delta x and delta y. */
    void Translate (int dx, int dy);

    /* Gets the upper bounds of the given x and y coordinates. */
    void UpdateBounds (T x, T y);

    /* Gets the upper bounds of the given x and y coordinates. */
    void UpdateBounds (int x, int y);

    /* Translates the polygon by the given delta x and delta y. */
    void Translate (int dx, int dy);

    /* Adds the given x and y points to the polygon. */
    void AddPoint (T x, T y);

    /* Adds the given x and y points to the polygon. */
    void AddPoint (int x, int y);

    /* Gets a const reference to the bounding box. */
    const Rect2D& getBounds();

    /* Returns true if this polygon contains the given point. */
    bool Contains (T x, T y);

    /* Returns true if this polygon contains the given point. */
    bool Contains (T x, T y);

    /* Returns true if this polygon contains the given point. */
    bool Contains (int x, int y);

    /* Returns true if this polygon contains the given point. */
    bool Contains (Point<T> p);

    /* Returns true if this polygon intersects the given rectangle. */
    bool Intersects (T leftX, T topY, T width, T height);

    /* Returns true if this polygon intersects the given rectangle. */
    bool Intersects (int leftX, int topY, int width, int height);

    /* Returns true if this polygon intersects the given rectangle. */
    bool Intersects (Rect2D r);

    Poly2DPathIterator<T> GetPathIterator (Transform<T> a);

    Poly2DPathIterator<T> GetPathIterator (AffineTransform<T> a, T flatness);

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

template<typename T>
class Poly2DPathIterator {
    public:

    Poly2DPathIterator<T> (Poly2D p, AffineTransform<T> a);

    bool IsDone();

    void GetNext();

    int CalcSegment (T* coords);

    int CalcSegment (T* coords);

    private:

    Poly2D polygon;

    AffineTransform<T> transform;

    int index;
};

}       //< namespace _
