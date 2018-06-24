/* Kabuki Toolkit
@file    ~/source/hmi/hmi_midi_widget.h
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
#ifndef HEADER_FOR_KT_HMI_MIDIWIDGET
#define HEADER_FOR_KT_HMI_MIDIWIDGET

#include "widget.h"

namespace _ {

class WidgetMidi : public Widget {
 public:
  enum { MinDoublePressTime = 1 MaxDoublePressTime = 1000 };

  WidgetMidi(byte common_channel, byte keyboard_channel, byte drums_channel);

  WidgetMidi(const WidgetMidi& o);

  ~WidgetMidi();

  byte SetCommonChannel();

  void SetCommonChannel(byte c);

  byte GetKeyboardChannel();

  void SetKeyboardChannel(byte channel);

  byte GetDrumsChannel();

  void SetDrumsChannel(byte channel);

  Time GetDoublePressTime();

  int SetDoublePressTime(timestamp_t new_time);

  _::Printer& Print(_::Printer& print);
};
}       //< namespace _
}  //< namespace kt
#endif  //< HEADER_FOR_KT_HMI_MIDIWIDGET
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
