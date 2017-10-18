/** Kabuki Starship
    @file    /.../Source/_id/Library.h
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

#include "Hit.h"

namespace _id {

class ID_API Library
/*< The main library of Hits.
    This class ID_API owns the memory for the patches.
*/
{
    public:
    
    Library ();
    /*< Simple default constructor. */
    
    bool addHit (Hit& p);
    /*< Adds a patch to the Hits. */
    
    void sortTags ();
    /*< Sorts the tags alphabetically. */
    
    void loadFromJSON (string jsonString);
    /*< Loads a patch from a JSON string. */
    
    string toJSON ();
    /*< Converts a patch to a JSON string. */
    
    bool findDuplicateName (string value);
    /*< Gets true if their is a duplicate patch name. */
    
    string getCatagoryName (int index);
    /*< Gets the catagory name at the given index. */
    
    Library find (string tag);
    /*< Searches for hits with the given search query. */
    
    Library find (vector<string>& tags);
    
    string getCatagoryImageName (string s);
    /*< Gets the subcatagory image name. */
    
    string getSubcatagoryImageName (string s);
    /*< Gets the subcatagory image name. */

    inline void print (Terminal& slot);
    /*< Prints this object to a Terminal. */
    
    //void injectTestData ();
    
    private:

    wint numUIDs;                           //< The total number of library itmes with a UID.

    map<string, Hit> patches;               //< The list of patches.
    vector<string> catagories,              //< The list of catagories.
        tags;                               //< List of tag strings.
    
    string& getDefaultHitLibrary ();
    /*< Gets the default library in JSON. */

    int initNumHits ();
    /*< Gets the number 0 at this point in time. */
};

}   //  _id

