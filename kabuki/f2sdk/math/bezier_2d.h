/** Kabuki Toolkit
    @file ~/kt/math/bezier2d.h
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

#include "config.h"
#include "point2d.h"

namespace _ {

/** Class that represents a n order Bezier. */
template<typename T>
class KABUKI_TOOLKIT Bezier2D {
    public:

    enum { 
        MinmumNumPoints = 3     //< The minimum number of points reqruied for a bezier curve.
    };

    /** Constructor.
        @pre The length of XPoints and YPoints must equal NumPoints or curve will not be created correctly. */
    Bezier2D  (T* initXPoints, T* initYPoints, int initNumPoints) :
        x_points ((float*)theXPoints),
        y_points ((float*)theYPoints),
        numPoints (theNumPoints) {
        assert (theXPoints != nullptr);
        assert (theYPoints != nullptr);
        assert (theNumPoints >= 0);
    }
    
    /** Constructor.
        @pre The length of XPoints and YPoints must equal NumPoints or curve will not be created correctly. */
    Bezier2D  (const T* initXPoints, const T* initYPoints, int initNumPoints)
        : x_points ((float*)theXPoints),
        y_points ((float*)theYPoints),
        numPoints (theNumPoints)
    {
        assert (theXPoints != nullptr);
        assert (theYPoints != nullptr);
        assert (theNumPoints >= 0);
    }
    
    /** Constructs a quadratic bezier curve with the given points. */
    Bezier2D  (T x0, T y0, T x1, T y1, T x2, T y2, T x3, T y3) :
        x_points (new float[3]),
        y_points (new float[3]),
        numPoints (3) {
        numPoints = 4;

        x_points[0] = x0;
        y_points[0] = y0;
        x_points[1] = x1;
        y_points[1] = y1;
        x_points[2] = x2;
        y_points[2] = y2;
        x_points[3] = x3;
        y_points[3] = y3;
    }

    /** Copy constructor initializes curve with the given points. */
    Bezier2D  (const Bezier2D& o) {
        int newNumPoints = o.numPoints;
        numPoints = newNumPoints;
        float* newXPoints = new float[newNumPoints],
            *newYPoints = new float[newNumPoints];
        x_points = newXPoints;
        y_points = newYPoints;

        for (int i = 0; i < newNumPoints; ++i) {
            newXPoints[i] = o.x_points[i];
            newYPoints[i] = o.y_points[i];
        }
    }

    /** Destructs dynamic memory if used. */
    ~Bezier2D () {
        delete x_points;
        delete y_points;
    }

    /** Retuns a pointer to the x points. */
    T* GetXPoints () 

    /** Retuns a pointer to the y points. */
    T* GetYPoints ();

    /** Gets the number of points. */
    int GetNumPoints () {
        return numPoints;
    }

    /** Gets a Point<T> along the path of the of curve at time t. */
    Point2D<T> GetPoint  (T p) {
        float x = 0,                                //< The x position to return.
            y = 0,                                  //< The y position to return.
            factn = factoral (numPoints);    //< Local copy of n!.

        int n = numPoints;                          //< Local copy on stack.

        for (int i = 0; i <= n; i++)
        {
            //< calculate binomial coefficient.
            float b = factn / (factoral (i) * factoral (n - i));

            //< calculate powers
            float k = power (1 - p, n - i) * power (p, i);

            // Add weighted points to totals
            x += b * k * x_points[i];
            y += b * k * y_points[i];
        }

        return Point3D<float> (x, y);
    }

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& printer) {

    }

    private:

    T* x_points,  //< Array of X points.
     * y_points;  //< Array of Y points.

    int numPoints;                  //< The number of points in the polygon.
};

}       //< namespace _
