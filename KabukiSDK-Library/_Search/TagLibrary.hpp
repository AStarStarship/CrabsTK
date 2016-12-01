/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Search/TagLibrary.hpp
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
 
#pragma once

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

#include <string>
#include <vector>

namespace _Search {

class _KabukiSDK TagLibrary
/*< A container of strings sorted alphabetically.
    This object owns the memory for the strings. Each time a patch is added,
    each tag is added, a pointer to the string is passed back.
*/
{
    public:

    TagLibrary ();
    /*< Constructs an empty tag library. */

    const char* getOrAddTag (const char* tag);
    /*< Gets the tag string, and adds it to the collection if it doesn't exist.
        @return Gets null if the tags list doesn't contatin the Tag, and 
            non-null if the Tag was added successfully. */
    
    void sortTags ();
    /*< Sorts the tags alphabetically for fast binary search. */
    
    int getNumTags ();
    /*< Gets the number of tags. */

    inline void print (I2P::Terminal& slot);
    /*< Prints this object to a Terminal. */
    
    private:
    
    ::std::vector<::std::string> tags;     //< Collection of tag strings.
};
}   //< namespace _Search

