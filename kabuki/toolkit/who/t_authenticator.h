/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_authenticator.h
@author  Cale McCollough <calemccollough.github.io>
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
#if SEAM >= KABUKI_TOOLKIT_WHO_1
#ifndef KABUKI_TOOLKIT_WHO_AUTHENTICATOR
#define KABUKI_TOOLKIT_WHO_AUTHENTICATOR


namespace _ {

/* Interface for a class that can validate a string for correctness.
This interface is useful for making rules for things like Handle(s) and
Password(s). Classes that implement this interface must define the indexes
of the types. */
struct TAuthenticator {
  /* Function validates the handle for correctness.
  @param  handle The handle to validate. */
  virtual const CH1* HandleIsValid(const TStrand<>& handle) = 0;

  /* Function validates the password for correctness.
  @param  password The password to validate.*/
  virtual const CH1* PasswordInvalid(const TStrand<>& password) = 0;
};

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_AUTHENTICATOR
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
