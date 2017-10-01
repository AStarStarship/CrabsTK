/** Kabuki Theater
    @file    /.../Source/_G/Poly2D.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

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

#include "Rect.h"
#include "AffineTransform.h"

namespace _Math {
    
template<typename Type>
class _KabukiTheater_ Poly2D
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

    Poly2D (Type* xPoints, Type* yPoints, int aNumPoints, int aBufferSize);
    /*< Constructs a polygon from the given x and y points. */

    ~Poly2D ();
    /*< Deconstructs the dynamic memory. */

    void reset();
    /*< Resets the polygon to a triangle. */

    Type* getXs ();
    /*< Gets a poniter to the x points array. */

    Type* getYs ();
    /*< Gets a poniter to the y points array. */

    int getNumPoints ();
    /*< Gets the number of points in the polygon. */

    int getBufferSize ();
    /*< Gets the number of points the buffer can store.. */

    void translate (Type dx, Type dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void translate (int dx, int dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void updateBounds (Type x, Type y);
    /*< Gets the upper bounds of the given x and y coordinates. */

    void updateBounds (int x, int y);
    /*< Gets the upper bounds of the given x and y coordinates. */

    void translate (int dx, int dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void addPoint (Type x, Type y);
    /*< Adds the given x and y points to the polygon. */

    void addPoint (int x, int y);
    /*< Adds the given x and y points to the polygon. */

    const Rect_f& getBounds();
    /*< Gets a const reference to the bounding box. */

    bool contains (Type x, Type y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (Type x, Type y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (int x, int y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (Point<Type> p);
    /*< Returns true if this polygon contains the given point. */

    bool intersects (Type leftX, Type topY, Type width, Type height);
    /*< Returns true if this polygon intersects the given rectangle. */

    bool intersects (int leftX, int topY, int width, int height);
    /*< Returns true if this polygon intersects the given rectangle. */

    bool intersects (Rect_f r);
    /*< Returns true if this polygon intersects the given rectangle. */

    Poly2DPathIterator_f getPathIterator (AffineTransform_f a);

    Poly2DPathIterator_f getPathIterator (AffineTransform_f a, Type flatness);

    protected:

    Rect_f bounds;          //< The stored outter rectangular bounds.
    
    private:
	
	int numPoints,          //< The number of points in the polygon.
        bufferSize;         //< The size of the buffer.

    Type* x,               //< Pointer to the x points array.
        * y;                //< Pointer to the y points array.

    //Crossings_f getCrossings (Type xlo, Type ylo, Type xhi, Type yhi);

    void updateBounds ();
};

class Poly2DPathIterator_f
{
    public:

    Poly2DPathIterator_f (Poly2D p, AffineTransform_f a);

    bool isDone();

    void getNext();

    int calcSegment (Type* coords);

    int calcSegment (Type* coords);

    private:

    Poly2D polygon;

    AffineTransform_f transform;

    int index;
};

}   //< _Math
