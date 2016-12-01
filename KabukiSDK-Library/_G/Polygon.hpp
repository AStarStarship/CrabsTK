/** Kabuki Software Development Kit
    @file    /.../Kabuki-Library/_G/ErrorList.cpp
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

#include "_2D/Polygon_i.h"

namespace _G {
    
/** A 2D polygon. */
class _G_API Polygon32
{
    public:
    
	Polygon_i ();

    Polygon()
    {
        xPoints = new int[MinLength];
        yPoints = new int[MinLength];
    }

    Polygon (int xPoints[], int yPoints[], int numPoints) 
    {
        // Fix 4489009: should throw IndexOutofBoundsException instead
        // of OutofMemoryException if numPoints is huge and > {x,y}points.length
        if  (numPoints > xPoints.length || numPoints > yPoints.length) {
            throw new IndexOutOfBoundsException("numPoints > xPoints.length || "+
                                                "numPoints > yPoints.length");
        }
        // Fix 6191114: should throw NegativeArraySizeException with
        // negative numPoints
        if  (numPoints < 0)
        {
            throw new NegativeArraySizeException("numPoints < 0");
        }
        // Fix 6343431: Applet compatibility problems if arrays are not
        // exactly numPoints in length
        numPoints = numPoints;
        xPoints = Arrays.copyOf (xPoints, numPoints);
        yPoints = Arrays.copyOf (yPoints, numPoints);
    }

    void reset()
    {
        numPoints = 0;
        bounds = nullptr;
    }

    void invalidate()
    {
        bounds = nullptr;
    }

    void translate (int deltaX, int deltaY)
    {
        for  (int i = 0; i < numPoints; i++)
        {
            xPoints[i] += deltaX;
            yPoints[i] += deltaY;
        }
        if  (bounds != nullptr)
        {
            bounds.translate (deltaX, deltaY);
        }
    }

    void calculateBounds (int xPoints[], int yPoints[], int numPoints)
    {
        int boundsMinX = Integer.MAX_VALUE;
        int boundsMinY = Integer.MAX_VALUE;
        int boundsMaxX = Integer.MIN_VALUE;
        int boundsMaxY = Integer.MIN_VALUE;

        for  (int i = 0; i < numPoints; ++i)
        {
            int x = xPoints[i];
            boundsMinX = Math.min (boundsMinX, x);
            boundsMaxX = Math.max (boundsMaxX, x);
            int y = yPoints[i];
            boundsMinY = Math.min (boundsMinY, y);
            boundsMaxY = Math.max (boundsMaxY, y);
        }
        bounds = new Rectangle (boundsMinX, boundsMinY,
                               boundsMaxX - boundsMinX,
                               boundsMaxY - boundsMinY);
    }

    void updateBounds (int x, int y)
    {
        if  (x < bounds.x)
        {
            bounds.width = bounds.width +  (bounds.x - x);
            bounds.x = x;
        }
        else
        {
            bounds.width = Math.max (bounds.width, x - bounds.x);
            // bounds.x = bounds.x;
        }

        if  (y < bounds.y)
        {
            bounds.height = bounds.height +  (bounds.y - y);
            bounds.y = y;
        }
        else
        {
            bounds.height = Math.max (bounds.height, y - bounds.y);
            // bounds.y = bounds.y;
        }
    }

    void addPoint (int x, int y)
    {
        if  (numPoints >= xPoints.length || numPoints >= yPoints.length)
        {
            int newLength = numPoints * 2;
            // Make sure that newLength will be greater than MinLength and
            // aligned to the power of 2
            if  (newLength < MinLength) {
                newLength = MinLength;
            } else if ((newLength &  (newLength - 1)) != 0) {
                newLength = Integer.highestOneBit (newLength);
            }

            xPoints = Arrays.copyOf (xPoints, newLength);
            yPoints = Arrays.copyOf (yPoints, newLength);
        }
        xPoints[numPoints] = x;
        yPoints[numPoints] = y;
        numPoints++;
        if  (bounds != nullptr)
        {
            updateBounds (x, y);
        }
    }

    Rectangle getBounds()
    {
        return getBoundingBox();
    }


    bool contains (Point_f p)
    {
        return contains (p.x, p.y);
    }

    bool contains (int x, int y)
    {
        return contains((float) x,  (float) y);
    }

    Rect2D_f getBounds2D()
    {
        return getBounds();
    }

