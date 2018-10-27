/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/seam_0_0_0_f2.h
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

#include "00/itos_and_stoi.h"

#if SEAM >= SEAM_0_0_0__01
#include "01/ascii_strings_and_socket.h"
#endif
#if SEAM >= SEAM_0_0_0__02
#include "02/ftos_and_stof.h"
#endif
#if SEAM >= SEAM_0_0_0__03
#include "04/ascii_clock.h"
#endif
#if SEAM >= SEAM_0_0_0__04
#include "05/ascii_stack.h"
#endif
#if SEAM >= SEAM_0_0_0__05
#include "06/ascii_array.h"
#endif
#if SEAM >= SEAM_0_0_0__06
#include "07/ascii_loom.h"
#endif
#if SEAM >= SEAM_0_0_0__07
#include "08/ascii_table.h"
#endif
#if SEAM >= SEAM_0_0_0__08
#include "09/ascii_varint.h"
#endif
#if SEAM >= SEAM_0_0_0__09
#include "10/ascii_list.h"
#endif
#if SEAM >= SEAM_0_0_0__10
#include "11/ascii_map.h"
#endif
#if SEAM >= SEAM_0_0_0__11
#include "12/ascii_book.h"
#endif
#if SEAM >= SEAM_0_0_0__12
#include "13/ascii_dictionary.h"
#endif

namespace _ {
static const char* _0_0_0_F2(char* seam_log, char* seam_end, const char* args) {
  return TestTree<_0_0_0__00_ItoS_and_StoI
#if SEAM >= SEAM_0_0_0__01
                  ,
                  _0_0_0__01_ASCII_Strings_and_Socket
#endif
#if SEAM >= SEAM_0_0_0__02
                  ,
                  _0_0_0__02_ASCII_String_and_Socket
#endif
#if SEAM >= SEAM_0_0_0__03
                  ,
                  _0_0_0__01_FtoS_and_StoF
#endif
#if SEAM >= SEAM_0_0_0__04
                  ,
                  _0_0_0__04_ASCII_Clock
#endif
#if SEAM >= SEAM_0_0_0__05
                  ,
                  _0_0_0__05_ASCII_Stack
#endif
#if SEAM >= SEAM_0_0_0__06
                  ,
                  _0_0_0__00_ASCII_Array
#endif
#if SEAM >= SEAM_0_0_0__07
                  ,
                  _0_0_0__07_ASCII_Loom
#endif
#if SEAM >= SEAM_0_0_0__08
                  ,
                  _0_0_0__08_ASCII_Table
#endif
#if SEAM >= SEAM_0_0_0__09
                  ,
                  _0_0_0__10_ASCII_List
#endif
#if SEAM >= SEAM_0_0_0__10
                  ,
                  _0_0_0__11_ASCII_Map
#endif
#if SEAM >= SEAM_0_0_0__11
                  ,
                  _0_0_0__12_ASCII_Book
#endif
#if SEAM >= SEAM_0_0_0__12
                  ,
                  _0_0_0__13_ASCII_Dictionary
#endif
                  >(seam_log, seam_end, args);
}
}  // namespace _
