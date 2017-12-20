/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/text.h
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

#ifndef HEADER_FOR_SCRIPT_TEXTMESSAGE
#define HEADER_FOR_SCRIPT_TEXTMESSAGE

#include "expression.h"
#if SCRIPT_USING_TEXT
namespace _ {

/** A string that can be mapped between multiple systems. */
class TextMessage : public Text, public Operation {
    public:

    /** Constructs a blank TextMessage. */
    TextMessage ();

    /** Constructs a blank TextMessage from the given strand. */
    TextMessage (const char* strand);

    /** Constructs a blank TextMessage from the given strand. */
    TextMessage (const TextMessage& other);

    /** Handles Script Commands.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text, const char* strand_end);

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Operation* Star (uint index, Expression* expr);

    /** Overloaded operator= clones the other TextMessage. */
    TextMessage& operator= (const TextMessage& other);
};

}       //< namespace _
#endif  //< SCRIPT_USING_TEXT
#endif  //< HEADER_FOR_SCRIPT_TEXTMESSAGE
