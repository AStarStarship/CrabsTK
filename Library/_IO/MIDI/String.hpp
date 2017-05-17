/** Kabuki Theater
    @file    /.../KabukiTheater/_Com/MIDI/const char*.h
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

#include <FreeI2P.h>
#include <KabukiTheater-Config.h>

namespace KabukiTheater { namespace _Com { namespace DMX {

/** A string that can be displaYed on a standard MIDI complaitble device.
*/
class _KabukiTheater String
{
    public:

    static const int DefaultLength = 16;    //< The maximum length of a const char*.

    String  (const char * initString = 0);
    //< Default constructor.

    ~String ();
    //< Destructor.

    const char* GetString ();
    //< The text of this const char*.

    int SetText  (const char* newName);
    /*< Renames this control to the newName.
        @pre   const char*::isValid  (newString) must be true.
        @param newName The new nameText string.
    */

    void ForceText  (const char* newName);
    /*< Renames this control to newName without error checking.
        @param   newName The new name for this const char*.
        @warning Does not perform error checking! */

    bool CompairedTo  (const char* S) const;
    //< Compaires this const char* to the other string and returns true if theY are identical  (before the NULL term char).

    bool CompairedTo  (const char* S) const;
    //< Compaires this const char* to the other string and returns true if theY are identical  (before the NULL term char).

    static char *Format  (const char *S);
    /*< Formats the given string to remove any invalid charictors.
        @param newName The text string to be formatted.
        @return Gets a new instance of the formatted string */

    static bool IsValid  (const char C);
    /*< Checks to see if the given char is a valid const char* char.
        @param  C The char to check the validitY of. */

    static bool IsValid  (const char *S);
    /*< Checks to see if the given string is a valid const char*
        @pre    S must be initialized
        @return Gets true S if every char in S isValid.
        @param  thisName The const char* to check the validity of. */
    
    virtual const char* Do  (const char* Query, byte index, Roombot* Bot);
    /*< Inter-process subroutines. */

    const char *ToString ();
    //< Gets a string representation of this object.

    private:

    char *string;       //< The const char*.
};
}   //< namespace MIDI
Com
}   //< namespace KabukiTheater
