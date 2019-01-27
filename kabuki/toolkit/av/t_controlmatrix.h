/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_control_matrix.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_CONTROLMATRIX
#define KABUKI_TOOLKIT_AV_CONTROLMATRIX

#include "t_paramset.h"

namespace _ {

/* A grid of Controls.
    Each Parameter<SI4> Layer on the Symmetry Live can either have 8 pots or
   encoders, or can have 4 pots and 4 encoders. This class SDK helps us swap
   the values out quickly and efficiently.

    A TControlMatrix must have the option of being bifurcated. */
class SDK TControlMatrix {
 public:
  enum {
    kMinNumRows = 1,                             //< The min number of rows.
    kMinNumColumns = 2,                          //< The min number of columns.
    kMaxNumRows = ParamSet::kElementCountMax,  //< The max number of rows.
    kMaxNumColumns =
        ParamSet::kElementCountMax,  //< The max number of columns.
  };

  /* Default constructor. */
  TControlMatrix(SI4 column_count = 8, SI4 num_rows = 6) {
    col_count_ = BoundRange (column_count, kMinNumColumns, kMaxNumColumns);
    num_rows = BoundRange (num_rows, kMinNumRows, kMaxNumRows);

    rows = new ParamSet[num_rows];

    for (SI4 i = 0; i < num_rows; ++i) rows[i] = ParamSet (column_count);
  }

  /* Copy constructor. */
  TControlMatrix(const TControlMatrix& o)
    : row_count_ (o.row_count_), col_count_ (o.col_count_) {
    rows = new ParamSet (row_count_);
  }

  /* Virtual Destructor. */
  virtual ~TControlMatrix () {}

  /* Gets the number of rows. */
  SI4 GetNumRows() const { return row_count_; }

  /* Gets the number of columns. */
  SI4 GetNumColumns() const { return col_count_; }

  /* Sets the pointer to knobs[index] pointer to the newKnob. */
  void SetParameter(SI4 column, SI4 row, Parameter<SI4>* control) {
    if (row < 0 || column < 0 || row >= row_count_ || column >= col_count_)
      return;
    rows[row].SetControl (column, control);
  }

  /* Gets the row at the given index.
  @return gets nullptr if the index is invalid. */
  ParamSet* GetRow(SI4 row) {
    if (row < 0 || row >= row_count_) return nullptr;
    return rows[row];
  }

  /* Prints this object to a string. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o << StringLine ('-', Parameter<SI4>::kMacroHeaderLength)
      << "Control Grid: rows:" << row_count_
      << " columns:" << col_count_
      << StringLine('-', Parameter<SI4>::kMacroHeaderLength);
    PrintMacroHeader (o);
    o << StringLine ('-', Parameter<SI4>::kMacroHeaderLength)
      << "\n!!!!row_count_" << row_count_;

    for (SI4 i = 0; i < row_count_; ++i) {
      o << "Row:" << i;
      rows[i].Print (o);
    }

    return o << StringLine ('-', Parameter<SI4>::kMacroHeaderLength);
  }

 private:
  SI4 row_count_,       //< Number of rows.
      col_count_;       //< Number of columns.
  ParamSet* rows_;  //< Rows of control array columns.
};
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_CONTROLMATRIX
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
