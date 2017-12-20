/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script_operation.cc
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
#include "operation.h"
#include "params.h"

namespace _ {

const Operation* OperationNil () {
    static const Operation Result = { "Nil",
        OperationCount (0), OperationFirst (' '),
        nullptr, 0 };
    return &Result;
}

const Operation* OperationInvalid () {
    static const Operation Result = { "Invalid",
        OperationCount (1), OperationFirst (' '),
        nullptr, 0 };
    return &Result;
}

#if SCRIPT_USING_TEXT
Text& OperationPrint (const Operation* operation, Text& text) {
    if (!operation) {
        return text << "\n| Operation: null";
    }
    return text << "\n| Operation   :" << operation->name
               << "\n| Params      :" << ParamsPrint (operation->params, text)
               << "\n| Result:     :" << ParamsPrint (operation->result, text)
               << "\n| Metadata:   :" << operation->description
               << "\n| Evaluation  : @todo Print me!";
}
#endif  //< SCRIPT_USING_TEXT

}   //< namespace _


#if SCRIPT_USING_TEXT

_::Text& operator<< (_::Text& text, const _::Operation* operation) {
    return OperationPrint (operation, text);
}

#endif  //< SCRIPT_USING_TEXT
