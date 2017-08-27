/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Elipse.hpp
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

#include "_G/Elipse_f.hpp"

namespace _G {

Elipse_f::Elipse_f (_2D::Point<float> position, _2D::Vect_f dimensions)
:   Entity_f (relativePosition, dimensions)
{

}

float Elipse_f::getRX ()
{
    return rx;
}

void Elipse_f::setRX (float value)
{
    if (value < 0.0) 
    {
        rx = value * -1.0;
        return;
    }
    rx = value;
}

float Elipse_f::getRY ()
{
    return ry;
}

void Elipse_f::setRY (float value)
{
    if (value < 0.0) 
    {
        ry = value * -1.0;
        return;
    }
    ry = value;
}

}   //< _G
