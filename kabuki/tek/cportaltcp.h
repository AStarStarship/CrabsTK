/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cpotaltcp.h
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
#ifndef INCLUDED_KABUKI_FEATURE_TEK_I2C_PORTAL
#define INCLUDED_KABUKI_FEATURE_TEK_I2C_PORTAL 1

namespace _ {

class PortalTCP : public _::Portal {
 public:
  /* Constructor creates a PortalTCP. */
  PortalTCP(_::Expr* expr);

  /* Sets the Portal up for a batch of bytes transfer.
  Implementation of this function is not required to do anything, but
  regardless it will be called by the Set. */
  // virtual void Prime ();

  /* Gets the length of current portal.
      @warning Length might not be the actual length, but rather the length
                of the data that is read to be pulled. */
  // virtual uint_t Length ();

  /* Feeds tx messages through the a without scanning them. */
  virtual void Feed();

  /* Pulls rx messages through the a and runs them through the scanner. */
  virtual void Pull();

 private:
  _::Expr* expr_;  //< Expr for this Portal.
};                 //< class PortalTCP
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_H_I2C_PORTAL
