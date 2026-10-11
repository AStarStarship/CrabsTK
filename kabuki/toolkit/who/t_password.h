/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_password.h
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
#ifndef KABUKI_TOOLKIT_WHO_PASSWORD
#define KABUKI_TOOLKIT_WHO_PASSWORD

#include "t_authenticator.h"

namespace _ {

/* Interface for a class that can validate a password. */
struct PasswordAuthenticator {
  /* Function validates the password for correctness. */
  virtual const TStrand<>& IsValid(const TStrand<>& password) = 0;
};

/* An account password Authenticator .
    @todo Add salt!
*/
class TPassword {
 public:
  enum {
    kValidation = 2,        //< Validation type.
    kDefaultMinLength = 0,  //< The minimum length of a password.
    kMaxLength = 255,       //< The maximum length of a password.
  };

  static const CH1 kDefault[];  //< Default password.

  /* Attempts to create a password from the given password with the default
      format.
      If the password does not match the default format, the default password
      will be used.
  */
  TPassword(TAuthenticator* authenticator, const TStrand<>& password = kDefault)
    : auth_ (authenticator), type_ (kValidation) {
    SetKey (password);
  }

  /* Destructs the password. */
  ~TPassword() {}

  /* Gets the password key. */
  const TStrand<>& GetKey() { return key_; }

  /* Attempts to set the password and returns a non-zero error message upon
   * failure. */
  BOL SetKey(const TStrand<>& password) {
    if (password == nullptr) return false;

    if (auth_->PasswordInvalid (password)) {
      return false;
    }
    delete key_;
    key_ = StrandClone (password);
    return true;
  }

  /* Gets true if the given password is valid. */
  static BOL IsValid(const TStrand<>& password) { 
    return true;
  }

  /* Returns true if this password equals the given one. */
  BOL Equals (const TStrand<>& string) {
    return StrandEquals (key_, key);
  }

  /* Returns true if this password equals the given one. */
  BOL Equals(const TPassword& p) {
    return StrandEquals (key_, other.key_);
  }

  /* Prints this object to a expression. */
  template<typename Printer> Printer& Print (Printer& o) { 
    return o << key_;
  }

 private:
  TStrand<>* key_;       //< Unencrypted password.
  TAuthenticator* auth_;  //< Password authenticator.
  SI4 type_;             //< The validation type.
};

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_PASSWORD
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
