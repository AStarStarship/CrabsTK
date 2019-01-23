/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cportali2c.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_SPI_PORTAL
#define INCLUDED_KABUKI_FEATURE_TEK_SPI_PORTAL 1

namespace _ {

/* A type of Portal that allows for reading and writing to the local system. */
class SpiPortal : public _::Portal {
public:

  /* Constructs a SpiPortal. */
  SpiPortal (_::Expr* expr, PinName mosi_pin, PinName miso_pin,
	     PinName clock_pin, PinName strobe_pin);

  /* Feeds B-Output bytes through the slot. */
  virtual void Feed ();

  /* Pulls B-Input bytes through the slot. */
  virtual void Pull ();

private:

  _::Expr* expr_;        //< Expr for this Portal.
  SI4            start_index_, //< Start index of the buffer.
		     stop_index_,  //< Stop index of the buffer.
		     buffer_size_; //< Buffer size in bytes.
  SPI            spi_;         //< SPI port.
};
}       //< namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_SPI_PORTAL
