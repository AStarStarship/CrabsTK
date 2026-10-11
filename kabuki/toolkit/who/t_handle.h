/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_handle.h
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
#ifndef KABUKI_TOOLKIT_WHO_HANDLE
#define KABUKI_TOOLKIT_WHO_HANDLE

#include "t_authenticator.h"

namespace _ {

/* A handle like a CH1 or UID.
*/
class THandle {
 public:
  enum {
    kValidation = 1,        //< Validation type.
    kDefaultMinLength = 3,  //< Min length of a Handle range.
    kMaxLength = 255        //< Max length of a Handle range.
  };

  static const CH1* Default ();

  /* Constructor creates a standard CH1. */
  THandle(TAuthenticator* authenticator, const TStrand<>& key,
         SI4 min_length = kDefaultMinLength, SI4 max_length = kMaxLength)
    : auth_ (authenticator), type_ (kValidation) {
    if (min_length >= max_length) {
      SI4 temp = min_length;
      min_length = max_length;
      max_length = temp;
    }
    key = key;
    if (min_length < kDefaultMinLength)
      min_length = kDefaultMinLength;
    else if (min_length > kMaxLength)
      min_length = kDefaultMinLength;
    key_ = StrandClone (key);
  }

  /* Gets a reference to the handle string. */
  const TStrand<>& GetKey() { return key_; }

  /* Gets true if this password is value. */
  BOL SetKey(const TStrand<>& key) {
    size_t length = StrandLength (key);
    if (length < kDefaultMinLength || length > kMaxLength) return false;

    if (!IsValid (key)) return false;
    delete key_;
    key_ = StrandClone (key);
    return true;
  }

  /* Returns true if the handle is valid. */
  BOL IsValid(const TStrand<>& key) {
    size_t length = StrandLength (key);
    if (length < kDefaultMinLength || length > kMaxLength) return false;
    return true;
  }

  /* Returns true if this Handle is identical to the given Handle. */
  BOL Equals(const THandle& h) { return StrandEquals (key_, h.key_); }

  /* Returns true if this Handle is identical to the given Handle. */
  BOL Equals(const TStrand<>& handle) { return StrandEquals (key_, handle); }

  /* Prints this object to the log. */
  template<typename Printer> Printer& Print (Printer& o) { return txt << "\nHandle: " << key_; }

 private:
  TStrand<>& key_;        //< Unique string key.
  TAuthenticator* auth_;   //< Handle authenticator.
  SI4 type_;              //< The validation type.

};  //< class Handle

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_HANDLE
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
