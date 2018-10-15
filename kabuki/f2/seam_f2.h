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
#include "13/ascii_dictionary.h"

namespace _ {

static const char* Kabuki__Terminal___Kabuki__F2(char* seam_log, char* log_end, const char* args) {
#if SEAM >= SEAM_0_0_0
  if (!TestBegin(seam_log, log_end, args)) return __FUNCTION__;
#endif
  return TestTree<
    Kabuki__F2___ItoS_and_StoI,
    Kabuki__F2___FtoS_and_StoF,
    Kabuki__F2___Core_text_functions,
    Kabuki__F2___ASCII_Clock,
    Kabuki__F2___ASCII_Stack,
    Kabuki__F2___ASCII_Array,
    Kabuki__F2___ASCII_Loom,
    Kabuki__F2___ASCII_Table,
    Kabuki__F2___ASCII_Varint,
    Kabuki__F2___ASCII_List,
    Kabuki__F2___ASCII_Map,
    Kabuki__F2___ASCII_Book,
    Kabuki__F2___ASCII_Dictionary>(seam_log, log_end, args);
}

}  // namespace _
