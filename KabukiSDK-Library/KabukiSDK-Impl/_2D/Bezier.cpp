/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_2D/Bezier.cpp
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

#include <cassert>
#include <string>

#include <_Math/Calc.hpp>

#include <_2D/Bezier.hpp>

namespace _2D {

Bezier_f::Bezier_f (const float* XPoints, const float* YPoints, int NumPoints)
:   xPoints ((float*)XPoints),
    yPoints ((float*)YPoints),
    numPoints (NumPoints),
    isDynamic (false)
{
    assert (XPoints != nullptr);
    assert (YPoints != nullptr);
    assert (NumPoints >= 0);
}

Bezier_f::Bezier_f (float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
:   xPoints (new float[3]),
    yPoints (new float[3]),
    numPoints (3),
    isDynamic (false)
{
    numPoints = 4;
    xPoints.resize (4);
    yPoints.resize (4);

    xPoints[0] = x0;
    yPoints[0] = y0;
    xPoints[1] = x1;
    yPoints[1] = y1;
    xPoints[2] = x2;
    yPoints[2] = y2;
    xPoints[3] = x3;
    yPoints[3] = y3;
}

Bezier_f::Bezier_f (const Bezier_f& O)
{
    int newNumPoints = O.numPoints;
    numPoints = newNumPoints;
    xPoints.resize (newNumPoints);
    yPoints.resize (newNumPoints);

    for (int i = 0; i < newNumPoints; ++i)
    {
        xPoints[i] = O.xPoints[i];
        yPoints[i] = O.yPoints[i];
    }
}

Point_f Bezier_f::getPoint (float P)
{
    float x = 0,                            //< The x position to return.
        y = 0,                              //< The y position to return.
        factn = factoral (numPoints);       //< Local copy of n!.

    int n = numPoints;                      //< Local copy of numPoints as a float.

    for (int i = 0; i <= n; i++)
    {
        float b = factn / (factoral (i) * factoral (n - i)), //< Calculate binomial coefficient
            k = Math.pow (1 - P, n - i) * Math.pow (P, i);   //< Calculate powers

     // Add weighted points to totals
        x += b * k * XPoints[i];
        y += b * k * YPoint[i];
    }

    return new Point_f.Double (x, y);
}

int Bezier_f::Factoral (int value)
{
    // return special case
    if (value == 0)
        return 1;

    if (value < 0)
        value *= -1;

    // Calculate factoral of value.

    int total = value;
    while (--value > 1)
        total *= value;

    return total;
}

int Bezier_f::getNumPoints ()
{
    return numPoints;
}

void Bezier_f::print (I2P::Terminal& slot)
{

}

}   //< namespace _2D
