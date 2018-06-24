/* Kabuki Toolkit
@file    ~/source/hmi/hmi_dmx_control.cc
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

#include <stdafx.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "control_dmx.h"

namespace _ {

ControlDmx::ControlDmx(const char* label, int channel, int init_value,
                       int min_value, int max_value, int word_size,
                       int control_type)
    : Parameter<int>(control_type, label, channel, init_value, min_value,
                     max_value, word_size) {
  // Nothing to do here! :-)
}

ControlDmx::ControlDmx(const ControlDmx& other) : Parameter<int>(other) {
  // Nothing to do here! :-)
}

void ControlDmx::Print() const {}

}       //< namespace _
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
