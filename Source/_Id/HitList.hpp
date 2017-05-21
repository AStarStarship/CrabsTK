/** Kabuki Theater
    @file    /.../Source/_Id/HitList.hpp
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
 
#pragma once

#include <KabukiTheater-Config.hpp>
#include "Hit.hpp"

namespace _Id {

class _KabukiTheater_ HitList
/** A collection of Hit(s).
*/
{
    public:

    HitList ();
    /*< Simple default constructor. */

    bool addHit (Hit& p);
    /*< Adds a patch to the Hits. */
    
    void sortTags ();
    /*< Sorts the tags alphabetically. */

    void loadFromJSON (std::string json);
    /*< Loads a patch from a JSON string. */

    std::string toJSON ();
    /*< Converts a patch to a JSON string. */

    bool findDuplicateName (std::string value);
    /*< Gets true if their is a duplicate patch name. */

    std::string getCatagoryName (int index);
    /*< Gets the catagory name at the given index. */
    
    HitList* findTag (std::string tag);
    /*< Gets a HitList of the given tag. */
    
    HitList* findTags (vector<std::string>& tags);
    /*< Gets a HitList of the given tags. */
    
    std::string getCatagoryImageName (std::string s);
    /*< Ges the catagory image name. */

    std::string getSubcatagoryImageName (std::string s);
    /*< Ges the subcatagory image name. */

    inline void print (Terminal& slot);
    /*< Prints this object to a Terminal. */
    
    private:

    long uid = 0;
    
    vector<Hit&> patches;       //< The list of patches.
    vector<std::string> catagories,  //< The list of catagories.
        tags;                   //< List of tag strings.

    void loadTestHits ();
    /*< Loads up some test data: delete me and make unit test! */
};

}   //  _Id
