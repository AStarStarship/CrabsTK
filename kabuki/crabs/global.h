/* Kabuki Toolkit
    @version 0.x
    @file    ~/kabuki-toolkit/kabuki/crabs/global.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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
#ifndef HEADER_FOR_CRABS_GLOBAL
#define HEADER_FOR_CRABS_GLOBAL
// Dependencies:
#include "config.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 1
#include "print_int.h"
#include "random.h"
#endif

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 2
#include "center.h"
#include "clock.h"
#include "hex.h"
#include "memory.h"
#include "printer.h"
#include "right.h"
#include "scan.h"
#include "text.h"
#endif

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 3
#include "array.h"
#include "ascii.h"
#include "bsq.h"
#include "list.h"
#include "map.h"
#include "stack.h"
//#include "multimap.h"
//#include "dictionary.h"
#endif

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 4
#include "address.h"
#include "args.h"
#include "bin.h"
#include "bout.h"
#include "event.h"
#include "expr.h"
#include "interrupts.h"
#include "op.h"
#include "room.h"
#include "slot.h"
#endif

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5
#include "table.h"
#endif
// End dependencies.

#endif  //< HEADER_FOR_CRABS_GLOBAL
