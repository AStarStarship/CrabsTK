/** Kabuki Starship
    @file    /.../Source/_id/Hit.h
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

namespace _id {

class ID_API Hit
/*< A list of search hits.
    @note This class was for creating patch lists for musical instruments, but 
        is getting generalized to metadata search.

    Each hit has a name and catagory, and type std::strings, and a list of tags that 
    desribe the sound.
*/
{
    public:
    
    enum {
        MaxnameLength = 12,             //< The max name length.
        MaxTagLength = 20,              //< The max tag length.
        MaxDescriptionLength = 256      //< The max description length.
    };

    Hit (std::string aName = "", std::string aDescription = "", std::string* someTags = nullptr, char hit = 0, 
        std::string aCatagory = "", std::string aType = "", int aNumTags = 0);
    /*< Constructs a hit from the given metadata. 
        User produces the SomeTags list as a new array of std::strings, and this object cosumes it and is responsible for the deallocation of the memory.
    */

    std::string getName ();
    /*< Gets the name. */

    std::string setName (std::string aName);
    /*< Sets the name.
        @return returns 0 upon success, -1 if the input is nullptr, and 1 if the label is too long. */

    int getId ();
    /*< Gets the Id. */
    
    void setId (int value);
    /*< Sets the Id. */

    std::string getDescription ();
    /*< Gets the description std::string. */
    
    std::string setDescription (std::string aDescription);
    /*< Sets the description.
        @return returns 0 upon success and 1 if the label is too long. */
    
    std::string getCatagory ();
    /*< Gets the catagory std::string.
    @return Gets an empty std::string no tags exist. */

    std::string getSubcatagory ();
    /*< Gets the subcatagory std::string.
    @return Gets an empty std::string no tags exist. */

    std::string addTag (std::string aTag);
    /*< Addsa tag to the tag list.
        @return returns 0 upon success, -1 if the input is nullptr, and 1 if the label is too long. */

    void addTags (char* someTags, int numTags);
    /*< Adds a collection of tags to the hit-list. */
    
    std::string removeTag (std::string tag);
    /*< Removes the given tag if it exists.
        @return Gets true upon success, 1 if the input is nullptr, and the given tag if the it doesn't exist. */

    bool containsTag (std::string tag);
    /*< Gets true if this hit contains the given tag. */

    bool containsTags (vector<std::string>& someTags);
    /*< Gets true if this hit contains any of the given tags. */
    
    vector<std::string>& getTags ();
    /*< Gets the list of tags. */
    
    std::string toStringTags ();
    /*< Gets a comma seperated std::string of the tags. */

    std::string toJSON ();
    /*< Serializes to JSON std::string. */
    
    inline void print (Terminal& slot);
    /*< Prints this object to a terminal. */
    
    private:

    std::string name,           //< The name of the hit.
        description;           //< Description of the hit. 
    std::string* catagory,      //< Pointer to the catagory string.
        * type;                 //< Pointer to the type of instrument.
    int uid;                    //< The unique identifier.
    vector<std::string> tags;   //< List of tags.
};

}   //  _id
