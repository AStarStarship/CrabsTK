/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_control_matrix.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#if SEAM == KABUKI_FEATURES_TOUCH_1
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "control_matrix.h"

namespace _ {

ControlMatrix::ControlMatrix(SI4 column_count, SI4 num_rows_) {
  num_columns_ = ::_::boundRange(column_count, MinNumColumns, MaxNumColumns);
  num_rows_ = ::_::boundRange(num_rows_, MinNumRows, MaxNumRows);

  rows = new ParameterSet[num_rows_];

  for (SI4 i = 0; i < num_rows_; ++i) rows[i] = ParameterSet(column_count);
}

ControlMatrix::ControlMatrix(const ControlMatrix& o)
    : num_rows_(o.num_rows_), num_columns_(o.num_columns_) {
  rows = new ParameterSet(num_rows_);
}

ControlMatrix::~ControlMatrix() {
  if (rows == nullptr) return;
  delete[] rows;
}

SI4 ControlMatrix::GetNumRows() const { return num_rows_; }

SI4 ControlMatrix::GetNumColumns() const { return num_columns_; }

void ControlMatrix::SetControl(SI4 column, SI4 row, Parameter<SI4>* control) {
  if (row < 0 || column < 0 || row >= num_rows_ || column >= num_columns_)
    return;
  rows[row].setControl(column, control);
}

ParameterSet* ControlMatrix::GetRow(SI4 row) {
  if (row < 0 || row >= num_rows_) return nullptr;

  return &rows[row];
}

void ControlMatrix::Print() const {
  ::_::printLine('-', Parameter<SI4>::MacroHeaderLength);
  Printf("Control Grid: rows = % , columns = %i\n", num_rows_, num_columns_);
  ::_::printLine('-', Parameter<SI4>::MacroHeaderLength);
  printMacroHeader();
  ::_::printNL();
  printLine('-', Parameter<SI4>::MacroHeaderLength);

  Printf("!!!!num_rows_ = %i\n", num_rows_);

  for (SI4 i = 0; i < num_rows_; ++i) {
    Printf("Row %i:\n", i);
    rows[i].Print();
  }

  PrintLine('-', Parameter<SI4>::MacroHeaderLength);
}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
