/** Kabuki Software Development Kit
    @file       /.../KabukiSDK-Impl/_Math/Imaginary.cpp
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright 2016 (C) Cale McCollough <cale.mccollough@gmail.com>

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

#include <_Math/Imaginary.hpp>
#include <_Math/Calc.hpp>

namespace _Math {

Imaginary_f::Imaginary_f ()
{

}

Imaginary_f::Imaginary_f (float Ax, float By)
:   A (Ax),
    B (By)
{
}

float Imaginary_f::getA () const
{
    return A;
}

float Imaginary_f::getB () const
{
    return B;
}

void Imaginary_f::setA (float Value)
{
    A = Value;
}

void Imaginary_f::setB (float Value)
{
    B = Value;
}

float Imaginary_f::calcAngle ()
{
    return atan (A/B);
}

float Imaginary_f::calcMagnitude ()
{
    return squareRoot (A * A + B * B);
}

void Imaginary_f::print (I2P::Terminal& slot)
{
}

Imaginary_d::Imaginary_d ()
{
}

Imaginary_d::Imaginary_d (double Ax, double By)
:   A (Ax),
    B (By)
{
}

double Imaginary_d::getA () const
{
    return A;
}

double Imaginary_d::getB () const
{
    return B;
}

void Imaginary_d::setA (double Value)
{
    A = Value;
}

void Imaginary_d::setB (double Value)
{
    B = Value;
}

double Imaginary_d::calcAngle ()
{
    return atan (A/B);
}

double Imaginary_d::calcMagnitude ()
{
    return squareRoot (A * A + B * B);
}

void Imaginary_d::print (I2P::Terminal& slot)
{

}

}   //< namespace _Math
