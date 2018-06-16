/* Kabuki Toolkit
    @file    ~/source/hmi/hmi_control_matrix.h
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

#pragma once
#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_HMI_CONTROLMATRIX
#define HEADER_FOR_KT_HMI_CONTROLMATRIX

#include "parameter_set.h"

namespace _ {

/* A grid of Controls.
    Each Parameter<int> Layer on the Symmetry Live can either have 8 pots or
   encoders, or can have 4 pots and 4 encoders. This class KABUKI helps us swap
   the values out quickly and efficiently.

    A ControlMatrix must have the option of being bifurcated. */
class KABUKI ControlMatrix {
 public:
  enum {
    kMinNumRows = 1,                             //< The min number of rows.
    kMinNumColumns = 2,                          //< The min number of columns.
    kMaxNumRows = ParameterSet::MaxNumElements,  //< The max number of rows.
    kMaxNumColumns =
        ParameterSet::MaxNumElements,  //< The max number of columns.
  };

  /* Default constructor. */
  ControlMatrix(int newNumColumns = 8, int newNumRows = 6);

  /* Copy constructor. */
  ControlMatrix(const ControlMatrix& o);

  /* Virtual Destructor. */
  virtual ~ControlMatrix();

  /* Gets the number of rows. */
  int GetNumRows() const;

  /* Gets the number of columns. */
  int GetNumColumns() const;

  /* Sets the pointer to knobs[index] pointer to the newKnob. */
  void SetParameter(int column, int row, Parameter<int>* control);

  /* Gets the row at the given index.
      @return gets nullptr if the index is invalid. */
  ParameterSet* GetRow(int row);

  /* Prints this object to a string. */
  _::Printer& Print(_::Printer& print) const;

 private:
  int num_rows_,        //< Number of rows.
      num_columns_;     //< Number of columns.
  ParameterSet* rows_;  //< Rows of control array columns.
};                      //< class ControlMatrix
}       //< namespace _
#endif  //< HEADER_FOR_KT_HMI_CONTROLMATRIX
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
