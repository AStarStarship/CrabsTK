/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_addressemail.h
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
#ifndef KABUKI_TOOLKIT_WHO_EMAILADDRESS
#define KABUKI_TOOLKIT_WHO_EMAILADDRESS


namespace _ {

/* An email address. */
class TAddressEmail {
 public:
  /* Default constructor. */
  TAddressEmail(const TStrand<>& address) {}

  /* Gets the address strand. */
  TStrand<>& GetAddress() { return address_; }

  /* Attempts to set the address to the given string. */
  void SetAddress(const TStrand<>& string) {
    // address_ = string;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {

  }

 private:
  TStrand<> address_;  //< The email address.
};

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_EMAILADDRESS
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
