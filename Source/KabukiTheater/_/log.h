/** The Chinese Room
    @version 0.x
    @file    /.../log.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough<calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_LOG_H
#define CHINESEROOM_LOG_H

#include "tx.h"

namespace _ {

/** A socket to write log info to. */
struct Log {
    Tx* socket;         //< The io socket stored as Tx pointer.
};

/** Initializes the socket with the given tx socket.
@param tx The io socket.
@param is_dynamic Flag for if we're using dynamic memory or not.
*/
static Log LogInit (Tx* tx, bool is_dynamic = false) {
    Log log;
    log.socket = tx;
    return log;
}

/** Prints a message with the given header to the given Tx slot. */
static ticket_t Write (Tx* tx, byte member, const char* string) {
#if DEBUG_CHINESEROOM
    printf ("| Writting string to %p\n", tx);
    Print (tx);
#endif

    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte c;

    uint_t size = tx->size,
        space;

    // Convert the socket offsets to pointers.
    byte* begin = SocketBaseAddress (tx),
        *end = begin + size,
        *start = begin + tx->start,
        *stop = begin + tx->stop;
    space = RingBufferSpace (start, stop, size);
    //  * temp_ptr;
    const byte* str_ptr;

    *stop = member;
    ++stop;
    // We know we will always write at least one null-term byte.
    c = *str_ptr;
    *stop = c;
    ++stop;

    while (c != 0) {
        if (space-- == 0)
            return Report (BufferUnderflowError, Esc<1, STX> (),
                                0, start);

        ++str_ptr;
        c = *str_ptr;     // Read byte.

        *stop = c;        // Write byte
        ++stop;

        if (stop >= end) stop -= size;
    }
    *stop = c;        // Write byte
    ++stop;
    tx->stop = Diff (stop, begin);
#if DEBUG_CHINESEROOM
    printf ("| After...\n");
    Print (tx);
#endif
    return 0;
}

/** Prints a message with the given header to the given Tx slot. */
static ticket_t Write (Log& log, byte member, const char* string) {
    return Write (log.socket, member, string);
}

/** Returns a reference to the log for this assembly. */
template<uint_t kNumber>
static Log& Logbook () {
    static byte* buffer = Buffer<kNumber, kLogSize> (); //static char buffer[kLogSize];
    static Log s = LogInit (TxInit (buffer, kLogSize));
    return s;
}

/** Operator << prints the given value as a string to the log. */
inline Log& operator<< (Log& log, int8_t value) {
    char buffer[5];
    sprintf_s (buffer, 5, "%i", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, uint8_t value) {
    char buffer[4];
    sprintf_s (buffer, 4, "%u", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, int16_t value) {
    char buffer[7];
    sprintf_s (buffer, 7, "%i", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, uint16_t value) {
    char buffer[6];
    sprintf_s (buffer, 6, "%u", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, int32_t value) {
    char buffer[12];
    sprintf_s (buffer, 12, "%i", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, uint32_t value) {
    char buffer[11];
    sprintf_s (buffer, 11, "%u", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, int64_t value) {
    char buffer[22];
    sprintf_s (buffer, 22, "%I64d", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, uint64_t value) {
    char buffer[21];
    sprintf_s (buffer, 21, "%llu", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, float value) {
    char buffer[FLT_MAX_10_EXP + 2];
    sprintf_s (buffer, FLT_MAX_10_EXP + 2, "%f", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints the given value as a string to the socket. */
inline Log& operator<< (Log& log, double value) {
    char buffer[DBL_MAX_10_EXP + 2];
    sprintf_s (buffer, DBL_MAX_10_EXP + 2, "%f", value);
    Write (log, '?', buffer);
    return log;
}

/** Operator << prints a string to the socket. */
inline Log& operator<< (Log& log, const char* s) {
    Write (log, '?', s);
    return log;
}

}       //< namespace _
#endif  //< CHINESEROOM_LOG_H
