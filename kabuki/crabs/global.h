/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/crabs/global.h
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

#include "../f2/global.h"

#if SEAM >= SEAM_00_00_00__00_00
#if SEAM == SEAM_00_00_00__00_00
#else
#include "binary_32.h"
#include "binary_64.h"
#include "random.h"
#endif
#endif

#if SEAM >= SEAM_00_00_00__00_01
#if SEAM == SEAM_00_00_00__00_01
#include "test.h"
#else
#include "binary_64.h"
#include "clock.h"
#include "hex.h"
#include "socket.h"
#include "str.h"
#endif
#endif

#if SEAM >= SEAM_00_00_00__00_02
#if SEAM == SEAM_00_00_00__00_02
#include "array.h"
#include "ascii.h"
#include "bsq.h"
#include "list.h"
#include "map.h"
#include "stack.h"
#include "test.h"
//#include "multimap.h"
//#include "dictionary.h"
#endif
#endif

#if SEAM >= SEAM_00_00_00__00_03
#if SEAM == SEAM_00_00_00__00_03
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
#endif

#if SEAM >= SEAM_00_00_00__00_04
#if SEAM == SEAM_00_00_00__00_04
#include "library.h"
#endif
#endif
// End dependencies.

#endif  //< INCLUDED_CRABS_GLOBAL
