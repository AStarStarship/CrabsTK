/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Search/Hit.cpp
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

#include <_Search/Hit.hpp>

#include <algorithm>

namespace _Search {

Hit::Hit (const char* aName, const char* aDescription, const char** someTags, char patch, const char* aCatagory, 
    const char* aType, int aNumTags)
{
    static int currentUId = 0;

    name = "";
    description = "";
    uid = ++currentUId;
    setName (aName);
    setDescription (aDescription);
    
    if (someTags == nullptr || aNumTags <= 0)
    {
        //tags.push_back ("");  //< Hit 0 is the new untitled patch you are editing.
        return;
    }
    
    std::string *s;
    for (int i = 0; i < aNumTags; ++i)
        ;//tags.push_back (s);
}

const char* Hit::getName () { return name; }

const char* Hit::setName (const char* aName)
{
    if (aName == nullptr) return (const char*)1;
    if (strlen (aName) > MaxnameLength) return (const char*)1;
    name = aName;
    return 0;
}

int Hit::getId () { return uid; }

void Hit::setId (int value) { uid = value; }

const char* Hit::getDescription ()
{
    return description;
}

const char* Hit::setDescription (const char* aDescription)
{
    if (aDescription == nullptr) return (const char*)1;
    if (strlen (aDescription) > MaxDescriptionLength) return aDescription;
    description = aDescription;
    return 0;
}

const char* Hit::getCatagory ()
{
    if (tags.size () == 0) return "";
    return tags[0].c_str ();
}

const char* Hit::getSubcatagory ()
{
    if (tags.size () < 0) return "";
    return tags[1].c_str ();
}

const char* Hit::addTag (const char* tag)
{
    if (tag == nullptr) return (const char*)1;
    if (strlen (tag) > MaxTagLength) return tag;
    
    tags.push_back (tag);
    return 0;
}

void Hit::addTags (char* someTags, int numTags)
{
    if (numTags < 0) return;

    for_each (tags.begin (), tags.end (), [](std::string *s)
    {
        //tags.push_back (s);
    });
}

const char* Hit::removeTag (const char* tag)
{
    for (int i = tags.size (); i >= 0; --i)
    {
        if (tags[i] == tag)
        {
            tags.erase (tags.begin() + i);
            return 0;
        }
    }
    return tag;
}

bool Hit::containsTag (const char* tag)
{
    for_each (tags.begin (), tags.end (), [] (std::string *a)
    {
        //if (a == tag) return true;
    });
    return false;
}

bool Hit::containsTags (std::vector<std::string>& someTags)
{
    /*
    for_each (tags.begin (), tags.end (), [] (string &a)
    {
        for_each (tags.begin (), tags.end (), [] (string &b)
        {
            if (a == b) return true;
        });
    });
    */
    return false;
}

std::vector<std::string>& Hit::getTags () { return tags; }

const char* Hit::toStringTags ()
{
    std::string s = "";
    int count = tags.size ();
    
    for (int i = 0; i < count; i++)
    {
        s += tags[i];
        if (i < count - 1) s += ", ";
    }
    return s.c_str;
}

const char* Hit::toJSON ()
{
    /*
    ::std::string json = "{\"Hit\":{\"name\":\"" + name + "\",\"description\":\"" + description + "\",\"tags\": [";

    tagsString = "";
    
    int count = tags.size ();
    for (int i = 0; i < count; i++)
    {
        tagsString += "\"" + tags[i];
        if (i < count - 1) tagsString += "\",";
        else tagsString += "\"";
    }
    json += tagsString + "]}}";

    return json.c_str ();
    */
    return 0;
}

void Hit::print (I2P::Terminal& slot)
{
    prints (slot, "Hit", name, "/n");
    //data.print (slot);
}

}   //< namespace _Search
