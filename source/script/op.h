/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/op.h
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

#ifndef HEADER_FOR_SCRIPT_OP
#define HEADER_FOR_SCRIPT_OP

#include "strand.h"
#include "error.h"

namespace _ {

enum {
    kOpPush          = 0, //< Operation Type 0: Stack push .
    kOpOperation     = 1, //< Operation Type 1: Abstract Operation.
    KOpOperationPush = 2, //< Operation Type 2: Operation with stack push.
};

/** An expression Operation with name key, multiple input params,
    result, and optional description of a data set.
    @code
    static const Op kThis = { "Key",
        Params<1, 2>::Header, Params<1, 2>::Header,
        "Description", 0, 0 };

    static const Op member_device =   { "Key", 
        NumOps (0), FirstOp ('A'),
        "Description", 1, 0 };
    @endcode */
struct KABUKI Op {
    const char   * name;        //< Op name.
    const uint_t * params,      //< Op parameters or Operand data B-Seq.
                 * result;      //< Op result B-Seq.
    const char   * description; //< Op description.
    int            type;        //< Type of Op: 0 for Operand.
    const void   * evaluation;  //< Evaluated expression.
};

/** Compares the given Op's name to the given key. */
inline bool OpEquals (const Op* op, const char* key) {
    if (!op) {
        return false;
    }
    return StrandEquals (op->name, key);
}

/** Compares the given Op's name to the given key. */
inline bool OpEquals (const Op* op, const char* begin,
                             const char* end) {
    if (!op) {
        return false;
    }
    return StrandEquals (begin, end, op->name);
}

/** Converts the given value to a pointer. */
inline intptr_t OpCount (const Op& op) {
    return op.result - op.params;
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
    return (wchar_t)reinterpret_cast<uintptr_t>(op->params);
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
    return (wchar_t)reinterpret_cast<uintptr_t>(op->result);
}

#if USING_SCRIPT_TEXT
/** Prints the given Set to the strand.
    Quote: Wikipedia "In mathematics an operand is the object of a mathematical
    op, i.e. it is the quantity that is operated on." */
KABUKI Strand& OpPrint (const Op* op, Strand& strand);
#endif  //< USING_SCRIPT_TEXT

/** A general purpose op made from a BOut pointer. 
struct KABUKI Macro {
    const char* address;
    BOut* bout;
};*/

/** Initializes the buffer of given size in bytes. 
KABUKI Macro MacroInit (uintptr_t* buffer, uint_t buffer_size);*/

/** Returns a reference to the bout for this assembly. 
template<uint_t kNumber>
Log& RoomLog () {
    static uintptr_t buffer[kLogSize / sizeof (uintptr_t) + 1];
    static Log op = LogInit (buffer, kLogSize);
    return op;
}

template<uint_t kNumber>
Log& LogInit () {
    static uintptr_t* buffer = WordBuffer<kNumber, kLogSize> ();
    static Log op = LogInit (buffer, kLogSize);
    return op;
}*/

inline const Op* OpError (void* source, Error error) {
    return reinterpret_cast <const Op*> (error);
}

}   //< namespace _

/** Overloaded operator<< prints the given op to the text. */
KABUKI _::Strand& operator<< (_::Strand& strand, const _::Op* op);

/** Macro << writes the given value to the op. 
inline _::Macro& operator<< (_::Macro& op, int8_t value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::STR> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op. 
inline _::Macro& operator<< (_::Macro& op, uint8_t value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::UI1> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op.
inline _::Macro& operator<< (_::Macro& op, int16_t value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::SI2> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op.
inline _::Macro& operator<< (_::Macro& op, uint16_t value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::UI2> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op.
inline _::Macro& operator<< (_::Macro& op, int32_t value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::SI4> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op.
inline _::Macro& operator<< (_::Macro& op, uint32_t value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::UI4> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op.
inline _::Macro& operator<< (_::Macro& op, int64_t value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::SI8> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op.
inline _::Macro& operator<< (_::Macro& op, uint64_t value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::UI8> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op.
inline _::Macro& operator<< (_::Macro& op, float value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::FLT> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << writes the given value to the op.
inline _::Macro& operator<< (_::Macro& op, double value) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::DBL> (),
                  _::Args (args, &value));
    return op;
} */

/** Macro << prints a char to the op.
inline _::Macro& operator<< (_::Macro& op, const char* string) {
    void* args[1];
    _::BOutWrite (op.bout,
                  _::Params<2, _::STR, _::kAddressLengthMax, _::STR> (),
                  _::Args (args, string));
    return op;
} */

#endif  //< HEADER_FOR_SCRIPT_OP
