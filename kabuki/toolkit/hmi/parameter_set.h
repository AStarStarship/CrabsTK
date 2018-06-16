/* Kabuki Toolkit
    @file    ~/source/hmi/hmi_parameter_set.h
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
#ifndef HEADER_FOR_KT_HMI_PARAMETERSET
#define HEADER_FOR_KT_HMI_PARAMETERSET

#include "parameter.h"

namespace _ {

/* A set of Parameter<T> that can be addressed by index.
 */
template <typename T>
class KABUKI ParameterSet {
 public:
  enum {
    MinNumElements = 2,    //< The minimum number of columns.
    MaxNumElements = 256,  //< The minimum number of columns.
  };

  /* Default constructor. */
  ParameterSet(size_type newNumControls = MinNumElements);

  /* Constructor creates a row with pointers to the newControls. */
  ParameterSet(const _::Array<Parameter<T>*>& controls);

  //< Copy constrcutor.
  ParameterSet(const ParameterSet& o);

  //< Destructor.
  ~ParameterSet();

  //< Adds a new control to the array.
  void addParam(Parameter<T>& c);

  /* Adds a new control to the array.
      @return Returns 0 upon success, and index if it was out of bounds. */
  size_type setParam(size_type index, Parameter<T>& c);

  //< Deletes the Parameter<T> at the given index.
  size_type removeControl(size_type index);

  /* Gets the parameter at the given index.
      @return gets nullptr if the index if out of bounds. */
  Parameter<T>* getParam(size_type index);

  size_type getNumParams() const;
  //< gets the number of parameters in the set.

  _::Printer& Print(_::Printer& print) const;
  /* Prints this object to a string. */

 private:
  _::Array<Parameter<T>> params;  //< An array of Parameter<T> pointers.
};                                //< class ParameterSet
}       // namespace _
#endif  //< HEADER_FOR_KT_HMI_PARAMETERSET
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
