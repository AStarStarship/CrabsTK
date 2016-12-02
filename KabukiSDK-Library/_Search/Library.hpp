/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Search/Library.hpp
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

namespace _Search {

#include <string>
#include <map>

#include "Hit.hpp"

/*< The main library of Hits.
    This class _KabukiSDK owns the memory for the patches.
*/
class _KabukiSDK Library
{
    public:
    
    Library ();
    /*< Simple default constructor. */
    
    bool addHit (Hit& p);
    /*< Adds a patch to the Hits. */
    
    void sortTags ();
    /*< Sorts the tags alphabetically. */
    
    void loadTestHites ();
    
    void loadFromJSON (string jsonString);
    /*< Loads a patch from a JSON string. */
    
    string toJSON ();
    /*< Converts a patch to a JSON string. */
    
    /*< Gets true if their is a duplicate patch name. */
    bool findDuplicateName (string value);
    
    /*< Gets the catagory name at the given index. */
    string getCatagoryName (int index);
    
    Library find (string tag);
    
    Library find (::vector<::string>& tags);
    
    string getCatagoryImageName (::string s);
    /*< Gets the subcatagory image name. */
    
    string getSubcatagoryImageName (::string s);
    /*< Gets the subcatagory image name. */

    inline void print (Terminal& slot);
    /*< Prints this object to a Terminal. */

    inline void print (Terminal& slot);
    /*< Prints this object to a Terminal. */
    
    private:

    static int currentUId;                      //< The total number of patches.
    
    ::map<::string, Hit> patches;       //< The list of patches.
    
    ::vector<::string> catagories,    //< The list of catagories.
        tags;                                   //< List of tag strings.
    
    ::string& getDefaultHitLibrary ();
    //< Gets the default library in JSON.

    int initNumHits ();
    //< Gets the number 0.
};
}   //< namespace _Search

