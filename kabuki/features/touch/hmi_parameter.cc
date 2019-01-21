/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_parameter.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "parameter.h"

namespace _ {

void PrintMacroHeader() {
  Printf("|");
  PrintCentered("Parameter", HmiComponent::MaxLabelLength);
  Printf(
      "|    Type    |#Bit|Init | min | max |Value|Ch |CC |  Action  |Step |");
}

Parameter::Parameter(uint16_t type, const char* label, uint16_t channel,
                     uint16_t init_value, uint16_t min_value,
                     uint16_t max_value, uint16_t word_size)
    : Parameter(label) {
  setType(type);
  setWordSize(word_size);
  setChannel(channel);
  setMinMaxValues(min_value, max_value);
  setInitValue(init_value);
  setValue(init_value);
}

Parameter::Parameter(const Parameter& o)
    : type(o.type),
      channel(o.channel),
      num_channels_(o.num_channels_),
      wordSize(o.wordSize),
      wordValue(o.wordValue),
      minWordSize(o.minWordSize),
      maxWordSize(o.maxWordSize),
      minWordValue(o.minWordValue),
      maxWordValue(o.maxWordValue),
      value_(o.value_),
      init_value_(o.init_value_),
      min_value_(o.min_value_),
      max_value_(o.max_value_)

// Nothing to do here. :-)
}  // namespace _

int Parameter::compare(const Parameter& o) const {
  if (type_ != o.type_ || channel_ != o.channel_ || value_ != o.value_ ||
      min_value_ != o.min_value_ || max_value_ != o.max_value_) {
    /*
    Logger::outputDebugString ("The variables were not the same: -1\n" +
    ConsoleLine ('-') + print () + ConsoleLine ('-') + o.print () + ConsoleLine
    ('-'));
    */
    return -1;
  }

  uint16_t compare_value = label_->Compare(o.label_);
  /*
  Logger::OutputDebugString ("The variables were the same: compare_value = " +
  string (compare_value) +
  "\nLabels: { " + label_ + ", " + o.label () + " }\nlabel ().compare (o.label
  ()) = " + string (label_.Compare (o.label_)));
  */
  return compare_value;
}

uint16_t Parameter::GetType() const { return type_; }

