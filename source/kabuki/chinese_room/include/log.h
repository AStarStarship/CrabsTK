/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/log.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef CHINESE_ROOM_LOG_H
#define CHINESE_ROOM_LOG_H

#include "bout.h"

namespace _ {

/** A general purpose log made from a Bout pointer. */
struct KABUKI Log {
    Bout* bout;
};

template<uint_t kNumber>
Log& Logs () {
    static byte* buffer = Buffer<kNumber, kBoutSize> ();
    static Log s = BoutInit (TxInit (buffer, kBoutSize));
    return s;
}

}       //< namespace _

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, int8_t value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, uint8_t value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, int16_t value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, uint16_t value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, int32_t value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, uint32_t value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, int64_t value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, uint64_t value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, float value);

/** Operation + writes the given value to the log. */
inline _::Log& operator+ (_::Log& log, double value);

/** Operation + prints a string to the log. */
inline _::Log& operator+ (_::Log& log, const char * s);

#endif  //< CHINESE_ROOM_LOG_H
