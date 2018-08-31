/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/crabs_op.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#if SEAM >= SEAM_0_0_4
// Dependencies:
#include "bsq.h"
#include "debug.h"
#include "op.h"
#include "slot.h"
#include "str1.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 4
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#else
#define PRINTF(x, ...)
#define PRINT(c)
#endif

namespace _ {

#if CRABS_TEXT
Utf8& Print(Utf8& print, const Op* op) {
  if (!op) {
    return print << "\nOp: nil";
  }
  return print << "\nOp      :" << op->name << "\nBSQ     :" << Bsq(op->in)
               << "\nResult: :" << Bsq(op->out)
               << "\nMetadata:" << op->description;
}
#endif
/*
#if CRABS_TEXT
Op OpInit (uintptr_t* buffer, uint_t buffer_size) {
    BOut* bout = BOutInit (buffer, buffer_size);
    Op log;
    log.bout = bout;
    return log;
}

void Print (Op& log) {
    BIn    * bin = reinterpret_cast<BIn*> (log.bout);
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
    char   index;
    //if (BinReadChar (reinterpret_cast<BIn*> (log.bout), index))
    //    return;
    char buffer[DBL_MAX_10_EXP + 2];
    while (index ) {
        switch (type) {
            case STR: {
                if (BinRead (bin, Params<2, ADR, STR> (), Args (args, &ui1,
                             buffer)))
                    return;
                Write (buffer);
            }
            case SI1: {
                if (BinRead (bin, Params<2, ADR, SI1> (), Args (args, &si1)))
                    return;
                Write (si1);
            }
            case UI1: {
                if (BinRead (bin, Params<2, ADR, UI1> (), Args (args, &ui1)))
                    return;
                Write (si1);
            }
            case BOL: {
                if (BinRead (bin, Params<2, ADR, SI1> (), Args (args, &si1)))
                    return;
                Write (si1);
            }
            case SI2: {
                if (BinRead (bin, Params<2, ADR, SI2> (), Args (args, &si2)))
                    return;
                Write (si1);
            }
            case UI2: {
                if (BinRead (bin, Params<2, ADR, UI2> (),
                             Args (args, &ui2, buffer)))
                    return;
                Write (si1);
            }
            case SI4: {
                if (BinRead (bin, Params<2, ADR, SI4> (),
                             Args (args, &si4, buffer)))
                    return;
                Write (si1;
            }
            case UI4: {
                if (BinRead (bin, Params<2, ADR, UI4> (), Args (args, &ui4)))
                    return;
                Write (si1;
            }
            case SI8: {
                if (BinRead (bin, Params<2, ADR, SI8> (), Args (args, &si8)))
                    return;
                Write (si8;
            }
            case UI8: {
                if (BinRead (bin, Params<2, ADR, UI8> (), Args (args, &ui8)))
                 return;
                Write (ui8;
            }
            case TMS: {
                if (BinRead (bin, Params<2, ADR, TMS> (), Args (args, &ui4)))
                    return;
                ClockPrintTime (ui4);
            }
            case TME: {
                if (BinRead (bin, Params<2, ADR, TME> (), Args (args, &ui8)))
                    return;
                ClockPrintTimestamp (ui8);
            }
            case FLT: {
                if (BinRead (bin, Params<2, ADR, FLT> (), Args (args, &flt)))
                    return;
                Write (si1;
            }
            case DBL: {
                if (BinRead (bin, Params<2, ADR, STR> (), Args (args, &dbl)))
                    return;
                Write (si1;
            }
        }
    }
}
#endif */

}  // namespace _

#undef PRINTF
#undef PRINT
#endif  //> #if SEAM >= SEAM_0_0_4