const char* Parameter::GetTypeString() const {
  switch (type_) {
    case ControlDmx:
      return "DMX Control";
    case MidiControl:
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

uint16_t Parameter::GetChannel() const { return channel; }

uint16_t Parameter::GetNumChannels() const { return num_channels_; }

uint16_t Parameter::GetValue() const { return value_; }

uint16_t Parameter::GetLSBValue() const { return value_ & 0xff; }

uint16_t Parameter::GetMsbValue() const { return (value_ & 0xff00) >> 8; }

uint16_t Parameter::GetInitValue() const { return init_value_; }

uint16_t Parameter::GetMinValue() const { return min_value_; }

uint16_t Parameter::GetMaxValue() const { return max_value_; }

uint16_t Parameter::GetWordSize() const { return word_size_; }

uint16_t Parameter::GetMinWordSize() const { return min_word_size_; }

uint16_t Parameter::GetMaxWordSize() const { return max_word_size; }

uint16_t Parameter::GetMinWordValue() const { return min_word_value; }

uint16_t Parameter::GetMaxWordValue() const { return maxWordValue; }

void Parameter::GetValue(uint16_t value_) {
  if (value_ < min_value_)
    value_ = min_value_;
  else if (value_ > max_value_)
    value_ = max_value_;
  else
    value_ = value_;
}

void Parameter::GetInitValue(uint16_t value_) {
  if (value_ < min_value_)
    init_value_ = min_value_;
  else if (value_ > max_value_)
    init_value_ = max_value_;
  else
    init_value_ = value_;
}

void Parameter::GetMinMaxValues(uint16_t newMin, uint16_t newMax) {
  // Ensure the max is greater than the min.
  if (newMax < newMin) {
    uint16_t temp = newMax;
    newMax = newMin;
    newMin = temp;
  }

  if (newMin < 0) newMin = 0;
  if (newMax < 0) newMax = 0;

  uint16_t theMaxWordValue = maxWordValue;

  if (newMin > theMaxWordValue) newMin = theMaxWordValue;
  if (newMax > theMaxWordValue) newMax = theMaxWordValue;

  min_value_ = newMin;
  max_value_ = newMax;

  SetValue(value_);
  SetInitValue(init_value_);
}

void Parameter::GetMinValue(uint16_t value) {
  if (value < 0)
    min_value_ = 0;
  else if (value > max_value_)
    min_value_ = max_value_;
  else
    min_value_ = value;

  if (value < min_value_) value = min_value_;
  if (init_value_ < max_value_) init_value_ = max_value_;
}

void Parameter::GetMaxValue(uint16_t value_) {
  if (value_ > maxWordValue)
    max_value_ = maxWordValue;
  else if (value_ < min_value_)
    max_value_ = min_value_;
  else
    max_value_ = value_;

  if (value_ > max_value_) value_ = max_value_;
  if (init_value_ > max_value_) init_value_ = max_value_;
}

void Parameter::GetLSBValue(uint16_t value_) {
  value_ = (value_ & 0xff00) | (value_ & 0xff);
}

void Parameter::GetMSBValue(uint16_t value_) {
  value_ = (value_ & 0x00ff) | ((value_ & 0xff) << 8);
}

void Parameter::SetChannel(uint16_t value_) {
  if (value_ < 0)
    channel = 0;
  else if (value_ > num_channels_)
    channel = num_channels_;
  else
    channel = value_;
}

void Parameter::GetWordSize(uint16_t value_) {
  if (value_ != minWordSize() && value_ != maxWordSize())
    value_ = minWordSize();

  wordSize = value_;
  maxWordValue = (value_ == minWordValue) ? minWordValue : maxWordValue;
  setMinMaxValues(min_value_, max_value_);
}

void Parameter::toggle() {
  if (value_ != min_value_)
    value_ = min_value_;
  else
    value_ = max_value_;
}

string Parameter::headerString() const {
  return "|" + PrintCentered("Parameter", Parameter::MaxLabelLength) +
         "|    Type    |#Bit|Init | min | max |Value|Ch |";
}

string Parameter::toStringRow() const {
  return "|" + PrintCentered(label(), Parameter::MaxLabelLength) + "|" +
         PrintCentered(typeString(), 12) + "|" +
         PrintCentered(string(wordSize), 4) + "|" +
         PrintCentered(string(init_value_), 5) + "|" +
         PrintCentered(string(min_value_), 5) + "|" +
         PrintCentered(string(max_value_), 5) + "|" +
         PrintCentered(string(value_), 5) + "|" +
         PrintCentered(string(channel), 3) + "|";
}

void Parameter::print() const {
  return getHeaderString() + "\n" + ToStringRow() + "\n";
}

void Parameter::GetType(uint16_t value_) {
  if (value_ < ControlDmx)
    value_ = ControlDmx;
  else if (value_ > ButtonMacro)
    value_ = ButtonMacro;

  type = value_;

  if (value_ == ControlDmx || value_ == DmxButton) {
    min_word_size_ = 7;
    maxWordSize = 14;
    minWordValue = 255;
    max_word_value_ = 0xffff;
    num_channels_ = 512;
    return;
  }
  if (value_ == MidiControl || value_ == ButtonMacro) {
    min_word_size_ = 8;
    maxWordSize = 16;
    minWordValue = 127;
    max_word_value_ = 0x3fff;
    num_channels_ = 16;
    return;
  }
  if (value_ == OSCControl || value_ == OSCButton) {
    // To do: Write me!
    return;
  }
  // Else its a Macro Button
  min_word_size_ = 1;
  max_word_size_ = 32;
  min_word_value_ = 1;
  max_word_value_ = 0x7fffffff;
  num_channels_ = 0x7fffffff;
}

}  // namespace kt
}  //< namespace kt
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
