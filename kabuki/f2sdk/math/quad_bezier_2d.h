/** Kabuki Toolkit
    @file    ~/kt/math/quad_bezier2d.h
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

#include "config.h"

#include "point2d.h"

#include <vector>
#include <string>
#include <memory>

namespace _ {

/** Class that represents a 4th order Bezier. */
template<typename T>
class KABUKI_TOOLKIT QuadBezier2D
{
    public:

    enum {
        NumPoints = 4     //< 4 points in a quad Bezier curve.
    };
    
    /** Constructor.
        @pre The length of XPoints and YPoints must equal NumPoints or curve will not be created correctly. */
    QuadBezier2D (const T* initXPoints, const T* initYPoints) {
        x0 = initXPoints[0];
        x1 = initXPoints[1];
        x2 = initXPoints[2];
        x3 = initXPoints[3];
        y0 = initXPoints[0];
        y1 = initXPoints[1];
        y2 = initXPoints[2];
        y3 = initXPoints[3];
    }

    /** Constructs a bezier curve with the given points. */
    QuadBezier2D (T initX0, T initY0, T initX1, T initY1, T initX2, T initY2, 
                T initX3, T initY3) :
        x0 (initX0),
        x1 (initX1),
        x2 (initX2),
        x3 (initX3)
    {
    }

    /** Copy constructor initializes curve with the given points. */
    QuadBezier2D (const QuadBezier2D& o) :
        x0 (o.x0),
        y0 (o.y0),
        x1 (o.x1),
        y1 (o.y1),
        x2 (o.x2),
        y2 (o.y2),
        x3 (o.x3),
        y3 (o.y3)
    {
        /// Nothing to do here! :-)
    }

    /** Gets a pointer to the x points. */
    T* GetXPoints () { return &x0; }

    /** Gets a pointer to the y points. */
    T* GetYPoints () { return &y0; }

    /** Sets the x and y points to the given arrays. */
    void SetXPoints (const T* x_points) {
        x0 = x_points[0];
        x1 = x_points[1];
        x2 = x_points[2];
        x3 = x_points[3];
    }

    /** Sets the x and y points to the given arrays. */
    void SetYPoints (const T* y_points) {
        y0 = y_points[0];
        y1 = y_points[1];
        y2 = y_points[2];
        y3 = y_points[3];
    }

    /** Sets the x and y points to the given arrays. */
    void SetPoints (const T* x_points, const T* y_points) {
        x0 = x_points[0];
        x1 = x_points[1];
        x2 = x_points[2];
        x3 = x_points[3];

        y0 = y_points[0];
        y1 = y_points[1];
        y2 = y_points[2];
        y3 = y_points[3];
    }

    /** Sets the x and y values to the new ones. */
    void SetPoints (T new_x0, T newY0, T newX1, T newY1, T newX2, T newY2, T newX3, T newY3) {
        x0 = new_x0;
        y0 = new_y0;
        x1 = new_x1;
        y1 = new_y1;
        x2 = new_x2;
        y2 = new_y2;
        x3 = new_x3;
        y3 = new_y3;
    }

    /** Gets the NumPoints. */
    int GetNumPoints () {
        return NumPoints;
    }

    /** Gets a Point2D<T> along the path of the of curve at time t. */
    Point2D<T> GetPoint (T t) {
        T x = (1 - t) * (1 - t) * x0 + 2 * (1 - t) * t * x1 + t * t * x2,
            y = (1 - t) * (1 - t) * y0 + 2 * (1 - t) * t * y1 + t * t * y2;

        return Point3D<T> (x, y);
    }

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& print) {
        return print;
    }

    private:

    T x0, //< x0 coordinate.
      x1, //< x1 coordinate.
      x2, //< x2 coordinate.
      x3, //< x3 coordinate.
      y0, //< y0 coordinate.
      y1, //< y1 coordinate.
      y2, //< y2 coordinate.
      y3; //< y3 coordinate.
};

}       //< namespace _
