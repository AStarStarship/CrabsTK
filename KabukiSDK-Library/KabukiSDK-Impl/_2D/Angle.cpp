/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_2D/Rads.cpp
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

#define _USE_MATH_DEFINES
#include <cmath>

#include <_2D/Angle.hpp>
#include <FreeI2P/IObject.hpp>

namespace _2D {

Angle_f::Angle_f (float Value)
:   Rads (Value)
{

}

float Angle_f::getDegs () { return Rads * (180.0f / M_PI); }

void Angle_f::setDegs (float Value) { Rads = (Value * M_PI) / 180.0f; }

void Angle_f::print (Terminal& slot)
{
    static const index_t header[] = { 2, STR, 32, FLT, STR, 3 };

    slot.print (header, "Angle_f: ", Rads, "\n\r");
}

Angle_d::Angle_d (double Value)
:   Rads (Value)
{

}

double Angle_d::getDegs () { return Rads * (180.0 / M_PI); }

double Angle_d::setDegs (double Value) { Rads = (Value * M_PI) / 180.0; }

void Angle_d::print (Terminal& slot)
{
    static const index_t header[] = { 2, STR, 32, FLT, STR, 3 };

    slot.print (header, "Angle_f: ", Rads, "\n\r");
}

}   //< namespace _2D