    bool contains (float x, float y)
    {
        if  (numPoints <= 2 || !getBoundingBox().contains (x, y)) {
            return false;
        }
        int hits = 0;

        int lastx = xPoints[numPoints - 1];
        int lasty = yPoints[numPoints - 1];
        int curx, cury;

        // Walk the edges of the polygon
        for  (int i = 0; i < numPoints; lastx = curx, lasty = cury, i++) {
            curx = xPoints[i];
            cury = yPoints[i];

            if  (cury == lasty) {
                continue;
            }

            int leftx;
            if  (curx < lastx) {
                if  (x >= lastx) {
                    continue;
                }
                leftx = curx;
            } else {
                if  (x >= curx) {
                    continue;
                }
                leftx = lastx;
            }

            float test1, test2;
            if  (cury < lasty) {
                if  (y < cury || y >= lasty) {
                    continue;
                }
                if  (x < leftx) {
                    hits++;
                    continue;
                }
                test1 = x - curx;
                test2 = y - cury;
            } else {
                if  (y < lasty || y >= cury) {
                    continue;
                }
                if  (x < leftx) {
                    hits++;
                    continue;
                }
                test1 = x - lastx;
                test2 = y - lasty;
            }

            if  (test1 <  (test2 /  (lasty - cury) *  (lastx - curx))) {
                hits++;
            }
        }

        return ((hits & 1) != 0);
    }

    bool contains (Point2D p)
    {
        return contains (p.getX(), p.getY());
    }

    bool intersects (float x, float y, float w, float h)
    {
        if  (numPoints <= 0 || !getBoundingBox().intersects (x, y, w, h)) {
            return false;
        }

        Crossings cross = getCrossings (x, y, x+w, y+h);
        return  (cross == nullptr || !cross.isEmpty());
    }

    bool intersects (Rect2D_f r)
    {
        return intersects (r.getX(), r.getY(), r.getWidth(), r.getHeight());
    }

    bool contains (float x, float y, float w, float h)
    {
        if  (numPoints <= 0 || !getBoundingBox().intersects (x, y, w, h)) {
            return false;
        }

        Crossings cross = getCrossings (x, y, x+w, y+h);
        return  (cross != nullptr && cross.covers (y, y+h));
    }

    bool contains (Rect2D_f r)
    {
        return contains (r.getX(), r.getY(), r.getWidth(), r.getHeight());
    }

    PathIterator getPathIterator (AffineTransform at)
    {
        return new PolygonPathIterator (this, at);
    }

    PathIterator getPathIterator (AffineTransform at, float flatness)
    {
        return getPathIterator (at);
    }

    protected:

    Rectangle bounds;
    
    private:
	
	_G::Color_i color;  //< The color of the polygon.

    int numPoints;

    int xPoints[];

    int yPoints[];

    static final long serialVersionUID = -6460061437900069969L;

    enum {
        MinLength = 4       //< The minimum length of a Polygon
    };

    Crossings getCrossings (float xlo, float ylo, float xhi, float yhi)
    {
        Crossings cross = new Crossings.EvenOdd (xlo, ylo, xhi, yhi);
        int lastx = xPoints[numPoints - 1];
        int lasty = yPoints[numPoints - 1];
        int curx, cury;

        // Walk the edges of the polygon
        for  (int i = 0; i < numPoints; i++) {
            curx = xPoints[i];
            cury = yPoints[i];
            if  (cross.accumulateLine (lastx, lasty, curx, cury)) {
                return nullptr;
            }
            lastx = curx;
            lasty = cury;
        }

        return cross;
    }
};



class Polygon32PathIterator: public PathIterator 
{
    Polygon poly;
    AffineTransform transform;
    int index;

    PolygonPathIterator (Polygon pg, AffineTransform at)
    {
        poly = pg;
        transform = at;
        if  (pg.numPoints == 0) {
            // Prevent a spurious SEG_CLOSE segment
            index = 1;
        }
    }

    int getWindingRule()
    {
        return WIND_EVEN_ODD;
    }

    bool isDone()
    {
        return index > poly.numPoints;
    }

    void next()
    {
        index++;
    }

    int currentSegment (float[] coords)
    {
        if  (index >= poly.numPoints) {
            return SEG_CLOSE;
        }
        coords[0] = poly.xPoints[index];
        coords[1] = poly.yPoints[index];
        if  (transform != nullptr) {
            transform.transform (coords, 0, coords, 0, 1);
        }
        return  (index == 0 ? SEG_MOVETO : SEG_LINETO);
    }

    int currentSegment (float[] coords)
    {
        if  (index >= poly.numPoints) {
            return SEG_CLOSE;
        }
        coords[0] = poly.xPoints[index];
        coords[1] = poly.yPoints[index];
        if  (transform != nullptr) {
            transform.transform (coords, 0, coords, 0, 1);
        }
        return  (index == 0 ? SEG_MOVETO : SEG_LINETO);
    }
};
}