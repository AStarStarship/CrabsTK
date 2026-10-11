/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_param.h
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
#ifndef KABUKI_TOOLKIT_AV_PARAMETER
#define KABUKI_TOOLKIT_AV_PARAMETER

#include "t_component.h"

namespace _ {

/* A parameter with upper and lower bounds.
*/
class SDK Parameter : public HmiComponent {
 public:
  typedef enum {
    ControlMidi = 0,
  };

  /* Default constructor. */
  Parameter(UI2 newType, const CH1* label = "", SI4 channel = 0,
            UI2 init_value = 0, UI2 min_value = 0,
            UI2 max_value_ = 1)
    : Parameter (label) {
    setType (type);
    setWordSize (word_size);
    setChannel (channel);
    setMinMaxValues (min_value, max_value);
    setInitValue (init_value);
    SetValue (init_value);
  }

  /* Copy constructor deep copies the other object. */
  Parameter(const Parameter& o)
    : type (o.type),
      channel (o.channel),
      channel_count_ (o.channel_count_),
      wordSize (o.wordSize),
      wordValue (o.wordValue),
      WordSizeMin (o.WordSizeMin),
      WordSizeMax (o.WordSizeMax),
      minWordValue (o.minWordValue),
      word_value_max_ (o.word_value_max_),
      value_ (o.value_),
      init_value_ (o.init_value_),
      min_value_ (o.min_value_),
      max_value_ (o.max_value_) {}

  /* Virtual destructor. */
  virtual ~Parameter() {}

  /* Gets what type of control this is. */
  UI2 GetType() const { return type_; }

  /* Sets the type to the new value*/
  void SetType (UI2 value) {
    if (value < ControlDMX)
      value = ControlDMX;
    else if (value > ButtonMacro)
      value = ButtonMacro;

    type = value;

    if (value == ControlDMX || value == DmxButton) {
      min_word_size_ = 7;
      WordSizeMax = 14;
      minWordValue = 255;
      max_word_value_ = 0xffff;
      channel_count_ = 512;
      return;
    }
    if (value == ControlMidi || value == ButtonMacro) {
      min_word_size_ = 8;
      WordSizeMax = 16;
      minWordValue = 127;
      max_word_value_ = 0x3fff;
      channel_count_ = 16;
      return;
    }
    if (value == OSCControl || value == OSCButton) {
      // To do: Write me!
      return;
    }
    // Else its a Macro Button
    min_word_size_ = 1;
    max_word_size_ = 32;
    min_word_value_ = 1;
    max_word_value_ = 0x7fffffff;
    channel_count_ = 0x7fffffff;
  }

  /* Gets a string that says what type of Parameter this is. */
  const CH1* GetTypeString() const {
    switch (type_) {
    case ControlDMX:
      return "DMX Control";
    case ControlMidi:
      return "MIDI Control";
    case OSCControl:
      return "OSC Control";
    case DmxButton:
      return "DMX Button";
    case ButtonMacro:
      return "MIDI Button";
    case OSCButton:
      return "OSC Button";
    default:
      return "Invalid type";
    }
  }

  /* Gets the output channel of this control. */
  SI4 GetChannel() const { return channel; }

  /* Gets the number of channels. */
  SI4 GetNumChannels() const { return channel_count_; }

  /* Gets the number of bits of in a control word. */
  UI2 GetWordSize() const { return word_size_; }

  /* Gets the minimum word size. */
  UI2 GetMinWordSize() const { return min_word_size_; }

  /* Gets the minimum integer value of a word. */
  UI2 GetMinWordValue() const { return min_word_value; }

  /* Gets the maximum word size. */
  UI2 GetMaxWordSize() const { return max_word_size; }

  /* Gets the maximum integer value of a word. */
  UI2 getMaxWordValue() const { return word_value_max_; }

  /* Gets the current value of this control. */
  UI2 GetValue() const { return value_; }

  /* Gets the default MIDI value of this control. */
  UI2 GetInitValue() const { return init_value_; }

  /* Gets the min value of this control. */
  UI2 GetMinValue() const { return min_value_; }

  /* Gets the max value of this control. */
  UI2 GetMaxValue() const { return max_value_; }

  /* Sets the output channel to value. */
  void SetChannel(UI2 value) {
    if (value_ < 0)
      channel = 0;
    else if (value_ > channel_count_)
      channel = channel_count_;
    else
      channel = value_;
  }

  /* Sets the control word size to the new value. */
  void SetWordSize(UI2 value) {
    if (value_ != WordSizeMin () && value_ != WordSizeMax ())
      value_ = WordSizeMin ();

    wordSize = value_;
    word_value_max_ = (value_ == minWordValue) ? minWordValue : word_value_max_;
    setMinMaxValues (min_value_, max_value_);
  }

  /* Sets this control's value of the parameter to the value. */
  void SetValue(UI2 value) {
    if (value_ < min_value_)
      value_ = min_value_;
    else if (value_ > max_value_)
      value_ = max_value_;
    else
      value_ = value_;
  }

