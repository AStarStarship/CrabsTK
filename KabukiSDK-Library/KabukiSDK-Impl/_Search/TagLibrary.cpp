/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Search/TagLibrary.cpp
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

#include <_Search/TagLibrary.hpp>

namespace _Search {

TagLibrary::TagLibrary ()
{

}

const char* TagLibrary::getOrAddTag (const char* Tag)
{
    /*
    if (Tag.Compare ("")) return nullptr;
    for_each (tags.begin (), tags.end (), [] (char* s)
    {
        if (s.Compare (Tag) == 0) return s;
    });
    */
    return nullptr;
}

void TagLibrary::sortTags ()
{
    //sort (tags.begin (), tags.end ());
}

int TagLibrary::getNumTags ()
{
    return tags.size ();
}

void TagLibrary::print (I2P::Terminal& slot)
{
}

}   //< namespace _Search
