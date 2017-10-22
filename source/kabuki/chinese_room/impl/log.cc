/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/impl/log.cc
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
    @desc    Slot implementation split into three files, slot.h, 
             monoid_rx.h, and monoid_tx.h because of the large read/write
             functions.
*/

#include "../include/log.h"
#include "../include/address.h"
#include "../include/args.h"

using namespace _;

Log& operator+ (Log& log, int8_t value) {
    void* args[2];
    char buffer[5];
    sprintf_s (buffer, 5, "%i", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, uint8_t value) {
    void* args[2];
    char buffer[4];
    sprintf_s (buffer, 4, "%u", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, int16_t value) {
    void* args[2];
    char buffer[7];
    sprintf_s (buffer, 7, "%i", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, uint16_t value) {
    void* args[2];
    char buffer[6];
    sprintf_s (buffer, 6, "%u", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, int32_t value) {
    void* args[2];
    char buffer[12];
    sprintf_s (buffer, 12, "%i", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, uint32_t value) {
    void* args[2];
    char buffer[11];
    sprintf_s (buffer, 11, "%u", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, int64_t value) {
    void* args[2];
    char buffer[22];
    sprintf_s (buffer, 22, "%I64d", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, uint64_t value) {
    void* args[2];
    char buffer[21];
    sprintf_s (buffer, 21, "%llu", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, 
                                                           Address<'?'> (),
                                                           buffer));
    return log;
}

Log& operator+ (Log& log, float value) {
    void* args[2];
    char buffer[FLT_MAX_10_EXP + 2];
    sprintf_s (buffer, FLT_MAX_10_EXP + 2, "%f", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, double value) {
    void* args[2];
    char buffer[DBL_MAX_10_EXP + 2];
    sprintf_s (buffer, DBL_MAX_10_EXP + 2, "%f", value);
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   buffer));
    return log;
}

Log& operator+ (Log& log, const char* string) {
    void* args[2];
    BoutWrite (log.bout, Params<2, SOH, STX> (), Args (args, Address<'?'> (),
                                                   string));
    return log;
}
