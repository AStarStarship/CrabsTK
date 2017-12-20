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

#ifndef SCRIPT_LOG_H
#define SCRIPT_LOG_H

#include "bout.h"
#include "params.h"
#include "args.h"

namespace _ {

/** A general purpose op made from a Bout pointer. */
struct KABUKI Op {
    const char* address;
    Bout* bout;
};

/** Initializes the buffer of given size in bytes. */
KABUKI Op OpInit (uintptr_t* buffer, uint_t buffer_size);

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
} */

/** Prints the given op to the stdout. */
KABUKI void OpPrint (Op& op);

}       //< namespace _

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, int8_t value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::STR> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, uint8_t value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::UI1> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, int16_t value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::SI2> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, uint16_t value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::UI2> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, int32_t value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::SI4> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, uint32_t value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::UI4> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, int64_t value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::SI8> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, uint64_t value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::UI8> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, float value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::FLT> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << writes the given value to the op. */
inline _::Op& operator<< (_::Op& op, double value) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::DBL> (),
                  _::Args (args, &value));
    return op;
}

/** Operation << prints a char to the op. */
inline _::Op& operator<< (_::Op& op, const char* string) {
    void* args[1];
    _::BoutWrite (op.bout,
                  _::Params<2, _::STR, _::kMaxAddressLength, _::STR> (),
                  _::Args (args, string));
    return op;
}

#endif  //< SCRIPT_LOG_H
