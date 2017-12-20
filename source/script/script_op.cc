/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/script_op.cc
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
    @desc    Slot implementation split into three files, slot.h, 
             monoid_rx.h, and monoid_tx.h because of the large read/write
             functions.
*/

#include <stdafx.h>
#include "op.h"

namespace _ {

#if USE_MORE_ROM
Op OpInit (uintptr_t* buffer, uint_t buffer_size) {
    Bout* bout = BoutInit (buffer, buffer_size);
    Op log;
    log.bout = bout;
    return log;
}

void OpPrint (Op& log) {
    /*
    Bin    * bin = reinterpret_cast<Bin*> (log.bout);
    void   * args[1];
    byte     type = 0,                                             
             ui1;
    uint16_t ui2;
    uint32_t ui4;
    uint64_t ui8;
    //int8_t   reserved,
    int8_t   si1;
    uint16_t si2;
    uint32_t si4;
    uint64_t si8;
    float    flt;
    double   dbl;
    char_t   index;

    //if (BinReadChar (reinterpret_cast<Bin*> (log.bout), index))
    //    return;

    char buffer[DBL_MAX_10_EXP + 2];

    while (index ) {

        switch (type) {
            case STR: {
                if (BinRead (bin, Params<2, ADR, STR> (), Args (args, &ui1,
                    buffer)))
                    return;
                std::cout << buffer;
            }
            case SI1: {
                if (BinRead (bin, Params<2, ADR, SI1> (), Args (args, &si1)))
                    return;
                std::cout << si1;
            }
            case UI1:
            {
                if (BinRead (bin, Params<2, ADR, UI1> (), Args (args, &ui1)))
                    return;
                std::cout << si1;
            }
            case BOL:
            {
                if (BinRead (bin, Params<2, ADR, SI1> (), Args (args, &si1)))
                    return;
                std::cout << si1;
            }
            case SI2:
            {
                if (BinRead (bin, Params<2, ADR, SI2> (), Args (args, &si2)))
                    return;
                std::cout << si1;
            }
            case UI2:
            {
                if (BinRead (bin, Params<2, ADR, UI2> (), Args (args, &ui2,
                    buffer)))
                    return;
                std::cout << si1;
            }

            case SI4:
            {
                if (BinRead (bin, Params<2, ADR, SI4> (), Args (args, &si4,
                    buffer)))
                    return;
                std::cout << si1;
            }

            case UI4:
            {
                if (BinRead (bin, Params<2, ADR, UI4> (), Args (args, &ui4)))
                    return;
                std::cout << si1;
            }

            case SI8:
            {
                if (BinRead (bin, Params<2, ADR, SI8> (), Args (args, &si8)))
                    return;
                std::cout << si8;
            }

            case UI8:
            {
                if (BinRead (bin, Params<2, ADR, UI8> (), Args (args, &ui8)))
                    return;
                std::cout << ui8;
            }
            case TMS:
            {
                if (BinRead (bin, Params<2, ADR, TMS> (), Args (args, &ui4)))
                    return;
                ClockPrintTime (ui4);
            }
            case TMU:
            {
                if (BinRead (bin, Params<2, ADR, TMU> (), Args (args, &ui8)))
                    return;
                ClockPrintTimestamp (ui8);
            }
            case FLT:
            {
                if (BinRead (bin, Params<2, ADR, FLT> (), Args (args, &flt)))
                    return;
                std::cout << si1;
            }
            case DBL:
            {
                if (BinRead (bin, Params<2, ADR, STR> (), Args (args, &dbl)))
                    return;
                std::cout << si1;
            }
        }
    }*/
}   
#endif  //< MEMORY_PROFILE > 2
}       //< namespace _
