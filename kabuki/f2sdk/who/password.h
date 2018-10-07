/* Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/password.h
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
#ifndef HEADER_FOR_KT_ID_PASSWORD
#define HEADER_FOR_KT_ID_PASSWORD

#include "authenticator.h"

namespace _ {

/* Interface for a class that can validate a password. */
struct PasswordAuthenticator {
  /* Function validates the password for correctness. */
  virtual const char* IsValid(const char* password) = 0;
};

/* An account password Authenticator .
    @todo Add salt!
*/
class API Password {
 public:
  enum {
    kValidation = 2,        //< Validation type.
    kDefaultMinLength = 0,  //< The minimum length of a password.
    kMaxLength = 255,       //< The maximum length of a password.
  };

  static const char kDefault[];  //< Default password.

  /* Attempts to create a password from the given password with the default
      format.
      If the password does not match the default format, the default password
      will be used.
  */
  Password(Authenticator* authenticator, const char* password = kDefault);

  /* Destructs the password. */
  ~Password();

  /* Gets the password key. */
  const char* GetKey();

  /* Attempts to set the password and returns a non-zero error message upon
   * failure. */
  bool SetKey(const char* password);

  /* Gets true if the given password is valid. */
  static bool IsValid(const char* password);

  /* Returns true if this password equals the given one. */
  bool Equals(const char* string);

  /* Returns true if this password equals the given one. */
  bool Equals(const Password& p);

  /* Prints this object to a expression. */
  Text& Out(Text& txt = Text());

 private:
  char* key_;                     //< Unencrypted password.
  Authenticator* authenticator_;  //< Password authenticator.
  int type_;                      //< The validation type.
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_PASSWORD
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
