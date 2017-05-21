/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_Theater/Librarian/Hit.cpp
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

#include <_Id/Hit.hpp>

#include <algorithm>

namespace _Theater { namespace Librarian {

Hit::Hit (std::string aName, std::string aDescription, std::string* someTags, char patch, std::string aCatagory, 
    std::string aType, int aNumTags)
{
    static int numUIDs = 0;

    name = "";
    description = "";
    uid = ++numUIDs;
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

std::string Hit::getName () { return name; }

std::string Hit::setName (std::string aName)
{
    if (aName.length () > MaxnameLength) return (std::string)1;
    name = aName;
    return 0;
}

int Hit::getId () { return uid; }

void Hit::setId (int value) { uid = value; }

std::string Hit::getDescription ()
{
    return description;
}

std::string Hit::setDescription (std::string aDescription)
{
    if (aDescription == nullptr) return (std::string)1;
    if (strlen (aDescription) > MaxDescriptionLength) return aDescription;
    description = aDescription;
    return 0;
}

std::string Hit::getCatagory ()
{
    if (tags.size () == 0) return "";
    return tags[0].c_str ();
}

std::string Hit::getSubcatagory ()
{
    if (tags.size () < 0) return "";
    return tags[1].c_str ();
}

std::string Hit::addTag (std::string tag)
{
    if (tag.empty ()) return 
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

std::string Hit::removeTag (std::string tag)
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

bool Hit::containsTag (std::string tag)
{
    for_each (tags.begin (), tags.end (), [] (std::string *a)
    {
        //if (a == tag) return true;
    });
    return false;
}

bool Hit::containsTags (vector<std::string>& someTags)
{
    /*
    for_each (tags.begin (), tags.end (), [] (std::string &a)
    {
        for_each (tags.begin (), tags.end (), [] (std::string &b)
        {
            if (a == b) return true;
        });
    });
    */
    return false;
}

vector<std::string>& Hit::getTags () { return tags; }

std::string Hit::toStringTags ()
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

std::string Hit::toJSON ()
{
    /*
    std::string json = "{\"Hit\":{\"name\":\"" + name + "\",\"description\":\"" + description + "\",\"tags\": [";

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

void Hit::print (Terminal& slot)
{
    slot.prints ( "Hit", name, "/n");
    //data.print (slot);
}

}   //< Librarian
}   //< _Theater
