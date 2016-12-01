/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/Bezier.hpp
    @author     Cale McCollough
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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

#include "Point.hpp"

#include <vector>
#include <string>
#include <memory>

namespace _2D {

class _KabukiSDK Bezier4_f
/*< Class that represents a 4th order Bezier. */
{
    public:

    enum {
        NumPoints = 4     //< 4 points in a quad Bezier curve.
    };

    Bezier4_f (const float* initXPoints, const float* initYPoints);
    /*< Constructor.
        @pre The length of XPoints and YPoints must equal NumPoints or curve will not be created correctly. */

    Bezier4_f (float initX0, float initY0, float initX1, float initY1, float initX2, float initY2, float initX3, float initY3);
    /*< Constructs a bezier curve with the given points. */

    Bezier4_f (const Bezier4_f& o);
    /*< Copy constructor initializes curve with the given points. */

    float* getXPoints ();
    /*< Gets a pointer to the x points. */

    float* getYPoints ();
    /*< Gets a pointer to the y points. */

    void setXPoints (const float* xPoints);
    /*< Sets the x and y points to the given arrays. */

    void setYPoints (const float* yPoints);
    /*< Sets the x and y points to the given arrays. */

    void setPoints (const float* xPoints, const float* yPoints);
    /*< Sets the x and y points to the given arrays. */

    void setPoints (float initX0, float initY0, float initX1, float initY1, float initX2, float initY2, float initX3, float initY3);
    /*< Sets the x and y values to the new ones. */

    int getNumPoints ();
    /*< Gets the NumPoints. */

    Point_f getPoint (float t);
    /*< Gets a Point_f along the path of the of curve at time t. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */

    private:

    float x0,   //< The x0 coordinate.
        x1,     //< The x1 coordinate.
        x2,     //< The x2 coordinate.
        x3,     //< The x3 coordinate.
        y0,     //< The y0 coordinate.
        y1,     //< The y1 coordinate.
        y2,     //< The y2 coordinate.
        y3;     //< The y3 coordinate.
};

}   //< namespace _2D
