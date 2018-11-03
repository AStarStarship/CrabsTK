/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/0_0_0_f2.h
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
#include <pch.h>
#if SEAM >= _0_0_0__00
#include "0_0_0__00_rng.h"
#endif
#if SEAM >= _0_0_0__01
#include "0_0_0__01_itos_and_stoi.h"
#endif
#if SEAM >= _0_0_0__02
#include "0_0_0__02_ascii_strings_and_socket.h"
#endif
#if SEAM >= _0_0_0__03
#include "0_0_0__03_ftos_and_stof.h"
#endif
#if SEAM >= _0_0_0__04
#include "0_0_0__04_clock.h"
#endif
#if SEAM >= _0_0_0__05
#include "0_0_0__05_ascii_stack.h"
#endif
#if SEAM >= _0_0_0__06
#include "0_0_0__06_ascii_array.h"
#endif
#if SEAM >= _0_0_0__07
#include "0_0_0__07_ascii_loom.h"
#endif
#if SEAM >= _0_0_0__08
#include "0_0_0__08_ascii_table.h"
#endif
#if SEAM >= _0_0_0__09
#include "0_0_0__09_ascii_list.h"
#endif
#if SEAM >= _0_0_0__10
#include "0_0_0__10_ascii_map.h"
#endif
#if SEAM >= _0_0_0__11
#include "0_0_0__11_ascii_book.h"
#endif
#if SEAM >= _0_0_0__12
#include "0_0_0__12_ascii_dictionary.h"
#endif

namespace _ {
static const char* _0_0_0_F2(char* seam_log, char* seam_end, const char* args) {
  return TestTree<_0_0_0__00_RNG

#if SEAM >= _0_0_0__01
                  ,
                  _0_0_0__01_ItoS_and_StoI
#endif
#if SEAM >= _0_0_0__02
                  ,
                  _0_0_0__02_ASCII_Strings_and_Socket
#endif
#if SEAM >= _0_0_0__03
                  ,
                  _0_0_0__03_FtoS_and_StoF
#endif
#if SEAM >= _0_0_0__04
                  ,
                  _0_0_0__04_Clock
#endif
#if SEAM >= _0_0_0__05
                  ,
                  _0_0_0__05_ASCII_Stack
#endif
#if SEAM >= _0_0_0__06
                  ,
                  _0_0_0__06_ASCII_Array
#endif
#if SEAM >= _0_0_0__07
                  ,
                  _0_0_0__07_ASCII_Loom
#endif
#if SEAM >= _0_0_0__08
                  ,
                  _0_0_0__08_ASCII_Table
#endif
#if SEAM >= _0_0_0__09
                  ,
                  _0_0_0__09_ASCII_List
#endif
#if SEAM >= _0_0_0__10
                  ,
                  _0_0_0__10_ASCII_Map
#endif
#if SEAM >= _0_0_0__11
                  ,
                  _0_0_0__11_ASCII_Book
#endif
#if SEAM >= _0_0_0__12
                  ,
                  _0_0_0__13_ASCII_Dictionary
#endif
                  >(seam_log, seam_end, args);
}
}  // namespace _
