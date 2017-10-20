/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/log.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough<calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_LOG_H
#define CHINESE_ROOM_LOG_H

#include "monoid.h"

namespace _ {

/** A socket to write log info to. */
struct Log {
    Bout* a;         //< The a socket.
};

/** Initializes the socket with the given tx socket.
    @param tx The a socket.
    @param is_dynamic Flag for if we're using dynamic memory or not. */
inline Log LogInit (Bout* tx, bool is_dynamic = false);

/** Prints a message with the given header to the given Tx slot. */
inline ticket_t Write (Bout* tx, byte op, const char* string);

/** Prints a message with the given header to the given Tx slot. */
inline ticket_t Write (Log& log, byte op, const char* string);

/** Returns a reference to the log for this assembly. */
template<uint_t kNumber>
inline Log& Logbag () {
    static byte* buffer = Buffer<kNumber, kLogSize> ();
    static Log s = LogInit (TxInit (buffer, kLogSize));
    return s;
}

/** Operation << prints the given value as a string to the log. */
inline Log& operator<< (Log& log, int8_t value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, uint8_t value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, int16_t value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, uint16_t value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, int32_t value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, uint32_t value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, int64_t value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, uint64_t value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, float value);

/** Operation << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, double value);

/** Operation << prints a string to the socket. */
inline Log& operator<< (Log& log, const char* s);

}       //< namespace _
#endif  //< CHINESE_ROOM_LOG_H
