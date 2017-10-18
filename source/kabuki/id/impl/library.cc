/** Kabuki Starship
    @file    /.../KabukiSDK-Impl/_Id/Library.cpp
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

#include <_Id/Library.h>

namespace _Id {

Library::Library ()
{
    LoadTestHites ();
}

Library::Library (const Hit& o)
{
    
}

char* Library::getHitChangeString ()
{
}

/** Adds a patch to the patches. */
bool Library::addHit (Hit& p)
{
    patches.add (p);
    
    /// Check the tags for duplicates and add to tags if no duplicates exist.
    string tagCatagory = p.Catagory;
    foreach (string s in tags) if (s == tagCatagory) return true;
    tags.add (tagCatagory);
    sort (tags.begin (), tags.end ());
    return true;
}

void Library::sortTags () { sort (tags.begin (), tags.end ());
    
void Library::loadTestHits ()
{
    patches = new vector<Hit> (9);
        
    addHit (new Hit ("Claranet", "Default claranet patch.", { "Woodwind", "Claranet", "Reed" }));
    addHit (new Hit ("Oboe", "Default oboe patch.", { "Woodwind", "Oboe", "Reed" }));
    addHit (new Hit ("Tenor Sax", "Default tenor sax patch.", { "Woodwind", "Tenor Sax", "Reed", "Tenor" }));
    addHit (new Hit ("Recorder", "Default recorder patch.", { "Woodwind", "Recorder", "Flute" }));
    addHit (new Hit ("Flute", "Default flute patch.", { "Woodwind", "Flute" }));
        
    addHit (new Hit ("Trumpet", "Default trumpet patch.", { "Brass", "Trumpet" }));
    addHit (new Hit ("FlugleHorn","Default flugle horn patch.", { "Brass", "Flugle Horn" }));
    addHit (new Hit ("FrenchHorn", "Default french horn patch.", { "Brass", "French Horn" }));
    addHit (new Hit ("Trombone", "Default trombone patch.", { "Brass", "Trombone" }));
}
    
void Library::LoadFromJSON (string json)
{
    /*
        try
        {
        patches = new vector<Hit> ();
        JArray a = JArray.Parse (json);
        foreach (JObject o in a.Children<JObject> ())
        {
        Hit p = o.ToObject<Hit> ();
        }
        }
        catch (Exception e)
        {
        Debug.Assert (false, e.print (Terminal& slot));
        }
        */
}
    
string* Library::toJSON ()
{
    string json = new string "[";
    int count = patches.size ();
    for (int i = 0; i < count; i++)
    {
        json += patches[i].ToJSON ();
        if (i < count - 1) json += ", ";
    }
    return json + "]";
}
    
bool Library::FindDuplicateName (char* value)
{
    foreach (Hit a in patches)
    if (a.Name == value)
        return true;
    return false;
}
    
string Library::getCatagoryName (int index)
{
    if (index >= catagories.size ()) return nullptr;
        
    return catagories[index];
}
    
Library Library::FindpatchesWithTag (char* tag)
{
    vector<Hit> Library = new vector<Hit> ();
    foreach (Hit patch in patches)
    if (patch.ContainsTag (tag))
        Library.add (patch);
    return Library;
}
    
Library Library::FindpatchesWithTags (vector<string> tags)
{
    Library
    vector<Hit> Library = new vector<Hit> ();
    for_each (patches.begin (), patches.end (), [] (Hit& p)
                {
                    if (patch.ContainsTags (tags)) Library.add (patch);
                });
    return Library;
}
    
char* Library::getCatagoryImageName (string s)
{
    if (s == nullptr || s == "") return nullptr;
    return "Catagory" + ::regex_replace (s, "\s+", "");// + ".png";
}
    
char* Library::getSubcatagoryImageName (string s)
{
    if (s == nullptr || s == "") return nullptr;
    return "Subcatagory" + ::regex_replace (s, "\s+", "");// + ".png";
}
    
char* Library::DefaultHitLibrary ()
{
    return "";
}

int Library::InitNumHites () { return 0; }

void Library::print (Terminal& slot)
{
}

}   //  _Id
