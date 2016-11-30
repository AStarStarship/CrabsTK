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

#include <_2D/Bezier.hpp>

#include <_Math/Calc.hpp>

#include <cassert>
#include <string>

namespace _2D {

Bezier_f::Bezier_f (float* theXPoints, float* theYPoints, int theNumPoints)
:   xPoints ((float*)theXPoints),
    yPoints ((float*)theYPoints),
    numPoints (theNumPoints),
    isDynamic (true)
{
    assert (theXPoints != nullptr);
    assert (theYPoints != nullptr);
    assert (theNumPoints >= 0);
}

Bezier_f::Bezier_f (const float* theXPoints, const float* theYPoints, int theNumPoints)
:   xPoints ((float*)theXPoints),
    yPoints ((float*)theYPoints),
    numPoints (theNumPoints),
    isDynamic (false)
{
    assert (theXPoints != nullptr);
    assert (theYPoints != nullptr);
    assert (theNumPoints >= 0);
}

Bezier_f::Bezier_f (float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
:   xPoints (new float[3]),
    yPoints (new float[3]),
    numPoints (3),
    isDynamic (true)
{
    numPoints = 4;

    xPoints[0] = x0;
    yPoints[0] = y0;
    xPoints[1] = x1;
    yPoints[1] = y1;
    xPoints[2] = x2;
    yPoints[2] = y2;
    xPoints[3] = x3;
    yPoints[3] = y3;
}

Bezier_f::Bezier_f (const Bezier_f& o)
{
    int newNumPoints = o.numPoints;
    numPoints = newNumPoints;
    float* newXPoints = new float[newNumPoints],
        * newYPoints = new float[newNumPoints];
    xPoints = newXPoints;
    yPoints = newYPoints;

    for (int i = 0; i < newNumPoints; ++i)
    {
        newXPoints[i] = o.xPoints[i];
        newYPoints[i] = o.yPoints[i];
    }
}

Bezier_f::~Bezier_f ()
{
    if (~isDynamic) return;
    delete xPoints;
    delete yPoints;
}

Point_f Bezier_f::getPoint (float P)
{
    float x = 0,                                //< The x position to return.
        y = 0,                                  //< The y position to return.
        factn = _Math::factoral (numPoints);    //< Local copy of n!.

    int n = numPoints;                          //< Local copy on stack.

    for (int i = 0; i <= n; i++)
    {
        //< calculate binomial coefficient.
        float b = factn / (_Math::factoral (i) * _Math::factoral (n - i));

        //< calculate powers
        float k = _Math::power (1 - P, n - i) * _Math::power (P, i);          

     // Add weighted points to totals
        x += b * k * xPoints[i];
        y += b * k * yPoints[i];
    }

    return Point_f (x, y);
}

int Bezier_f::getNumPoints ()
{
    return numPoints;
}

void Bezier_f::print (I2P::Terminal& slot)
{

}

}   //< namespace _2D
