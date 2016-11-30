/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_2D/Entity.cpp
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

#include <string>
using namespace std;

#include <_2D/Entity.hpp>

namespace _2D {

Entity_f::Entity_f (const char* Name, float X, float Y, float Width, float Height)
:   name (Name),
    bounds (X, Y, Width, Height)
{

}

const char* Entity_f::getName () { return name; }

void Entity_f::getName (const char* Name)
{
    name = Name; 
}

Entity_f& Entity_f::getBounds () { return bounds; }

void Entity_f::print (I2P::Terminal& slot)
{
    std::string str;
    static const index_t Header[] = { 1, STX, str.length () };
    Slot.Print ();
}

}   //< namespace _2D
