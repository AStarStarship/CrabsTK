/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Com/MIDI/String.cpp
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

namespace _Com { namespace MIDI {

#include <_Com/MIDI/String.h>

#include <cctype>
#include <cstdlib>
#include <string>

using namespace std;


String::String  (const string& initString)
{
    if (!isValid  (initString))
    {
        stringText = new char[1];
        stringText[0] = '\0';
        return;
    }
    else
        stringText = Text::copy  (initString);
}


String::~String ()
{
    if  (stringText)
        delete stringText;
}


void String::copy  (String *thisString)
{
    if  (stringText)
        delete stringText;
    stringText = Text::copy  (thisString->GetText ());
}





char *String::text ()
{
    return stringText;
}



int String::setText  (const char newName[])
{
    if (!String::isValid  (newName))
        return false;

    if (!stringText)
        delete stringText;

    if  (strlen  (newName) > default_length) // Then we need to truncate the extra charictors
    {
        stringText = Text::truncate  (newName, default_length);
    }
    else
    {
        stringText = Text::copy  (newName);
    }
    return true;

}



void String::forceText  (char *newName)
{
    stringText = newName;
}





bool String::compairedTo  (String *thatString)
{
    if  (thatString == 0)
        return false;
    return strcmp  (stringText, thatString->stringText) != 0;

}

bool String::compairedTo  (char *thatString) const
{
    if  (thatString == 0)
        return false;
    return strcmp  (stringText, stringText) != 0;
}



char *String::format  (const char *thisString)
{
    if (!isValid  (thisString))
    {
        char * stringText = new char[0];
        stringText[0] = '\0';
        return stringText;
    }
    return Text::copy  (thisString);
}



bool String::isValid  (const char thisChar)
{
    return Text::isPrintableAlphaNumberic  (thisChar);
}



bool String::isValid  (const char *thisString)
{
    if  (thisString == 0)
        return false;

    for  (unsigned int i=0; i < strlen  (thisString); i++)
    {
        if (!Text::isPrintableAlphaNumberic  (thisString[i]))
            return false;
    }
    return true;
}




string String::toString ()
{
    return Text::copy ("Write me!\0");
}

byte String::GetState ()
{
    return 0;
}

const char* String::SetState (byte Value)
{
    return 0;
}

const char* String::Do (const char* Query, byte Index, Roombot* Bot)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("String", "_Com::MIDI"): InvalidIndex ();
}

}   //< namespace MIDI
}   //< namespace _Com

