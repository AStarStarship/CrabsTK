/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/f2/seam_tree.h
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
/*
#include "01/ftos_and_stof.h"
#include "02/core_text_functions.h"
#include "03/ascii_strings.h"
#include "04/ascii_clock.h"
#include "05/ascii_stack.h"
#include "06/ascii_array.h"
#include "07/ascii_loom.h"
#include "08/ascii_table.h"
#include "09/ascii_varint.h"
#include "10/ascii_list.h"
#include "11/ascii_map.h"
#include "12/ascii_book.h"
#include "13/ascii_dictionary.h"*/

namespace _ {
static const char* _0_0_0_F2(char* seam_log, char* seam_end, const char* args) {
  return TestTree<_0_0_0__00_ItoS_and_StoI/*, _0_0_0__01_FtoS_and_StoF,
                  _0_0_0__02_Core_text_functions, _0_0_0__03_ASCII_Strings,
                  _0_0_0__04_ASCII_Clock, _0_0_0__05_ASCII_Stack,
                  _0_0_0__00_ASCII_Array, _0_0_0__07_ASCII_Loom,
                  _0_0_0__08_ASCII_Table, _0_0_0__09_ASCII_Varint,
                  _0_0_0__10_ASCII_List, _0_0_0__11_ASCII_Map,
                  _0_0_0__12_ASCII_Book, _0_0_0__13_ASCII_Dictionary*/>(
      seam_log, seam_end, args);
}
}  // namespace _

#if SEAM >= SEAM_0_0_0__00 && SEAM <= SEAM_0_0_0__13
using namespace _;
int main(int arg_count, char** args) {
  enum { kSize = 1024 };
  char seam_log[kSize];
  return SeamTreeTest<_0_0_0_F2>(arg_count, args, seam_log, kSize);
}
#endif
