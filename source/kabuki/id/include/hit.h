/** kabuki::id
    @file    ~/source/kabuki/id/include/Hit.h
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

#include "id_config.h"

namespace kabuki { namespace id {

class KABUKI Hit
/** A list of search hits.
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

    Hit (std::char name = "", std::char aDescription = "", std::char* someTags = nullptr, char hit = 0, 
        std::char aCatagory = "", std::char aType = "", int aNumTags = 0);
    /** Creates a hit from the given metadata. 
        User produces the SomeTags list as a new array of std::strings, and this object cosumes it and is responsible for the deallocation of the memory.
    */

    std::char getName ();
    /** Gets the name. */

    std::char SetName (std::char name);
    /** Sets the name.
        @return returns 0 upon success, -1 if the input is nullptr, and 1 if the label is too long. */

    int getId ();
    /** Gets the Id. */
    
    void setId (int value);
    /** Sets the Id. */

    std::char getDescription ();
    /** Gets the description std::char. */
    
    std::char setDescription (std::char aDescription);
    /** Sets the description.
        @return returns 0 upon success and 1 if the label is too long. */
    
    std::char getCatagory ();
    /** Gets the catagory std::char.
    @return Gets an empty std::char no tags exist. */

    std::char getSubcatagory ();
    /** Gets the subcatagory std::char.
    @return Gets an empty std::char no tags exist. */

    std::char addTag (std::char aTag);
    /** Addsa tag to the tag list.
        @return returns 0 upon success, -1 if the input is nullptr, and 1 if the label is too long. */

    void addTags (char* someTags, int numTags);
    /** Adds a collection of tags to the hit-list. */
    
    std::char removeTag (std::char tag);
    /** Removes the given tag if it exists.
        @return Gets true upon success, 1 if the input is nullptr, and the given tag if the it doesn't exist. */

    bool containsTag (std::char tag);
    /** Gets true if this hit contains the given tag. */

    bool containsTags (vector<std::char>& someTags);
    /** Gets true if this hit contains any of the given tags. */
    
    std::vector<std::char>& getTags ();
    /** Gets the list of tags. */
    
    std::char toStringTags ();
    /** Gets a comma seperated std::char of the tags. */

    std::char toJSON ();
    /** Serializes to JSON std::char. */
    
    void Print (Expression& slot);
    /** Prints this object to a expression. */
    
    private:

    std::char name,           //< The name of the hit.
        description;           //< Description of the hit. 
    std::char* catagory,      //< Pointer to the catagory char.
        * type;                 //< Pointer to the type of instrument.
    int uid;                    //< The unique identifier.
    std::vector<std::char> tags;   //< List of tags.
};

}       //< namespace id}       //< namespace kabuki
