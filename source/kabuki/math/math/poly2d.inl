/** Kabuki Starship
    @file    /.../Kabuki-Library/_Math/Poly2D.cpp
    @author  Edited by Cale McCollough <cale.mccollough@gmail.com>
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
/*
* Copyright 1995-2006 Sun Microsystems, Inc.  All Rights Reserved.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* This code is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 2 only, as
* published by the Free Software Foundation.  Sun designates this
* particular file as subject to the "Classpath" exception as provided
* by Sun in the LICENSE file that accompanied this code.
*
* This code is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
* version 2 for more details (a copy is included in the LICENSE file that
* accompanied this code).
*
* You should have received a copy of the GNU General Public License version
* 2 along with this work; if not, write to the Free Software Foundation,
* Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
*
* Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
* CA 95054 USA or visit www.sun.com if you need additional information or
* have any questions.
*/

#include <_Math/Calc.hpp>
using namespace _Math;

namespace _Math {

template<typename T>
Poly2D::Poly2D()
:   numPoints (MinBufferSize),
    bufferSize (MinBufferSize),
    x (new float[MinNumPoints]),
    y (new float[MinNumPoints])
{
    /// Nothing to do here! :-)
}

template<typename T>
Poly2D::Poly2D (float* initXPoints, float* initYPoints, int initNumPoints, int initBufferSize)
:   numPoints (initNumPoints),
    bufferSize (initBufferSize)
{
    if (initBufferSize < initNumPoints)
    {
        bufferSize = initBufferSize = initNumPoints;
    }


    if (initNumPoints < MinNumPoints)
    {
        x = new float[MinNumPoints];
        y = new float[MinNumPoints];

        x[0] = 0;
        x[1] = 1;
        x[2] = 0.5;
        y[0] = 0;
        y[1] = 1.0;
        return;
    }

    float* newXPoints = new float[MinNumPoints],
        * newYPoints = new float[MinNumPoints];

    numPoints = numPoints;
    memcpy (newXPoints, x, numPoints);
    memcpy (newYPoints, y, numPoints);

    x = newXPoints;
    y = newYPoints;

}

template<typename T>
Poly2D::~Poly2D ()
{
    delete x;
    delete y;
}

template<typename T>
void Poly2D::reset()
{
    // Write me!
}

template<typename T>
float* Poly2D::getXPoints ()
{
    return x;
}

template<typename T>
float* Poly2D::getYPoints ()
{
    return y;
}

template<typename T>
int Poly2D::getNumPoints ()
{
    return numPoints;
}

template<typename T>
int Poly2D::getBufferSize ()
{
    return bufferSize;
}

template<typename T>
void Poly2D::translate (float dx, float dy)
{
    for (int i = 0; i < numPoints; i++)
    {
        x[i] += dx;
        y[i] += dy;
    }

    bounds.translate (dx, dy);
}

template<typename T>
void Poly2D::translate (int dx, int dy)
{
    translate ((float)dx, (float)dy);
}

template<typename T>
void Poly2D::translate (int dx, int dy)
{
    translate ((float)dx, (float)dy);
}

template<typename T>
void Poly2D::calculateBounds (float* x, float* y, int numPoints)
{
    int boundsMinX = getMinFloat (),
        boundsMinY = getMinFloat (),
        boundsMaxX = getMaxFloat (),
        boundsMaxY = getMaxFloat ();

    for (int i = 0; i < numPoints; ++i)
    {
        float xi = x[i],
            yi = y[i];

        boundsMinX = min (boundsMinX, xi);
        boundsMaxX = max (boundsMaxX, xi);

        boundsMinY = min (boundsMinY, yi);
        boundsMaxY = max (boundsMaxY, yi);
    }
    return Rect_f (boundsMinX, boundsMinY, boundsMaxX - boundsMinX, boundsMaxY - boundsMinY);
}

template<typename T>
void Poly2D::updateBounds (int x, int y)
{
    if (x < bounds.x)
    {
        bounds.setWidth (bounds.getWidth () + (bounds.x - x));
        bounds.x = x;
    }
    else
    {
        bounds.setWidth (max (bounds.getWidth (), x - bounds.x));
        // bounds.x = bounds.x;
    }

    if (y < bounds.y)
    {
        bounds.setHeight (bounds.getHeight () + (bounds.y - y));
        bounds.y = y;
    }
    else
    {
        bounds.setHeight (max (bounds.getHeight (), y - bounds.y));
        // bounds.y = bounds.y;
    }
}

template<typename T>
void Poly2D::addPoint (int newX, int newY)
{
    if (numPoints >= bufferSize)    //< Then double the buffer size.
    {
        int newBufferSize = bufferSize = 2;
        bufferSize = newBufferSize;

        float* tempX = x,
            * tempY = y,
            * newX = new float[newBufferSize],
            * newY = new float[newBufferSize];
        
         memcpy (newX, x, numPoints);
         memcpy (newY, y, numPoints);

         delete x;
         delete y;
         x = newX;
         y = newY;
    }

    x[numPoints] = newX;
    y[numPoints] = newY;
    ++numPoints;

    updateBounds (newX, newY);
}

template<typename T>
void Poly2D::addPoint (int newX, int newY)
{
    addPoint ((float)newX, (float)newY);
}

template<typename T>
const Rect_f& Poly2D::getBounds ()
{
    return bounds;
}

template<typename T>
bool contains (int x, int y)
{
    return contains((float) x, (float) y);
}

template<typename T>
bool Poly2D::contains (Point<float> p)
{
    return contains (p.x, p.y);
}

template<typename T>
const Rect_f& Poly2D::getBounds ()
{
    return getBounds ();
}

template<typename T>
bool Poly2D::contains (float x, float y)
{
    if (numPoints < 3)                  return false;
    if (!getBounds ().contains (x, y))  return false;

    int hits = 0;

    float prevX = x[numPoints - 1],
        prevY = y[numPoints - 1],
        currentX,
        currentY;

    // Walk the edges of the polygon
    for (int i = 0; i < numPoints; prevX = currentX, prevY = currentY, i++)
    {
        currentX = x[i];
        currentY = y[i];

        if (currentY == prevY) 
            continue;

        int leftX;
        if (currentX < prevX) 
        {
            if (x >= prevX) continue;
            leftX = currentX;
        }
        else 
        {
            if (x >= currentX) continue;
            leftX = prevX;
        }

        float test1, test2;
        if (currentY < prevY) 
        {
            if (y < currentY || y >= prevY) 
            {
                continue;
            }
            if (x < leftX) 
            {
                ++hits;
                continue;
            }
            test1 = x - currentX;
            test2 = y - currentY;
        } 
        else 
        {
            if (y < prevY || y >= currentY) continue;
            
            if (x < leftX)
            {
                ++hits;
                continue;
            }
            test1 = x - prevX;
            test2 = y - prevY;
        }

        if (test1 < (test2 / (prevY - currentY) * (prevX - currentX))) ++hits;
    }

    return ((hits & 1) != 0);
}

template<typename T>
bool Poly2D::contains (Point<float> p)
{
    return contains (p.x, p.y);
}

template<typename T>
bool Poly2D::intersects (float x, float y, float w, float h)
{
    if (numPoints <= 0 || !getBounds ().intersects (x, y, w, h)) {
        return false;
    }

    Crossings_f* cross = getCrossings (x, y, x + w, y + h);

    return (cross == nullptr || !cross.isEmpty ());
}

template<typename T>
bool Poly2D::intersects (_Math::Rect_f r)
{
    return intersects (r.x, r.y, r.getWidth (), r.getHeight ());
}

template<typename T>
bool Poly2D::contains (float x, float y, float w, float h)
{
    if (numPoints <= 0) return false;
    if (!getBounds ().intersects (x, y, w, h)) return false;

    Crossings_f* c = getCrossings (x, y, x + w, y + h);
    return (c != nullptr && c.covers (y, y + h));
}

template<typename T>
bool Poly2D::contains (_Math::Rect_f r)
{
    return contains (r.x, r.y, r.getWidth (), r.getHeight ());
}

template<typename T>
PathIterator_f Poly2D::getPathIterator (AffineTransform_f a)
{
    return new Poly2DPathIterator_f (this, a);
}

template<typename T>
PathIterator_f Poly2D::getPathIterator (AffineTransform_f a, float flatness)
{
    return getPathIterator (a);
}

/*
Crossings_f Poly2D::getCrossings (float xlo, float ylo, float xhi, float yhi)
{
    Crossings_f cross = new Crossings_f.EvenOdd (xlo, ylo, xhi, yhi);
    int prevX = x[numPoints - 1];
    int prevY = y[numPoints - 1];
    int currentX, currentY;

    // Walk the edges of the polygon
    for (int i = 0; i < numPoints; i++) {
        currentX = x[i];
        currentY = y[i];
        if (cross.accumulateLine (prevX, prevY, currentX, currentY)) {
            return nullptr;
        }
        prevX = currentX;
        prevY = currentY;
    }

    return cross;
}*/

//-----------------------------------------------------------------------------

Poly2DPathIterator_f::Poly2DPathIterator_f (Poly2D p, AffineTransform_f a)
:   polygon   (p),
    transform (a)
{
    
    if (p.getNumPoints () == 0)
        index = 1;
}

int Poly2DPathIterator_f::getWindingRule()
{
    return WIND_EVEN_ODD;
}

bool Poly2DPathIterator_f::isDone()
{
    return index > polygon.getNumPoints ();
}

void Poly2DPathIterator_f::getNext()
{
    index++;
}

int Poly2DPathIterator_f::currentSegment (float* coords)
{
    if (index >= polygon.getNumPoints ())
        return SEG_CLOSE;
    
    coords[0] = polygon.getXPoints ()[index];
    coords[1] = polygon.getYPoints ()[index];
    if (transform != nullptr) {
        transform.transform (coords, 0, coords, 0, 1);
    }
    return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

int Poly2DPathIterator_f::currentSegment (float* coords)
{
    if (index >= polygon.getNumPoints ())
        return SEG_CLOSE;
    
    coords[0] = polygon.x[index];
    coords[1] = polygon.y[index];
    if (transform != nullptr)
        transform.transform (coords, 0, coords, 0, 1);

    return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

}   //  _Math
