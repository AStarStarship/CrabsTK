/** Kabuki Toolkit: pro
    @version 0.x
    @file    ~/source/kabuki/pro/impl/hmi_set.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include <set.h"

namespace kabuki { namespace hmi {

Set::Set ()
//:   controlSurfaces ()
{
}

Set::Set  (const Set& other) 
//:   controlSurfaces  (other.control_surfaces_)
{

}

Set::~Set ()
{
    for (int i = 0; i < projects.size; ++i)
        delete projects[i];
}

void Set::Add (HMI::Controller* c)
{
    if  (c == nullptr)
        return;

    projects.Add (c);
}

int Set::GetNumProject ()
{
    return projects.GetCount ();
}

}    //< namespace hmi
}    //< namespace kabuki
