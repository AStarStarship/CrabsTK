/** Kabuki Theater
    @file    /.../KabukiTheater-Library/_G/Poly2D.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include "Rect.hpp"
#include "AffineTransform.hpp"

namespace _Math {
    
class _KabukiTheater Poly2D_f
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
    
    Poly2D_f ();
    /*< Constructs a unit square.
        Unit square is similiar to a unit circle, and has coordinates 
        {0.0, 0.0}, { 1.0. 0.0}, { 1.0, 1.0}, { 0.0, 1.0}; */

    Poly2D_f (float* xPoints, float* yPoints, int aNumPoints, int aBufferSize);
    /*< Constructs a polygon from the given x and y points. */

    ~Poly2D_f ();
    /*< Deconstructs the dynamic memory. */

    void reset();
    /*< Resets the polygon to a triangle. */

    float* getXs ();
    /*< Gets a poniter to the x points array. */

    float* getYs ();
    /*< Gets a poniter to the y points array. */

    int getNumPoints ();
    /*< Gets the number of points in the polygon. */

    int getBufferSize ();
    /*< Gets the number of points the buffer can store.. */

    void translate (float dx, float dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void translate (int dx, int dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void updateBounds (float x, float y);
    /*< Gets the upper bounds of the given x and y coordinates. */

    void updateBounds (int x, int y);
    /*< Gets the upper bounds of the given x and y coordinates. */

    void translate (int dx, int dy);
    /*< Translates the polygon by the given delta x and delta y. */

    void addPoint (float x, float y);
    /*< Adds the given x and y points to the polygon. */

    void addPoint (int x, int y);
    /*< Adds the given x and y points to the polygon. */

    const Rect_f& getBounds();
    /*< Gets a const reference to the bounding box. */

    bool contains (float x, float y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (float x, float y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (int x, int y);
    /*< Returns true if this polygon contains the given point. */

    bool contains (Point_f p);
    /*< Returns true if this polygon contains the given point. */

    bool intersects (float leftX, float topY, float width, float height);
    /*< Returns true if this polygon intersects the given rectangle. */

    bool intersects (int leftX, int topY, int width, int height);
    /*< Returns true if this polygon intersects the given rectangle. */

    bool intersects (Rect_f r);
    /*< Returns true if this polygon intersects the given rectangle. */

    Poly2DPathIterator_f getPathIterator (AffineTransform_f a);

    Poly2DPathIterator_f getPathIterator (AffineTransform_f a, float flatness);

    protected:

    Rect_f bounds;          //< The stored outter rectangular bounds.
    
    private:
	
	int numPoints,          //< The number of points in the polygon.
        bufferSize;         //< The size of the buffer.

    float* x,               //< Pointer to the x points array.
        * y;                //< Pointer to the y points array.

    //Crossings_f getCrossings (float xlo, float ylo, float xhi, float yhi);

    void updateBounds ();
};

class Poly2DPathIterator_f
{
    public:

    Poly2DPathIterator_f (Poly2D_f p, AffineTransform_f a);

    bool isDone();

    void getNext();

    int calcSegment (float* coords);

    int calcSegment (float* coords);

    private:

    Poly2D_f polygon;

    AffineTransform_f transform;

    int index;
};

}   //< _Math
