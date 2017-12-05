/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/log.h
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
#include "args.h"
#include "memory.h"

namespace _ {

/** A general purpose log made from a Bout pointer. */
struct KABUKI Log {
    Bout* bout;
};

/** Initializes the buffer of given size in bytes. */
KABUKI Log LogInit (uintptr_t* buffer, uint_t buffer_size);

/** Returns a reference to the bout for this assembly.
template<uint_t kNumber>
Log& RoomLog () {
    static uintptr_t buffer[kLogSize / sizeof (uintptr_t) + 1];
    static Log log = LogInit (buffer, kLogSize);
    return log;
}

template<uint_t kNumber>
Log& LogInit () {
    static uintptr_t* buffer = WordBuffer<kNumber, kLogSize> ();
    static Log log = LogInit (buffer, kLogSize);
    return log;
} */

/** Prints the given log to the stdout. */
KABUKI void LogPrint (Log& log);

}       //< namespace _

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, int8_t value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::STR> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, uint8_t value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::UI1> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, int16_t value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::SI2> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, uint16_t value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::UI2> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, int32_t value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::SI4> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, uint32_t value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<2, _::UI4> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, int64_t value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::SI8> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, uint64_t value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::UI8> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, float value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::FLT> (), _::Args (args, &value));
    return log;
}

/** Operation << writes the given value to the log. */
inline _::Log& operator<< (_::Log& log, double value) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::DBL> (), _::Args (args, &value));
    return log;
}

/** Operation << prints a char to the log. */
inline _::Log& operator<< (_::Log& log, const char* string) {
    void* args[1];
    _::BoutWrite (log.bout, _::Params<1, _::STR> (), _::Args (args, string));
    return log;
}

#endif  //< SCRIPT_LOG_H
