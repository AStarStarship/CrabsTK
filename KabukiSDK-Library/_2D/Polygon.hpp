/** Kabuki Software Development Kit
    @file    /.../Kabuki-Library/_G/ErrorList.cpp
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

#include "_2D/Polygon_i.h"

namespace _G {
    
class _G_API Polygon32
/** A 2D polygon that uses 32-bit floating point values. */
{
    public:
    
	Polygon_i ();

    Polygon()
    {
        xPoints = new int[MinLength];
        yPoints = new int[MinLength];
    }

    Polygon (int xPoints[], int yPoints[], int numPoints);

    void reset();

    void invalidate();

    void translate (int deltaX, int deltaY);

    void calculateBounds (int xPoints[], int yPoints[], int numPoints);

    void updateBounds (int x, int y);

    void addPoint (int x, int y);

    Rect_f getBounds();

    bool contains (Point_f p);

    bool contains (int x, int y);

    Rect2D_f getBounds2D();

    bool contains (float x, float y);

    bool contains (Point2D p);

    bool intersects (float x, float y, float w, float h);

    bool intersects (Rect2D_f r);

    bool contains (float x, float y, float w, float h);

    bool contains (Rect2D_f r);

    Polygon32PathIterator getPathIterator (AffineTransform at);

    PathIterator getPathIterator (AffineTransform at, float flatness);

    protected:

    Rect_f bounds;
    
    private:
	
	_G::Color_i color;  //< The color of the polygon.

    int numPoints;

    int* xPoints;

    int* yPoints;

    enum {
        MinLength = 4       //< The minimum length of a Polygon
    };

    Crossings getCrossings (float xlo, float ylo, float xhi, float yhi);
};



class Polygon32PathIterator: public PathIterator 
{
    Polygon poly;
    AffineTransform transform;
    int index;

    PolygonPathIterator (Polygon pg, AffineTransform at);

    int getWindingRule();

    bool isDone();

    void next();

    int currentSegment (float* coords);

    int currentSegment (float* coords);
};
}