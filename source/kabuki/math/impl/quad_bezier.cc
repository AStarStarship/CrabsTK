/** Kabuki Theater
    @file    /.../Source-Impl/_Math/QuadBezier.cpp
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

#include <_Math/QuadBezier.h>

#include <_Math/Calc.h>

#include <cassert>
#include <string>

namespace _Math {

QuadBezier::QuadBezier (const Type* initXPoints, const Type* initYPoints)
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

QuadBezier::QuadBezier (Type initX0, Type initY0, 
    Type initX1, Type initY1, 
    Type initX2, Type initY2, 
    Type initX3, Type initY3)
:   x0 (initX0),
    x1 (initX1),
    x2 (initX2),
    x3 (initX3)
{
}

QuadBezier::QuadBezier (const QuadBezier& o)
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

Type* QuadBezier::getXPoints ()
{
    return &x0;
}

Type* QuadBezier::getYPoints ()
{
    return &y0;
}

void QuadBezier::setXPoints (const Type* xPoints)
{
    x0 = xPoints[0];
    x1 = xPoints[1];
    x2 = xPoints[2];
    x3 = xPoints[3];
}

void QuadBezier::setYPoints (const Type* yPoints)
{
    y0 = yPoints[0];
    y1 = yPoints[1];
    y2 = yPoints[2];
    y3 = yPoints[3];
}

void QuadBezier::setPoints (Type newX0, Type newY0,
    Type newX1, Type newY1,
    Type newX2, Type newY2,
    Type newX3, Type newY3)
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

void QuadBezier::setPoints (const Type* xPoints, const Type* yPoints)
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

int QuadBezier::getNumPoints ()
{
    return NumPoints;
}

Point<Type> QuadBezier::getPoint (Type t)
{
    Type x = (1 - t) * (1 - t) * x0 + 2 * (1 - t) * t * x1 + t * t * x2,
        y = (1 - t) * (1 - t) * y0 + 2 * (1 - t) * t * y1 + t * t * y2;

    return Point<Type> (x, y);
}

void QuadBezier::print (Terminal& slot)
{

}

}   //  _Math