  /* Sets the initial value of a new control to the value. */
  void SetInitValue(UI2 value) {
    if (value_ < min_value_)
      init_value_ = min_value_;
    else if (value_ > max_value_)
      init_value_ = max_value_;
    else
      init_value_ = value_;
  }

  /* Sets the min and max values.
  @pre The word size must be set before calling this function! */
  virtual void SetMinMaxValues(UI2 newMin, UI2 newMax) {
    // Ensure the max is greater than the min.
    if (newMax < newMin) {
      UI2 temp = newMax;
      newMax = newMin;
      newMin = temp;
    }

    if (newMin < 0) newMin = 0;
    if (newMax < 0) newMax = 0;

    UI2 theMaxWordValue = word_value_max_;

    if (newMin > theMaxWordValue) newMin = theMaxWordValue;
    if (newMax > theMaxWordValue) newMax = theMaxWordValue;

    min_value_ = newMin;
    max_value_ = newMax;

    SetValue (value_);
    SetInitValue (init_value_);
  }

  /* Sets the min value to the value. */
  virtual void SetMinValue(UI2 value) {
    if (value < 0)
      min_value_ = 0;
    else if (value > max_value_)
      min_value_ = max_value_;
    else
      min_value_ = value;

    if (value < min_value_) value = min_value_;
    if (init_value_ < max_value_) init_value_ = max_value_;
  }

  /* Sets the max value to the value. */
  virtual void SetMaxValue(UI2 value) {
    UI bounds = word_value_max_;
    if (value > bounds)
      max_value_ = bounds;
    else if (value < (bounds = word_value_min_))
      max_value_ = bounds;
    else
      max_value_ = value;

    if (value > max_value_) value = max_value_;
    if (init_value_ > max_value_) init_value_ = max_value_;
  }

  /* Toggles between the min and the max values. */
  virtual void Toggle() {
    if (value_ != min_value_)
      value_ = min_value_;
    else
      value_ = max_value_;
  }

  /* Compares this Parameter to the given Parameter.
  @return gets 0 if both Controls are the same.
  @return gets 1 if the controls are not identical. */
  virtual SI4 Compare(const Parameter& o) const {
    if (type_ != o.type_ || channel_ != o.channel_ || value_ != o.value_ ||
      min_value_ != o.min_value_ || max_value_ != o.max_value_) {
      PRINT (\n"The variables were not the same: -1\n");
      PRINT_LINE ('-');
      PRINT (this);
      PRINT_LINE ('-');
      PRINT (o);
      PRINT_LINE ('-');
      return -1;
    }

    UI2 compare_value = label_->Compare (o.label_);
    template<typename Printer>
    Printer& PrintDebug (Printer& o) const {
      o << "The variables were the same: compare_value = " 
        << compare_value 
        << "\nLabels: { " << label_ << ", " << o.label () 
        << " }\nlabel ().Compare (o.label_) = "
        << label_.Compare (o.label_));
    */
    return compare_value;
  }

  /* Gets a column of the values without the labels. */
  template<typename Printer>
  Printer& PrintRow(Printer& o) const {
    return o << '|' << Centered (label (), Parameter::MaxLabelLength) 
      << "|" << Centered (typeString (), 12)
      << "|" << Centered (string (wordSize), 4)
      << "|" << Centered (string (init_value_), 5)
      << "|" << Centered (string (min_value_), 5)
      << "|" << Centered (string (max_value_), 5)
      << "|" << Centered (string (value_), 5)
      << "|" << Centered (string (channel), 3)
  }

  /* Prints this object to a string. */
  template<typename Printer>
  Printer& Print (Printer& o) const {
    o << "\nParameter:" << index <<
      "\n|    Type    |#Bit|Init | min | max |Value|Ch |CC |  Action  "
      "|Step |" 
      << "\n" << ToStringRow ();
  }

 private:
  const UI2* channel_;  //< Output channel.
  UI2 value,            //< Parameter value.
      init_value_,           //< Initial value.
      min_value_,            //< Min value.
      max_value_;            //< Max value.

  template<typename Printer>
  Printer& PrintHeader (Printer& o) {
    o << '|' + Centered ("Parameter", Parameter::MaxLabelLength)
      << "|    Type    |#Bit|Init | min | max |Value|Ch |";
  }
};

class ParameterPair {
 public:
  /* A pair of parameters to create a higher resolution parameter. */
  ParameterPair(UI2& msb, UI2& lsb) : msb(msb), lsb(lsb) {}

  /* Sets the LSB value to the value. */
  void SetLsbValue(UI2 value) {
    value_ = (value_ & 0xff00) | (value_ & 0xff);
  }

  /* Gets the LSB of this value. */
  UI2 GetLsbalue() const { return value_ & 0xff; }

  /* Gets the MSB of this value. */
  UI2 GetMsbValue() const { return (value_ & 0xff00) >> 8; }

  /* Sets the MSB value to the value. */
  void SetMsbValue(UI2 value) {
    lsb_ = value_ & 0xff;
    msb_ = (value_ & 0xff) << 8;
  }

 private:
  UI1* msb, *lsb;
};  //< ParameterPair

}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_PARAMETER
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
