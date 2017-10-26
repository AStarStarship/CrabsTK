/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/console.h
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

#pragma once
#include <stdafx.h>

#ifndef KABUKI_SCRIPT_CONSOLE_H
#define KABUKI_SCRIPT_CONSOLE_H

#include "room.h"
#include "table.h"
#include "set.h"
#include "utils.h"
#include "expression.h"

namespace _ {

/** Reads a single char from the keyboard 
    @param header The header to print. */
KABUKI char KeyboardChar (const char* header);

/** Reads an int from the keyboard.
    @param header The header to print. 
    @param number The number to read. */
KABUKI int KeyboardInt (const char* header);

/** Reads a float from the keyboard. 
    @param header The header to print. 
    @param number The number to read. */
KABUKI float KeyboardFloat (const char* header);

/** Reads a new char from the keyboard.
    @header The header to print.
    @return Returns a new char that you have to delete. */
KABUKI char* KeyboardString (const char* header, int buffer_size);

/** Reads a new char from the keyboard.
    @param  header      The header to print.
    @param  buffer      A buffer to read keyboard input to.
    @param  buffer_size The size of the buffer.
    @return Returns a new char that you have to delete. */
KABUKI void KeyboardString (const char* header, char* buffer, int buffer_size);
  
/** A text Console.
    @todo Write me!
*/
class Console: public Room
{
    public:

    /** Creates a Console. */
    Console ();

    /** Destructor. */
    virtual ~Console ();

    /** Sets a the Room. */
    virtual const Operation* Init ();

    /** Main process loop. */
    virtual const Operation* Loop ();

    /** Chinese room expressions. */
    virtual const Operation* Star (char_t index, Expression* expr);

    private:

    // 2-to-4-byte vtable pointer here in memory!
  
};

}       //< namespace _
#endif  //< KABUKI_SCRIPT_CONSOLE_H
