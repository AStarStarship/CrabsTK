/** kabuki::pro
    @file    ~/source/kabuki/id/imp/tag_library.cc
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

#include <stdafx.h>
#include "../include/tag_library.h"

namespace kabuki { namespace id {

TagLibrary::TagLibrary ()
{

}

const char* TagLibrary::GetOrAddTag (const char* Tag)
{
    /*
    if (Tag.compare ("")) return nullptr;
    for_each (tags.begin (), tags.end (), [] (char* string)
    {
        if (string.compare (Tag) == 0) return string;
    });
    */
    return nullptr;
}

void TagLibrary::Sort ()
{
    //sort (tags_.begin (), tags_.end ());
}

int TagLibrary::GetNumTags ()
{
    return tags_.count;
}

void TagLibrary::Print (Expression& slot)
{
}

}       //< id
}       //< kabuki
