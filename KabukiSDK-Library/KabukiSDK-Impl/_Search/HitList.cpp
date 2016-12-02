/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Search/HitList.cpp
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

#include <_Search/HitList.hpp>

#include <regex>

namespace _Search {

HitList::HitList ()
{
    loadTestHits ();
}

bool HitList::addHit (Hit& p)
{
    patches.push_back (p);

    /// Check the tags for duplicates and add to tags if no duplicates exist.
    /*
    string tagCatagory = p.getCatagory ();
    for_each (patches.begin (), patches.end (), [](string s) { if (s == tagCatagory) return true; }
    tags.push_back (tagCatagory);
    sort (tags.begin (), tags.end ());
    return true;
    */
    return false;
}

void HitList::sortTags () 
{ 
    //sort (tags.begin (), tags.end ());
}

void HitList::loadFromJSON (string json)
{
    /*
    try
    {
        patches = new vector<Hit> ();
        JArray a = JArray.Parse (json);
        foreach (JObject o in a.Children<JObject> ())
        {
            Hit p = o.toObject<Hit> ();
        }
    }
    catch (Exception e)
    {
        assert (false, e.print (Terminal& slot));
    }
     */
}

string HitList::toJSON ()
{
    /*
    ::string json = "[";
    int count = patches.size ();
    for (int i = 0; i < count; i++)
    {
        json += patches[i].toJSON ();
        if (i < count - 1) json += ", ";
    }
    json += "]";
    return json.c_str ();
    */
    return 0;
}

bool HitList::findDuplicateName (string value)
{
    /*
    for_each (patches.begin (), patches.end (), [](Hit& hit) 
    {
    if (a.getName () == value)
        return true;
    });
    */
    return false;
}

string HitList::getCatagoryName (int index)
{
    if (index >= catagories.size ()) return nullptr;
    
    return catagories[index].c_str ();
}

HitList* HitList::findTag (string tag)
{
    /*
    vector<Hit> patchList;

    for_each (Hit patch in patches)
    if (patch.ContainsTag (tag))
        patchList.add (patch);
    return patchList;
    */
    return 0;
}

HitList* HitList::findTags (::vector<::string> tags)
{
    /*
    ::vector<Hit> patchList;
    for_each (patches.begin (), patches.end (), [] (Hit& p)
    {
        if (patch.ContainsTags (tags)) patchList.add (patch);
    });
    return patchList;
    */
    return 0;
}

string HitList::getCatagoryImageName (string s)
{
    if (s == nullptr || s == "") return nullptr;
    ::regex regularExpression ("\s+");
    ::string result = "Catagory" + ::regex_replace (s, regularExpression, "");// + ".png";
    return result.c_str ();
}

string HitList::getSubcatagoryImageName (string s)
{
    if (s == nullptr || s == "") return nullptr;
    ::regex regularExpression ("\s+");
    ::string result = "Subcatagory" + ::regex_replace (s, regularExpression, "");// + ".png";
    return result.c_str ();
}

void HitList::print (Terminal& slot)
{
}

void HitList::loadTestHits ()
{
    patches.clear ();
    /*
    addHit (new Hit ("Claranet", "Default claranet patch.", { "Woodwind", "Claranet", "Reed" }));
    addHit (new Hit ("Oboe", "Default oboe patch.", { "Woodwind", "Oboe", "Reed" }));
    addHit (new Hit ("Tenor Sax", "Default tenor sax patch.", { "Woodwind", "Tenor Sax", "Reed", "Tenor" }));
    addHit (new Hit ("Recorder", "Default recorder patch.", { "Woodwind", "Recorder", "Flute" }));
    addHit (new Hit ("Flute", "Default flute patch.", { "Woodwind", "Flute" }));

    addHit (new Hit ("Trumpet", "Default trumpet patch.", { "Brass", "Trumpet" }));
    addHit (new Hit ("FlugleHorn","Default flugle horn patch.", { "Brass", "Flugle Horn" }));
    addHit (new Hit ("FrenchHorn", "Default french horn patch.", { "Brass", "French Horn" }));
    addHit (new Hit ("Trombone", "Default trombone patch.", { "Brass", "Trombone" }));
    */
}

}   //< namespace _Search
