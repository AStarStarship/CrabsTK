/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/operation.h
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

#ifndef HEADER_FOR_SCRIPT_OPERATION
#define HEADER_FOR_SCRIPT_OPERATION

#include "text.h"

namespace _ {

/** Key/name char, params, result, and optional description of a data set.
    @code
    static const Operation this_evaluation = { "Key",
        Params<1, 2>::Header, Params<1, 2>::Header,
        "Description" };
    static const Operation member_device =   { "Key", 
        NumOperations (0), FirstOperation ('A'),
        "Description" };
    @endcode
*/
struct KABUKI Operation {
    const char   * name;       //< Operation name.
    const uint_t * params,     //< Operation parameters B-Seq.
                 * result;     //< Operation result B-Seq.
    const char   * description;//< Operation description.
    const byte   * evaluation; //< Evaluated Operation b-sequence.
};

/** Converts the value to a pointer. */
inline const uint_t* OperationCount (uint_t value) {
    return reinterpret_cast<const uint_t*> (value);
}

/** Converts the given value to a pointer. 
inline uint_t OperationCount (const uint_t* params) {
    return (uint_t)reinterpret_cast<uintptr_t> (params);
}*/

/** Converts the given value to a pointer. */
inline uint_t OperationCount (const Operation& operation) {
    return (uint_t)reinterpret_cast<uintptr_t> (operation.params);
}

/** Converts the given value to a pointer. */
inline const uint_t* OperationFirst (uint_t value) {
    uintptr_t index = value;
    return reinterpret_cast<const uint_t*>(index);
}

/** Converts the given value to a pointer. 
inline uint_t OperationFirst (const uint_t* result) {
    if (!result) {
        return 0;
    }
    return (uint_t)reinterpret_cast<uintptr_t> (result);
}*/

/** Converts the given value to a pointer. */
inline uint_t OperationFirst (const Operation& operation) {
    return (uint_t)reinterpret_cast<uintptr_t> (operation.result);
}

/** Converts the given value to a pointer. */
inline uintptr_t OperationLast (const Operation* operaion) {
    if (!operaion) {
        return 0;
    }
    return reinterpret_cast<uintptr_t> (operaion->params) +
           reinterpret_cast<uintptr_t> (operaion->result);
}

/** Evaluated Nil Operation result. */
KABUKI const Operation* OperationNil ();

/** Evaluated Invalid Operation result. */
KABUKI const Operation* OperationInvalid ();

#if SCRIPT_USING_TEXT
/** Prints the given Set to the std::out
    Quote: Wikipedia "In mathematics an operand is the object of a mathematical
    operation, i.e. it is the quantity that is operated on." */
KABUKI Text& OperationPrint (const Operation* op, Text& text);
#endif  //< SCRIPT_USING_TEXT

}   //< namespace _

#if SCRIPT_USING_TEXT

/** Overloaded operator<< prints the given operation to the text. */
KABUKI _::Text& operator<< (_::Text& text, const _::Operation* operation);

#endif  //< SCRIPT_USING_TEXT
#endif  //< HEADER_FOR_SCRIPT_OPERATION
