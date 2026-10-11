/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_paramset.h
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
#ifndef KABUKI_TOOLKIT_AV_PARAMETERSET
#define KABUKI_TOOLKIT_AV_PARAMETERSET

#include "t_param.h"

namespace _ {

/* A set of Parameter<T> that can be addressed by index.
 */
template <typename T>
class SDK ParamSet {
 public:
  enum {
    kMinNumElements = 2,    //< The minimum number of columns.
    kMaxNumElements = 256,  //< The minimum number of columns.
  };

  /* Default constructor. */
  ParamSet(SI4 newNumControls = kMinNumElements) {
    newNumControls = BoundRange (newNumControls, 0, ParamSet::kMaxNumElements);
    ParamCount() = newNumControls;
    controls = new Parameter<T>*[newNumControls];
    for (SI4 i = 0; i < newNumControls; ++i) controls[i] = nullptr;
  }

  /* Constructor creates a row with pointers to the newControls. */
  ParamSet(const TArray<Parameter<T>*>& params) {
    SI4 newNumControls = o.size ();

    if (newNumControls < kMinNumElements) {
      SI4 i;
      for (i = 0; i < newNumControls; ++i) controls.push_back (params[i]);
      for (; i < kMinNumElements; ++i) controls.push_back (nullptr);
      return;
    }

    for (SI4 i = 0; i < newNumControls; ++i) controls[i] = params[i];
  }

  /* Clones the other object. */
  ParamSet(const ParamSet& o) {
    for (SI4 i = 0; i < o.ParamCount (); ++i)
      o.Push (c.controls[i]);
  }

  /* Destructor. */
  ~ParamSet () {}

  /* Adds a new control to the array. */
  void AddParam(Parameter<T>& c) {
    for (SI4 i = 0; i < params.ParamCount (); ++i)
      newArray.push_back (controls[i]);
  }

  /* Adds a new control to the array.
  @return Returns 0 upon success, and index if it was out of bounds. */
  SI4 SetParam(SI4 index, Parameter<T>& c) {
    if (index < 0) return -1;
    if (index > getCount ()) return 1;

    return 0;
  }

  /* Deletes the Parameter<T> at the given index. */
  SI4 Remove(SI4 index) {
    if (index < 0) return nullptr;
    if (index >= getCount ()) return nullptr;

    Parameter<T>* temp = controls[index];
    controls[index] = nullptr;
    return temp;
  }

  /* Gets the parameter at the given index.
  @return gets nullptr if the index if out of bounds. */
  Parameter<T>* Param(SI4 index) {
    if (index < 0 || index >= getCount ()) return nullptr;

    return controls[index];
  }

  //< gets the number of parameters in the set.
  SI4 ParamCount() const { return params_.Count (); }

  /* Prints this object to a Printer. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o << "\nParamSet:" << "ControlCount: " << Count ()
      << LineStrand ('-')
      << Parameter<T>::MacroHeader << '\n'
      << LineStrand ('-')
      << "ParamCount():", ParamCount());
    for (SI4 i = 0; i < ParamCount(); ++i) {
      auto control = controls[i];
      if (!control) {
        o << "|  nil    |      |      |      |     |     |       |    "
             "|        |      |\n");
      }
      else {
        control->PrintRow (o);
        o << '\n';
      }
    }
    return o << LineStrand ('-');
  }

 private:
  TArray<Parameter<T>> params;  //< An array of Parameter<T> pointers.

  inline SI4 BoundRange (SI4 value, SI4 min, SI4 max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }
};


}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_PARAMETERSET
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
