/* Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/authenticator.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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
#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_VALIDATORDEFAULT
#define HEADER_FOR_KT_ID_VALIDATORDEFAULT

#include "authenticator.h"

namespace _ {

class AuthenticatorDefault : public Authenticator {
 public:
  /* Constructs a Foo. */
  AuthenticatorDefault();

  /* Gets the min handle length. */
  int GetMinHandleLength();

  /* Gets the max handle length. */
  int GetMaxHandleLength();

  /* Gets the min password length. */
  int GetMinPasswordLength();

  /* Gets the max password length. */
  int GetMaxPasswordLength();

  /* Function validates the handle for correctness.
      @param  handle The handle to validate.
      @return Returns null upon success and a pointer to an error string
              if the input is invalid. */
  virtual const char* HandleIsInvalid(const char* handle);

  /* Function validates the password for correctness.
      @param  password The password to validate.
      @return Returns null upon success and a pointer to an error string
              if the input is invalid. */
  virtual const char* PasswordIsInvalid(const char* password);

 private:
  int min_handle_length_, max_handle_length_, min_password_length_,
      max_password_length_;
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_VALIDATORDEFAULT
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
