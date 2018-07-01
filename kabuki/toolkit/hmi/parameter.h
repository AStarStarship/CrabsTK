/* Kabuki Toolkit
@file    ~/source/hmi/hmi_parameter.h
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
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_KT_HMI_PARAMETER
#define HEADER_FOR_KT_HMI_PARAMETER

#include "component.h"

namespace _ {

/* A parameter with upper and lower bounds mapped to an Script device.
        

*/
class KABUKI Parameter : public HmiComponent {
 public:
  typedef enum {
    MidiControl = 0,
  };

  /* Default constructor. */
  Parameter(uint16_t newType, const char* label = "", int channel = 0,
            uint16_t init_value = 0, uint16_t min_value = 0,
            uint16_t max_value_ = 1);

  /* Copy constructor deep copies the other object. */
  Parameter(const Parameter& other);

  /* Virtual destructor. */
  virtual ~Parameter() {}

  /* Gets what type of control this is. */
  uint16_t GetType() const;

  /* Gets a string that says what type of Parameter this is. */
  const char* GetTypeString() const;

  /* Gets the output channel of this control. */
  int GetChannel() const;

  /* Gets the number of channels. */
  int GetNumChannels() const;

  /* Gets the number of bits of in a control word. */
  uint16_t GetWordSize() const;

  /* Gets the minimum word size. */
  uint16_t GetMinWordSize() const;

  /* Gets the minimum integer value of a word. */
  uint16_t GetMinWordValue() const;

  /* Gets the maximum word size. */
  uint16_t GetMaxWordSize() const;

  /* Gets the maximum integer value of a word. */
  uint16_t getMaxWordValue() const;

  /* Gets the current value of this control. */
  uint16_t GetValue() const;

  /* Gets the default MIDI value of this control. */
  uint16_t GetInitValue() const;

  /* Gets the min value of this control. */
  uint16_t GetMinValue() const;

  /* Gets the max value of this control. */
  uint16_t GetMaxValue() const;

  /* Sets the output channel to value. */
  void SetChannel(uint16_t value);

  /* Sets the control word size to the new value. */
  void SetWordSize(uint16_t value);

  /* Sets this control's value of the parameter to the value. */
  void SetValue(uint16_t value);

  /* Sets the initial value of a new control to the value. */
  void SetInitValue(uint16_t value);

  /* Sets the min and max values.
      @pre The word size must be set before calling this function! */
  virtual void SetMinMaxValues(uint16_t newMin, uint16_t newMax);

  /* Sets the min value to the value. */
  virtual void SetMinValue(uint16_t value);

  /* Sets the max value to the value. */
  virtual void SetMaxValue(uint16_t value);

  /* Toggles between the min and the max values. */
  virtual void Toggle();

  /* Compares this Parameter to the given Parameter.
      @return gets 0 if both Controls are the same.
      @return gets 1 if the controls are not identical. */
  virtual int Compare(const Parameter& p) const;

  /* Gets the header for toStringRow (). */
  virtual void PrintHeader() const;

  /* Gets a column of the values without the labels. */
  virtual void PrintRow() const;

  /* Prints this object to a string. */
  _::Utf& Print(_::Utf& print) const;

 private:
  const uint16_t* channel_;  //< Output channel.
  uint16_t value,            //< Parameter value.
      init_value_,           //< Initial value.
      min_value_,            //< Min value.
      max_value_;            //< Max value.
};

class ParameterPair {
 public:
  /* A pair of parameters to create a higher resolution parameter. */
  ParameterPair(uint16_t& msb, uint16_t& lsb) : msb(msb), lsb(lsb) {}

  /* Sets the LSB value to the value. */
  void SetLsbValue(uint16_t value);

  /* Gets the LSB of this value. */
  uint16_t GetLsbalue() const;

  /* Gets the MSB of this value. */
  uint16_t GetMsbValue() const;

  /* Sets the MSB value to the value. */
  void SetMsbValue(uint16_t value);

 private:
  uint16_t &msb, &lsb;
};  //< ParameterPair
}       // namespace _
#endif  //< HEADER_FOR_KT_HMI_PARAMETER
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
