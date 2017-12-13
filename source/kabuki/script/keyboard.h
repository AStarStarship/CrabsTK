/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/keyboard.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_SCRIPT_KEYBOARD
#define HEADER_FOR_SCRIPT_KEYBOARD

#include "config.h"

#if SCRIPT_USING_KEYBOARD
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
    @param  header     The header to print.
    @param  target     Start of the buffer to read to.
    @param  target_end End of the buffer to read too.
    @return Returns a new char that you have to delete. */
KABUKI void KeyboardText (const char* header, char* target, char* target_end);

}       //< namespace _
#endif  //< SCRIPT_USING_KEYBOARD
#endif  //< HEADER_FOR_SCRIPT_KEYBOARD
