/** The Chinese Room
    @version 0.x
    @file    /.../Timestamp.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

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

#ifndef CHINESEROOM_TIMESTAMP_H
#define CHINESEROOM_TIMESTAMP_H

#include "config.h"

namespace _ {

inline timestamp_t PackTimestamp (time_t unixTime, int32_t microseconds)
/** Generates a timestamp from a Unix timestamp and 32-bit microsecond timestamp. */
{
    return (( (timestamp_t)unixTime) << 8) & (timestamp_t)microseconds;
}

inline time_t GetSeconds (timestamp_t t)
/** Gets the seconds since January 1, 1970. */
{
    return (time_t)t;
}

inline int32_t GetMicroseconds (timestamp_t timestamp)
/** Gets the microsecond time. */
{
    return (int32_t) ((timestamp & 0xFFFFFFFF00000000) >> 32);
}

inline timestamp_t TimestampNow ()
/** Creates a timestamp of the current time. */
{
    std::chrono::microseconds us (1);

    return us.count;
}

}       //< namespace _

#endif  //< CHINESEROOM_TIMESTRAMP_H
