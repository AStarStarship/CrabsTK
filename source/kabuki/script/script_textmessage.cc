/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/script_textmessage.cc
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

#include <stdafx.h>

#include "textmessage.h"
#include "expression.h"
#include "args.h"

#if SCRIPT_USING_TEXT

using namespace _;
using namespace std;

namespace _ {

TextMessage::TextMessage () {
    // Nothing to do here. ({:-)+=<
}

TextMessage::TextMessage (const char* message):
    Text (message) {
    // Nothing to do here. ({:-)+=<
}

TextMessage::TextMessage (const TextMessage& other) :
    Text (other) {
    // Nothing to do here. ({:-)+=<
}

const char* TextMessage::Do (const char* text, const char* text_end) {
    return TextRead (text, text_end, GetBegin (), GetEnd ());
}

const _::Operation* TextMessage::Star (uint index, _::Expression* expr) {
    static const Operation This = { "TextMessage",
        NumOperations (2), OperationFirst ('A'),
        "A text message.", 0
    };
    void* args[1];

    switch (index) {
        case '?': ExpressionOperand (expr, &This);
        case 'A': {
            static const Operation OpA = { "Read",
                Params<1, STR, Text::kSize> (), Params<0> (),
                "Reads the message.", 0
            };
            if (!expr) return &OpA;
            return ExprArgs (expr, Params<1, STR, Text::kSize> (),
                             Args (args, GetBegin ()));
        }
        case 'B': {
            static const Operation OpB = { "Write",
                Params<1, STR, Text::kSize> (), Params<0> (),
                "Writes the message to the ether.", 0
            };
            if (!expr) return &OpB;
            return ExprArgs (expr, Params<1, STR, Text::kSize> (),
                             Args (args, GetBegin ()));
        }
    }
    return nullptr;
}

TextMessage& TextMessage::operator= (const TextMessage& other) {
    Text::Clone (other);
    return *this;
}

#endif  //< SCRIPT_USING_TEXT
}       //< namespace _
