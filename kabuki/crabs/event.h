/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    kabuki-toolkit.git/kabuki/crabs/event.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= SEAM_0_0_4
#ifndef INCLUDED_CRABS_EVENT
#define INCLUDED_CRABS_EVENT
// Dependencies:
#include "operand.h"
// End dependencies.

namespace _ {

/* Interface for a temporal event in a Bayesian net.
    






    @code
    #include <script/Event.h>
    struct API Example: public Event {
        virtual void Trigger () {
            // ...
        }
    };
    @endcode
*/
struct API Event : public Operand {
  /* Virtual destructor. */
  virtual ~Event() = 0;

  /* Triggers the event. */
  virtual void Trigger() = 0;

  virtual double GetProbability() = 0;

  /* Script operations. */
  virtual const Op* Star(wchar_t index, Expr* expr) = 0;
};

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_4
#endif  //< INCLUDED_CRABS_EVENT
