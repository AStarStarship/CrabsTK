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

#include <math.hpp>

#include <_2D/Polar.hpp>
namespace _2D {

Polar_f::Polar_f (float angle, float radius)
    :   Angle (angle),
    Radius (radius)
{
}

float Polar_f::getAngle ()           { return Angle; }
float Polar_f::getRadius ()          { return Radius; }
void Polar_f::getAngle (float A)    { Angle = A; }
void Polar_f::getRadius (float R)    { Radius = R; }

float Polar_f::CalcMagnitude ()
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

double Polar_d::getAngle ()          { return Angle; }
double Polar_d::getRadius ()          { return Radius; }
void Polar_d::getAngle (double A)    { Angle = A; }
void Polar_d::getRadius (double R)    { Radius = R; }

double Polar_d::CalcMagnitude ()
{
    double angle = Angle,
        radius = Radius;
    return sqrt (angle * angle + radius * radius);
}

void Polar_d::print (I2P::Terminal& slot)
{

}

}   //< namespace _2D
