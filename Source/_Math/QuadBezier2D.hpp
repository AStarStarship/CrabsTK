/** Kabuki Theater
    @file    /.../Source/_Math/Bezier.hpp
    @author     Cale McCollough
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


#include <KabukiTheater-Config.hpp>

#include "Point.hpp"

#include <vector>
#include <string>
#include <memory>

namespace _Math {

template<typename Type>
class _KabukiTheater_ QuadBezier
/*< Class that represents a 4th order Bezier. */
{
    public:

    enum {
        NumPoints = 4     //< 4 points in a quad Bezier curve.
    };

    QuadBezier (const Type* initXPoints, const Type* initYPoints);
    /*< Constructor.
        @pre The length of XPoints and YPoints must equal NumPoints or curve will not be created correctly. */

    QuadBezier (Type initX0, Type initY0, Type initX1, Type initY1, Type initX2, Type initY2, Type initX3, Type initY3);
    /*< Constructs a bezier curve with the given points. */

    QuadBezier (const QuadBezier& o);
    /*< Copy constructor initializes curve with the given points. */

    Type* getXPoints ();
    /*< Gets a pointer to the x points. */

    Type* getYPoints ();
    /*< Gets a pointer to the y points. */

    void setXPoints (const Type* xPoints);
    /*< Sets the x and y points to the given arrays. */

    void setYPoints (const Type* yPoints);
    /*< Sets the x and y points to the given arrays. */

    void setPoints (const Type* xPoints, const Type* yPoints);
    /*< Sets the x and y points to the given arrays. */

    void setPoints (Type initX0, Type initY0, Type initX1, Type initY1, Type initX2, Type initY2, Type initX3, Type initY3);
    /*< Sets the x and y values to the new ones. */

    int getNumPoints ();
    /*< Gets the NumPoints. */

    Point_f getPoint (Type t);
    /*< Gets a Point_f along the path of the of curve at time t. */

    inline void print (Terminal& io);
    /*< Prints this object to the terminal. */

    private:

    Type x0,   //< The x0 coordinate.
        x1,     //< The x1 coordinate.
        x2,     //< The x2 coordinate.
        x3,     //< The x3 coordinate.
        y0,     //< The y0 coordinate.
        y1,     //< The y1 coordinate.
        y2,     //< The y2 coordinate.
        y3;     //< The y3 coordinate.
};

}   //  _Math
