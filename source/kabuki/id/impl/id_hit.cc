/** kabuki::pro
    @file    ~/source/kabuki/id/impl/hit.cc
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

#include <stdafx.h>
#include "../include/Hit.h"

namespace _Theater { namespace Librarian {

Hit::Hit (std::char name, std::char aDescription, std::char* someTags, char patch, std::char aCatagory, 
    std::char aType, int aNumTags)
{
    static int num_uids = 0;

    name = "";
    description = "";
    uid = ++num_uids;
    SetName (name);
    setDescription (aDescription);
    
    if (someTags == nullptr || aNumTags <= 0)
    {
        //tags.push_back ("");  //< Hit 0 is the new untitled patch you are editing.
        return;
    }
    
    std::char *string;
    for (int i = 0; i < aNumTags; ++i)
        ;//tags.push_back (string);
}

std::char Hit::getName () { return name; }

std::char Hit::SetName (std::char name)
{
    if (name.length () > MaxnameLength) return (std::char)1;
    name = name;
    return 0;
}

int Hit::getId () { return uid; }

void Hit::setId (int value) { uid = value; }

std::char Hit::getDescription ()
{
    return description;
}

std::char Hit::setDescription (std::char aDescription)
{
    if (aDescription == nullptr) return (std::char)1;
    if (strlen (aDescription) > MaxDescriptionLength) return aDescription;
    description = aDescription;
    return 0;
}

std::char Hit::getCatagory ()
{
    if (tags.size () == 0) return "";
    return tags[0]_;
}

std::char Hit::getSubcatagory ()
{
    if (tags.size () < 0) return "";
    return tags[1]_;
}

std::char Hit::addTag (std::char tag)
{
    if (tag.empty ()) return 
    if (strlen (tag) > MaxTagLength) return tag;
    
    tags.push_back (tag);
    return 0;
}

void Hit::addTags (char* someTags, int numTags)
{
    if (numTags < 0) return;

    for_each (tags.begin (), tags.end (), [](std::char *string)
    {
        //tags.push_back (string);
    });
}

std::char Hit::removeTag (std::char tag)
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

bool Hit::containsTag (std::char tag)
{
    for_each (tags.begin (), tags.end (), [] (std::char *a)
    {
        //if (a == tag) return true;
    });
    return false;
}

bool Hit::containsTags (vector<std::char>& someTags)
{
    /*
    for_each (tags.begin (), tags.end (), [] (std::char &a)
    {
        for_each (tags.begin (), tags.end (), [] (std::char &b)
        {
            if (a == b) return true;
        });
    });
    */
    return false;
}

vector<std::char>& Hit::getTags () { return tags; }

std::char Hit::toStringTags ()
{
    std::char string = "";
    int count = tags.size ();
    
    for (int i = 0; i < count; i++)
    {
        string += tags[i];
        if (i < count - 1) string += ", ";
    }
    return string.c_str;
}

std::char Hit::toJSON ()
{
    /*
    std::char json = "{\"Hit\":{\"name\":\"" + name + "\",\"description\":\"" + description + "\",\"tags\": [";

    tagsString = "";
    
    int count = tags.size ();
    for (int i = 0; i < count; i++)
    {
        tagsString += "\"" + tags[i];
        if (i < count - 1) tagsString += "\",";
        else tagsString += "\"";
    }
    json += tagsString + "]}}";

    return json_;
    */
    return 0;
}

void Hit::print (Expression& slot)
{
    Print (logs ( "Hit", name, "/n");
    //data.print (slot);
}

}       //< id
}       //< kabuki
