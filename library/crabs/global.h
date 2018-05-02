/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/global.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#include "config.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 1
#include "script_itos.h"
#include "random.h"
#endif

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 2
#include "printer.h"
#include "scan.h"
#include "memory.h"
#include "text.h"
#include "clock.h"
#include "right.h"
#include "hex.h"
#include "center.h"
#endif

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
#include "op.h"
#include "address.h"
#include "room.h"
#include "expr.h"
#include "slot.h"
#include "args.h"
#include "bsq.h"
#include "bin.h"
#include "bout.h"
#include "ascii.h"
#endif

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 4
#include "event.h"
#include "interrupts.h"
#endif

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 5
#include "table.h"
#include "lst.h"
#include "map.h"
#include "bok.h"
#include "dic.h"
#endif

#endif  //< HEADER_FOR_CRABS_GLOBAL
