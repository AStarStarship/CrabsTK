/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_2D/QuadBezier.cpp
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

#include <_2D/QuadBezier.hpp>

#include <_Math/Calc.hpp>

#include <cassert>
#include <string>

namespace _2D {

QuadBezier_f::QuadBezier_f (const float* initXPoints, const float* initYPoints)
{
    x0 = initXPoints[0];
    x1 = initXPoints[1];
    x2 = initXPoints[2];
    x3 = initXPoints[3];
    y0 = initXPoints[0];
    y1 = initXPoints[1];
    y2 = initXPoints[2];
    y3 = initXPoints[3];
}

QuadBezier_f::QuadBezier_f (float initX0, float initY0, 
    float initX1, float initY1, 
    float initX2, float initY2, 
    float initX3, float initY3)
:   x0 (initX0),
    x1 (initX1),
    x2 (initX2),
    x3 (initX3)
{
}

QuadBezier_f::QuadBezier_f (const QuadBezier_f& o)
:   x0 (o.x0),
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

float* QuadBezier_f::getXPoints ()
{
    return &x0;
}

float* QuadBezier_f::getYPoints ()
{
    return &y0;
}

void QuadBezier_f::setXPoints (const float* xPoints)
{
    x0 = xPoints[0];
    x1 = xPoints[1];
    x2 = xPoints[2];
    x3 = xPoints[3];
}

void QuadBezier_f::setYPoints (const float* yPoints)
{
    y0 = yPoints[0];
    y1 = yPoints[1];
    y2 = yPoints[2];
    y3 = yPoints[3];
}

void QuadBezier_f::setPoints (float newX0, float newY0,
    float newX1, float newY1,
    float newX2, float newY2,
    float newX3, float newY3)
{
    x0 = newX0;
    y0 = newY0;
    x1 = newX1;
    y1 = newY1;
    x2 = newX2;
    y2 = newY2;
    x3 = newX3;
    y3 = newY3;
}

void QuadBezier_f::setPoints (const float* xPoints, const float* yPoints)
{
    x0 = xPoints[0];
    x1 = xPoints[1];
    x2 = xPoints[2];
    x3 = xPoints[3];

    y0 = yPoints[0];
    y1 = yPoints[1];
    y2 = yPoints[2];
    y3 = yPoints[3];
}

int QuadBezier_f::getNumPoints ()
{
    return NumPoints;
}

Point_f QuadBezier_f::getPoint (float t)
{
    float x = (1 - t) * (1 - t) * x0 + 2 * (1 - t) * t * x1 + t * t * x2,
        y = (1 - t) * (1 - t) * y0 + 2 * (1 - t) * t * y1 + t * t * y2;

    return Point_f (x, y);
}

void QuadBezier_f::print (Terminal& slot)
{

}

}   //  _2D
