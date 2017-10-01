/** Kabuki Theater
    @file    /.../Source/_id/HitList.h
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

#include <KabukiTheater-Config.h>
#include "Hit.h"

namespace _id {

/** A collection of Hit(s).
*/
class ID_API HitList {
    public:

    /** Simple default constructor. */
    HitList ();

    /** Adds a patch to the Hits. */
    bool AddHit (Hit& p);
    
    /** Sorts the tags alphabetically. */
    void SortTags ();

    /** Loads a patch from a JSON string. */
    void LoadFromJSON (std::string json);

    /** Converts a patch to a JSON string. */
    std::string ToJSON ();

    /** Gets true if their is a duplicate patch name. */
    bool FindDuplicateName (std::string value);

    /** Gets the catagory name at the given index. */
    std::string GetCatagoryName (int index);
    
    /** Gets a HitList of the given tag. */
    HitList* findTag (std::string tag);
    
    /** Gets a HitList of the given tags. */
    HitList* findTags (vector<std::string>& tags);
    
    /** Ges the catagory image name. */
    std::string getCatagoryImageName (std::string s);

    /** Ges the subcatagory image name. */
    std::string getSubcatagoryImageName (std::string s);

    /** Prints this object to the stdout. */
    void Print ();
    
    private:

    long uid = 0;
    
    vector<Hit&> patches;               //< The list of patches.
    vector<std::string> catagories,     //< The list of catagories.
        tags;                           //< List of tag strings.

    void loadTestHits ();
    /*< Loads up some test data: delete me and make unit test! */
};

}   //  _id
