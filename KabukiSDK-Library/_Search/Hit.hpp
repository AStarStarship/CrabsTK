/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Search/Hit.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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

#include <KabukiSDK-Config.hpp>

#include <FreeI2P.hpp>

#include <stdint.h>
#include <string.h>
#include <vector>

namespace _Search {

/** A list of search hits.
    @note This class was for creating patch lists for musical instruments, but 
        is getting generalized to metadata search.

    Each hit has a name and catagory, and type strings, and a list of tags that 
    desribe the sound.
*/
class _KabukiSDK Hit
{
    public:
    
    enum {
        MaxnameLength = 12,             //< The max name length.
        MaxTagLength = 20,              //< The max tag length.
        MaxDescriptionLength = 256      //< The max description length.
    };

    Hit (const char* aName = "", const char* aDescription = "", const char** someTags = nullptr, char hit = 0, 
        const char* aCatagory = "", const char* aType = "", int aNumTags = 0);
    /*< Constructs a hit from the given metadata. 
        User produces the SomeTags list as a new array of strings, and this object cosumes it and is responsible for the deallocation of the memory.
    */

    const char* getName ();
    /*< Gets the name. */

    const char* setName  (const char* aName);
    /*< Sets the name.
        @return returns 0 upon success, -1 if the input is nullptr, and 1 if the label is too long. */

    int getId ();
    /*< Gets the Id. */
    
    void setId  (int value);
    /*< Sets the Id. */

    const char* getDescription ();
    /*< Gets the description string. */
    
    const char* setDescription  (const char* aDescription);
    /*< Sets the description.
        @return returns 0 upon success and 1 if the label is too long. */
    
    const char* getCatagory ();
    /*< Gets the catagory const char*.
    @return Gets an empty const char* no tags exist. */

    const char* getSubcatagory ();
    /*< Gets the subcatagory const char*.
    @return Gets an empty const char* no tags exist. */

    const char* addTag  (const char* aTag);
    /*< Addsa tag to the tag list.
        @return returns 0 upon success, -1 if the input is nullptr, and 1 if the label is too long. */

    void addTags (char* someTags, int numTags);
    /*< Adds a collection of tags to the hit-list. */
    
    const char* removeTag  (const char* tag);
    /*< Removes the given tag if it exists.
        @return Gets true upon success, 1 if the input is nullptr, and the given tag if the it doesn't exist. */

    bool containsTag  (const char* tag);
    /*< Gets true if this hit contains the given tag. */

    bool containsTags (::std::vector<::std::string>& someTags);
    /*< Gets true if this hit contains any of the given tags. */
    
    ::std::vector<::std::string>& getTags ();
    /*< Gets the list of tags. */
    
    const char* toStringTags ();
    /*< Gets a comma seperated const char* of the tags. */

    const char* toJSON ();
    /*< Serializes to JSON const char*. */

    inline void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */
    
    private:

    const char* name,               //< The name of the hit.
        * description;              //< Description of the hit. 
    
    const char** catagory,          //< Pointer to the catagory const char*.
        * type;                     //< Pointer to the type of instrument.
    
    int uid;                        //< The unique identifier.

    ::std::vector<::std::string> tags;  //< List of tags.
};

}   //< namespace _Search
