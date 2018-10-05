/* Kabuki Toolkit
@file    ~/source/hmi/hmi_control_matrix.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#include <pch.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
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

ControlMatrix::ControlMatrix(int column_count, int num_rows_) {
  num_columns_ = _::boundRange(column_count, MinNumColumns, MaxNumColumns);
  num_rows_ = _::boundRange(num_rows_, MinNumRows, MaxNumRows);

  rows = new ParameterSet[num_rows_];

  for (int i = 0; i < num_rows_; ++i) rows[i] = ParameterSet(column_count);
}

ControlMatrix::ControlMatrix(const ControlMatrix& o)
    : num_rows_(o.num_rows_), num_columns_(o.num_columns_) {
  rows = new ParameterSet(num_rows_);
}

ControlMatrix::~ControlMatrix() {
  if (rows == nullptr) return;
  delete[] rows;
}

int ControlMatrix::GetNumRows() const { return num_rows_; }

int ControlMatrix::GetNumColumns() const { return num_columns_; }

void ControlMatrix::SetControl(int column, int row, Parameter<int>* control) {
  if (row < 0 || column < 0 || row >= num_rows_ || column >= num_columns_)
    return;
  rows[row].setControl(column, control);
}

ParameterSet* ControlMatrix::GetRow(int row) {
  if (row < 0 || row >= num_rows_) return nullptr;

  return &rows[row];
}

void ControlMatrix::Print() const {
  _::printLine('-', Parameter<int>::MacroHeaderLength);
  Printf("Control Grid: rows = % , columns = %i\n", num_rows_, num_columns_);
  _::printLine('-', Parameter<int>::MacroHeaderLength);
  printMacroHeader();
  _::printNL();
  printLine('-', Parameter<int>::MacroHeaderLength);

  Printf("!!!!num_rows_ = %i\n", num_rows_);

  for (int i = 0; i < num_rows_; ++i) {
    Printf("Row %i:\n", i);
    rows[i].Print();
  }

  PrintLine('-', Parameter<int>::MacroHeaderLength);
}

}       //< namespace _
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
