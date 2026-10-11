/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_event.h
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
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_EVENT
#define KABUKI_TOOLKIT_AV_EVENT

#include "t_config.h"

namespace _ {

/* A event with an ASCII TSS (Time Subsecond) timestamp. */
class Event {
 public:
  /* Default constructor. */
  Event();

  /* Virtual destructor. */
  virtual ~Event();

  /* gets the timestamp of the Event. */
  TSS GetTimestamp();

  /* Triggers the event. */
  virtual void Trigger() = 0;

  /* Prints this object to a string. */
  template<typename Printer> 
  virtual Printer& Print(Printer& o) const = 0;

 private:
  timestamp_t timestamp;  //< Event timestamp in microseconds.
};

}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_EVENT
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
