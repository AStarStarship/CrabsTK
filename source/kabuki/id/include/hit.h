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
#define KABUKI_ID_HITLIST_H

#include "module_config.h"
#include "../../data/include/array.h"

namespace kabuki { namespace id {

/** A list of search hits.
    @note This class was for creating patch lists for musical instruments, but 
        is getting generalized to metadata search.

    Each hit has a name and category, and type std::strings, and a list of tags that 
    describe the sound.
*/
class KABUKI Hit {
    public:
    
    enum {
        MaxnameLength = 12,             //< The max name length.
        MaxTagLength = 20,              //< The max tag length.
        MaxDescriptionLength = 256      //< The max description length.
    };

    /** Creates a hit from the given metadata.
    User produces the SomeTags list as a new array of std::strings, and this object consumes it and is responsible for the deallocation of the memory.
    */
    Hit (const char* name = "", const char* description = "", const char** tags = nullptr, char hit = 0, 
        const char* aCategory = "", const char* aType = "", int num_tags = 0);

    /** Gets the name. */
    const char* GetName ();

    /** Sets the name.
        @return returns 0 upon success, -1 if the input is nullptr, and 1 if the label is too long. */
    const char* SetName (const char* name);

    /** Gets the Id. */
    int GetId ();

    /** Sets the Id. */
    void SetId (int value);

    /** Gets the description const char*. */
    const char* GetDescription ();
    
    /** Sets the description.
        @return returns 0 upon success and 1 if the label is too long. */
    const char* SetDescription (const char* description);

    /** Gets the category const char*.
        @return Gets an empty const char* no tags exist. */
    const char* GetCategory ();

    /** Gets the subcategory const char*.
    @return Gets an empty const char* no tags exist. */
    const char* GetSubcategory ();

    /** Adds a tag to the tag list.
        @return returns 0 upon success, -1 if the input is nullptr, and 1 if the label is too long. */
    const char* AddTag (const char* tag);

    /** Adds a collection of tags to the hit-list. */
    void AddTags (char* tags, int num_tags);
    
    /** Removes the given tag if it exists.
        @return Gets true upon success, 1 if the input is nullptr, and the given tag if the it doesn't exist. */
    const char* RemoveTag (const char* tag);

    /** Gets true if this hit contains the given tag. */
    bool ContainsTag (const char* tag);

    /** Gets true if this hit contains any of the given tags. */
    bool ContainsTags (data::StringArray* tags);

    /** Gets the list of tags. */
    data::StringArray* GetTags ();
    
    /** Gets a comma separated const char* of the tags. */
    const char* ToStringTags ();

    /** Serializes to JSON const char*. */
    const char* ToJson ();
    
    /** Prints this object to a expression. */
    void Print (_::Log& log);
    
    private:

    const char             * name_,        //< The name of the hit.
                           * description_; //< Description of the hit. 
    const char            ** category_,    //< Pointer to the category char.
                           * type_;        //< Pointer to the instrument type.
    uid_t                    uid_;         //< The unique identifier.
    data::StringArray      * tags_;        //< List of tags.
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_HITLIST_H
