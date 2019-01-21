/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cdmxreceiver.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_DMX_DMXTRANSCEIVER_H
#define INCLUDED_KABUKI_FEATURE_TEK_DMX_DMXTRANSCEIVER_H

#include "cdmxreceiver.h"
#include "cdmxtransmitter.h"

namespace _ {

template <UI1 NumPorts>
class DMXTransceiver {
 public:
  /* Default constructor. */
  DMXTransceiver();

  /* Returns a reference to the DMXTransmitter. */
  DMXTransmitter& GetTransmitter();

  /* Returns a reference to the DMXReceiver. */
  DMXReceiver& GetReceiver;

 private:
  DMXTransmitter transmitter_;
  DMXReceiver receiver_;
};
}  // namespace _
#endif KABUKI_FEATURE_TEK_DMX_DMXTRANSCEIVER_H
