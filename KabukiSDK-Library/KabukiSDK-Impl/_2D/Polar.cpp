/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_2D/Polar.cpp
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

#include <math.h>

#include <_2D/Polar.hpp>

namespace _2D {

Polar_f::Polar_f (float angle, float radius)
    :   Angle (angle),
    Radius (radius)
{
}

float Polar_f::getAngle ()              { return Angle; }
float Polar_f::getRadius ()             { return Radius; }
void Polar_f::setAngle (float angle)    { Angle = angle; }
void Polar_f::setRadius (float radius)  { Radius = radius; }

float Polar_f::calcMagnitude ()
{
    float angle = Angle,
        radius = Radius;
    return sqrt (angle * angle + radius * radius);
}

void Polar_f::print (I2P::Terminal& slot)
{

}

Polar_d::Polar_d (double angle, double radius)
:   Angle (angle),
    Radius (radius)
{
}

double Polar_d::getAngle ()             { return Angle; }
double Polar_d::getRadius ()            { return Radius; }
void Polar_d::setAngle (double angle)   { Angle = angle; }
void Polar_d::setRadius (double radius) { Radius = radius; }

double Polar_d::calcMagnitude ()
{
    double angle = Angle,
        radius = Radius;
    return sqrt (angle * angle + radius * radius);
}

void Polar_d::print (I2P::Terminal& slot)
{

}

}   //< namespace _2D
