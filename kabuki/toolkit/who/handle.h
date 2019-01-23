/* Kabuki Toolkit
    @file    /library/kt/id/handle.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
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
#ifndef HEADER_FOR_KT_ID_HANDLE
#define HEADER_FOR_KT_ID_HANDLE

#include "authenticator.h"

namespace _ {

/* A handle like a char or UID.
 */
class API Handle {
 public:
  enum {
    kValidation = 1,        //< Validation type.
    kDefaultMinLength = 3,  //< Min length of a Handle range.
    kMaxLength = 255        //< Max length of a Handle range.
  };

  static const char kDefault[];

  /* Constructor creates a standard char. */
  Handle(Authenticator* authenticator, const char* key,
         int min_length = kDefaultMinLength, int max_length = kMaxLength);

  /* Gets a reference to the handle string. */
  const char* GetKey();

  /* Gets true if this password is value. */
  bool SetKey(const char* key);

  /* Returns true if the handle is valid. */
  bool IsValid(const char* key);

  /* Returns true if this Handle is identical to the given Handle. */
  bool Equals(const Handle& h);

  /* Returns true if this Handle is identical to the given Handle. */
  bool Equals(const char* handle);

  /* Prints this object to the log. */
  Text& Out(Text& txt = Text());

 private:
  char* key_;                     //< Unique string key.
  Authenticator* authenticator_;  //< Handle authenticator.
  int type_;                      //< The validation type.

};  //< class Handle

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_HANDLE
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
