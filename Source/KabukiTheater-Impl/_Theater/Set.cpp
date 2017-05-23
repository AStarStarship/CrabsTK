/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/Set.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <_Theater/Set.hpp>

namespace _Theater {

Set::Set ()
//:   controlSurfaces ()
{
}

Set::Set  (const Set& o) 
//:   controlSurfaces  (O.controlSurfaces)
{

}

Set::~Set ()
{
    for (int i = 0; i < projects.size; ++i)
        delete projects[i];
}

void Set::add (HMI::Controller* c)
{
    if  (c == nullptr)
        return;

    projects.add (c);
}

int Set::getNumProject ()
{
    return projects.count ();
}

}   //< namespace _Theater
