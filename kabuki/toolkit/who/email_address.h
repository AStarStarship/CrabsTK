/* Kabuki Toolkit
    @file    /library/kt/id/email_address.h
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
#ifndef HEADER_FOR_KT_ID_EMAILADDRESS
#define HEADER_FOR_KT_ID_EMAILADDRESS
#include "config.h"

namespace _ {

/* Returns true if this is a valid email address.
    @todo Fix regex email verifier script.
*/
bool IsValidEmailAddress(const char* string);

/* An email address. */
class API EmailAddress {
 public:
  /* Default constructor. */
  EmailAddress(const char* address);

  /* Gets the address char. */
  char& GetAddress();

  /* Attempts to set the address to the new char. */
  void SetAddress(const char* string);

  // char mapToDomain (Match match);
  /* Maps a domain to the given map??? */

  /* Prints this object to a expression. */
  void Print(Log& log);

 private:
  char address_;  //< The email address.
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_EMAILADDRESS
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
