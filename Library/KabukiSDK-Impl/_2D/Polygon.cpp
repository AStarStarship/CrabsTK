/** Kabuki Software Development Kit
    @file    /.../Kabuki-Library/_2D/Polygon.cpp
    @author  Edited by Cale McCollough
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

#include <_2D/Polygon.hpp>

#include <_Math/Calc.hpp>
using namespace _Math;

namespace _2D {

Polygon_f::Polygon_f()
:   numPoints (MinBufferSize),
    bufferSize (MinBufferSize),
    x (new float[MinNumPoints]),
    y (new float[MinNumPoints])
{
    /// Nothing to do here! :-)
}

Polygon_f::Polygon_f (float* initXPoints, float* initYPoints, int initNumPoints, int initBufferSize)
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

Polygon_f::~Polygon_f ()
{
    delete x;
    delete y;
}

void Polygon_f::reset()
{
    // Write me!
}

float* Polygon_f::getXPoints ()
{
    return x;
}

float* Polygon_f::getYPoints ()
{
    return y;
}

int Polygon_f::getNumPoints ()
{
    return numPoints;
}

int Polygon_f::getBufferSize ()
{
    return bufferSize;
}

void Polygon_f::translate (float dx, float dy)
{
    for (int i = 0; i < numPoints; i++)
    {
        x[i] += dx;
        y[i] += dy;
    }

    bounds.translate (dx, dy);
}

void Polygon_f::translate (int dx, int dy)
{
    translate ((float)dx, (float)dy);
}

void Polygon_f::translate (int dx, int dy)
{
    translate ((float)dx, (float)dy);
}

void Polygon_f::calculateBounds (float* x, float* y, int numPoints)
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

void Polygon_f::updateBounds (int x, int y)
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

void Polygon_f::addPoint (int newX, int newY)
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

void Polygon_f::addPoint (int newX, int newY)
{
    addPoint ((float)newX, (float)newY);
}

const Rect_f& Polygon_f::getBounds ()
{
    return bounds;
}

bool contains (int x, int y)
{
    return contains((float) x, (float) y);
}

bool Polygon_f::contains (Point_f p)
{
    return contains (p.x, p.y);
}

const Rect_f& Polygon_f::getBounds ()
{
    return getBounds ();
}

bool Polygon_f::contains (float x, float y)
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

bool Polygon_f::contains (Point_f p)
{
    return contains (p.x, p.y);
}

bool Polygon_f::intersects (float x, float y, float w, float h)
{
    if (numPoints <= 0 || !getBounds ().intersects (x, y, w, h)) {
        return false;
    }

    Crossings_f* cross = getCrossings (x, y, x + w, y + h);

    return (cross == nullptr || !cross.isEmpty ());
}

bool Polygon_f::intersects (_2D::Rect_f r)
{
    return intersects (r.x, r.y, r.getWidth (), r.getHeight ());
}

bool Polygon_f::contains (float x, float y, float w, float h)
{
    if (numPoints <= 0) return false;
    if (!getBounds ().intersects (x, y, w, h)) return false;

    Crossings_f* c = getCrossings (x, y, x + w, y + h);
    return (c != nullptr && c.covers (y, y + h));
}

bool Polygon_f::contains (_2D::Rect_f r)
{
    return contains (r.x, r.y, r.getWidth (), r.getHeight ());
}

PathIterator_f Polygon_f::getPathIterator (AffineTransform_f a)
{
    return new PolygonPathIterator_f (this, a);
}

PathIterator_f Polygon_f::getPathIterator (AffineTransform_f a, float flatness)
{
    return getPathIterator (a);
}

/*
Crossings_f Polygon_f::getCrossings (float xlo, float ylo, float xhi, float yhi)
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

PolygonPathIterator_f::PolygonPathIterator_f (Polygon_f p, AffineTransform_f a)
:   polygon   (p),
    transform (a)
{
    
    if (p.getNumPoints () == 0)
        index = 1;
}

int PolygonPathIterator_f::getWindingRule()
{
    return WIND_EVEN_ODD;
}

bool PolygonPathIterator_f::isDone()
{
    return index > polygon.getNumPoints ();
}

void PolygonPathIterator_f::getNext()
{
    index++;
}

int PolygonPathIterator_f::currentSegment (float* coords)
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

int PolygonPathIterator_f::currentSegment (float* coords)
{
    if (index >= polygon.getNumPoints ())
        return SEG_CLOSE;
    
    coords[0] = polygon.x[index];
    coords[1] = polygon.y[index];
    if (transform != nullptr)
        transform.transform (coords, 0, coords, 0, 1);

    return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

}   //  _2D
