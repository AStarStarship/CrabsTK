/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/op.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_SCRIPT_OP
#define HEADER_FOR_SCRIPT_OP

#include "error.h"

namespace _ {

//enum {
//    kOpPush          = 0, //< Operation Type 0: Stack push .
//    kOpOperation     = 1, //< Operation Type 1: Abstract Operation.
//    KOpOperationPush = 2, //< Operation Type 2: Operation with stack push.
//};

/** An expression Operation with name key, multiple input params,
    result, and optional description of a data set.
    @code
    static const Op kThis = { "Key",
        Bsq<1, 2>::Header, Bsq<1, 2>::Header,
        "Description", '}', ';', '-', nullptr };
    static const Op member_device =   { "Key", 
        NumOps (0), FirstOp ('A'),
        "Description", '}', ';', '-', nullptr };
    @endcode */
struct KABUKI Op {
    const char   * name;        //< Op name.
    const uint_t * in,          //< Input BSQ params or OpFirst.
                 * out;         //< Output BSQ params or OpLast.
    const char   * description; //< Op description.
    wchar_t        pop,         //< Index of the Pop Operation.
                   close,       //< Index of the Close Operation.
                   default_op,  //< Index of the Default Operation.
                   separator;   //< Char that does not exist in this context.
    const void   * evaluation;  //< Evaluated expression Slot.
};

/** Converts the given value to a pointer. */
inline intptr_t OpCount (const Op& op) {
    return op.out - op.in;
}

/** Converts the given value to a pointer. */
inline const uint_t* OpFirst (wchar_t index) {
    return reinterpret_cast<const uint_t*>(index);
}

/** Converts the given value to a pointer. */
inline wchar_t OpFirst (const Op* op) {
    if (!op) {
        return 0;
    }
    return (wchar_t)reinterpret_cast<uintptr_t>(op->in);
}

/** Converts the given value to a pointer. */
inline const uint_t* OpLast (wchar_t index) {
    return reinterpret_cast<const uint_t*> (index);
}

/** Converts the given value to a pointer. */
inline wchar_t OpLast (const Op* op) {
    if (!op) {
        return 0;
    }
    return (wchar_t)reinterpret_cast<uintptr_t>(op->out);
}

#if USING_SCRIPT_TEXT
/** Prints the given Set to the slot.
    Quote: Wikipedia "In mathematics an operand is the object of a mathematical
    op, i.e. it is the quantity that is operated on." */
KABUKI Slot& OpPrint (const Op* op, Slot& slot);
#endif  //< USING_SCRIPT_TEXT

inline const Op* OpError (void* source, Error error) {
    return reinterpret_cast <const Op*> (error);
}

}   //< namespace _

#endif  //< HEADER_FOR_SCRIPT_OP