/** Kabuki Toolkit
    @file    ~/kt/math/poly2d.cc
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

#include "calc.h"
using namespace math;

namespace math {

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
    int boundsMinX = GetMinFloat (),
        boundsMinY = GetMinFloat (),
        boundsMaxX = GetMaxFloat (),
        boundsMaxY = GetMaxFloat ();

    for (int i = 0; i < numPoints; ++i)
    {
        float xi = x[i],
            yi = y[i];

        boundsMinX = Min (boundsMinX, xi);
        boundsMaxX = Max (boundsMaxX, xi);

        boundsMinY = Min (boundsMinY, yi);
        boundsMaxY = Max (boundsMaxY, yi);
    }
    return Rect2D (boundsMinX, boundsMinY, boundsMaxX - boundsMinX, boundsMaxY - boundsMinY);
}

template<typename T>
void Poly2D::UpdateBounds (int x, int y)
{
    if (x < bounds.x) {
        bounds.setWidth (bounds.getWidth () + (bounds.x - x));
        bounds.x = x;
    }
    else {
        bounds.setWidth (max (bounds.getWidth (), x - bounds.x));
        // bounds.x = bounds.x;
    }

    if (y < bounds.y) {
        bounds.setHeight (bounds.getHeight () + (bounds.y - y));
        bounds.y = y;
    }
    else {
        bounds.setHeight (max (bounds.getHeight (), y - bounds.y));
        // bounds.y = bounds.y;
    }
}

template<typename T>
void Poly2D::AddPoint (int newX, int newY)
{
    if (numPoints >= bufferSize) {  //< Then double the buffer size.
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
void Poly2D::AddPoint (int newX, int newY) {
    AddPoint ((float)newX, (float)newY);
}

template<typename T>
const Rect2D& Poly2D::GetBounds () {
    return bounds;
}

template<typename T>
bool Contains (int x, int y) {
    return Contains((float) x, (float) y);
}

template<typename T>
bool Poly2D::Contains (Point<float> p) {
    return Contains (p.x, p.y);
}

template<typename T>
const Rect2D& Poly2D::GetBounds () {
    return GetBounds ();
}

template<typename T>
bool Poly2D::Contains (float x, float y) {
    if (numPoints < 3)                  return false;
    if (!GetBounds ().Contains (x, y))  return false;

    int hits = 0;

    float prev_x = x[numPoints - 1],
        prev_y = y[numPoints - 1],
        current_x,
        current_y;

    // Walk the edges of the polygon
    for (int i = 0; i < numPoints; prev_x = current_x, prev_y = current_y, i++) {
        current_x = x[i];
        current_y = y[i];

        if (current_y == prev_y) 
            continue;

        int leftX;
        if (current_x < prev_x) {
            if (x >= prev_x) continue;
            leftX = current_x;
        }
        else  {
            if (x >= current_x) continue;
            leftX = prev_x;
        }

        float test1, test2;
        if (current_y < prev_y) {
            if (y < current_y || y >= prev_y) {
                continue;
            }
            if (x < leftX) {
                ++hits;
                continue;
            }
            test1 = x - current_x;
            test2 = y - current_y;
        } 
        else {
            if (y < prev_y || y >= current_y) continue;
            
            if (x < leftX) {
                ++hits;
                continue;
            }
            test1 = x - prev_x;
            test2 = y - prev_y;
        }

        if (test1 < (test2 / (prev_y - current_y) * (prev_x - current_x))) ++hits;
    }

    return ((hits & 1) != 0);
}

template<typename T>
bool Poly2D::Contains (Point<float> p) {
    return Contains (p.x, p.y);
}

template<typename T>
bool Poly2D::Intersects (float x, float y, float w, float h) {
    if (numPoints <= 0 || !GetBounds ().Intersects (x, y, w, h)) {
        return false;
    }

    Crossings_f* cross = GetCrossings (x, y, x + w, y + h);

    return (cross == nullptr || !cross.IsEmpty ());
}

template<typename T>
bool Poly2D::intersects (Rect2D<T> r) {
    return Intersects (r.x, r.y, r.GetWidth (), r.GetHeight ());
}

template<typename T>
bool Poly2D::Contains (float x, float y, float w, float h) {
    if (numPoints <= 0) return false;
    if (!GetBounds ().Intersects (x, y, w, h)) return false;

    Crossings_f* c = GetCrossings (x, y, x + w, y + h);
    return (c != nullptr && c.Covers (y, y + h));
}

template<typename T>
bool Poly2D::Contains (Rect2D<T> r) {
    return Contains (r.x, r.y, r.GetWidth (), r.GetHeight ());
}

template<typename T>
PathIterator_f Poly2D::GetPathIterator (AffineTransform<T> a) {
    return new Poly2DPathIterator<T> (this, a);
}

template<typename T>
PathIterator_f Poly2D::GetPathIterator (AffineTransform<T> a, float flatness) {
    return GetPathIterator (a);
}

/*
Crossings_f Poly2D::GetCrossings (float xlo, float ylo, float xhi, float yhi)
{
    Crossings_f cross = new Crossings_f.EvenOdd (xlo, ylo, xhi, yhi);
    int prev_x = x[numPoints - 1];
    int prev_y = y[numPoints - 1];
    int current_x, current_y;

    // Walk the edges of the polygon
    for (int i = 0; i < numPoints; i++) {
        current_x = x[i];
        current_y = y[i];
        if (cross.accumulateLine (prev_x, prev_y, current_x, current_y)) {
            return nullptr;
        }
        prev_x = current_x;
        prev_y = current_y;
    }

    return cross;
}*/

//-----------------------------------------------------------------------------

Poly2DPathIterator<T>::Poly2DPathIterator<T> (Poly2D p, AffineTransform<T> a)
:   polygon   (p),
    transform (a) {
    
    if (p.getNumPoints () == 0)
        index = 1;
}

int Poly2DPathIterator<T>::GetWindingRule() {
    return WIND_EVEN_ODD;
}

bool Poly2DPathIterator<T>::IsDone() {
    return index > polygon.GetNumPoints ();
}

void Poly2DPathIterator<T>::GetNext() {
    index++;
}

int Poly2DPathIterator<T>::CurrentSegment (float* coords) {
    if (index >= polygon.GetNumPoints ())
        return SEG_CLOSE;
    
    coords[0] = polygon.GetXPoints ()[index];
    coords[1] = polygon.GetYPoints ()[index];
    if (transform != nullptr) {
        transform.transform (coords, 0, coords, 0, 1);
    }
    return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

int Poly2DPathIterator<T>::CurrentSegment (float* coords) {
    if (index >= polygon.getNumPoints ())
        return SEG_CLOSE;
    
    coords[0] = polygon.x[index];
    coords[1] = polygon.y[index];
    if (transform != nullptr)
        transform.Transform (coords, 0, coords, 0, 1);

    return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

}   //  math
