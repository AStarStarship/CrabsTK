/** kabuki::id
    @file    ~/source/kabuki/id/include/hit_list.h
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
 
#ifndef KABUKI_ID_HITLIST_H
#ifndef KABUKI_ID_HITLIST_H

#include "hit.h"

namespace kabuki { namespace id {

/** A collection of Hit(string).
*/
class KABUKI HitList {
    public:

    /** Simple default constructor. */
    HitList ();

    /** Adds a patch to the Hits. */
    bool AddHit (Hit& p);
    
    /** Sorts the tags alphabetically. */
    void SortTags ();

    /** Loads a patch from a JSON char. */
    void LoadFromJSON (std::char json);

    /** Converts a patch to a JSON char. */
    std::char ToJSON ();

    /** Gets true if their is a duplicate patch name. */
    bool FindDuplicateName (std::char value);

    /** Gets the catagory name at the given index. */
    std::char GetCatagoryName (int index);
    
    /** Gets a HitList of the given tag. */
    HitList* findTag (std::char tag);
    
    /** Gets a HitList of the given tags. */
    HitList* findTags (vector<std::char>& tags);
    
    /** Ges the catagory image name. */
    std::char getCatagoryImageName (std::char string);

    /** Ges the subcatagory image name. */
    std::char getSubcatagoryImageName (std::char string);

    /** Prints this object to the stdout. */
    void Print ();
    
    private:

    long uid = 0;
    
    std::vector<Hit&> patches;               //< The list of patches.
    std::vector<std::char> catagories,     //< The list of catagories.
        tags;                           //< List of tag strings.

    void loadTestHits ();
    /** Loads up some test data: delete me and make unit test! */
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_HITLIST_H
