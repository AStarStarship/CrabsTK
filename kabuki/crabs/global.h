/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/global.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>
#ifndef INCLUDED_CRABS_GLOBAL
#define INCLUDED_CRABS_GLOBAL
// Dependencies:
#include "config.h"

#if SEAM >= SEAM_0_0_0
#include "random.h"
#include "tint.h"
#endif

#if SEAM >= SEAM_0_0_0
#include "decimal.h"
#include "tfp.h"
#endif

#if SEAM >= SEAM_0_0_2
#include "clock.h"
#include "debug.h"
#include "hex.h"
#include "socket.h"
#include "str.h"
#endif

#if SEAM >= SEAM_0_0_3
#include "array.h"
#include "ascii.h"
#include "bsq.h"
#include "list.h"
#include "map.h"
#include "stack.h"
//#include "multimap.h"
//#include "dictionary.h"
#endif

#if SEAM >= SEAM_0_0_4
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

#if SEAM >= SEAM_0_0_5
#include "table.h"
#endif
// End dependencies.

#endif  //< INCLUDED_CRABS_GLOBAL
