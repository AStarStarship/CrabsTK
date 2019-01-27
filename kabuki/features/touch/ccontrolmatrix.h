/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_control_matrix.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#ifndef HEADER_FOR_KT_HMI_CONTROLMATRIX
#define HEADER_FOR_KT_HMI_CONTROLMATRIX

#include "parameter_set.h"

namespace _ {

/* A grid of Controls.
    Each Parameter<SI4> Layer on the Symmetry Live can either have 8 pots or
   encoders, or can have 4 pots and 4 encoders. This class SDK helps us swap
   the values out quickly and efficiently.

    A ControlMatrix must have the option of being bifurcated. */
class SDK ControlMatrix {
 public:
  enum {
    kMinNumRows = 1,                             //< The min number of rows.
    kMinNumColumns = 2,                          //< The min number of columns.
    kMaxNumRows = ParameterSet::MaxNumElements,  //< The max number of rows.
    kMaxNumColumns =
        ParameterSet::MaxNumElements,  //< The max number of columns.
  };

  /* Default constructor. */
  ControlMatrix(SI4 newNumColumns = 8, SI4 newNumRows = 6);

  /* Copy constructor. */
  ControlMatrix(const ControlMatrix& o);

  /* Virtual Destructor. */
  virtual ~ControlMatrix();

  /* Gets the number of rows. */
  SI4 GetNumRows() const;

  /* Gets the number of columns. */
  SI4 GetNumColumns() const;

  /* Sets the pointer to knobs[index] pointer to the newKnob. */
  void SetParameter(SI4 column, SI4 row, Parameter<SI4>* control);

  /* Gets the row at the given index.
      @return gets nullptr if the index is invalid. */
  ParameterSet* GetRow(SI4 row);

  /* Prints this object to a string. */
  ::_::Utf& Print(_::Utf& print) const;

 private:
  SI4 num_rows_,        //< Number of rows.
      num_columns_;     //< Number of columns.
  ParameterSet* rows_;  //< Rows of control array columns.
};                      //< class ControlMatrix
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_CONTROLMATRIX
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
