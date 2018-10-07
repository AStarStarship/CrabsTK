/* Kabuki Toolkit
    @file    /library/kt/id/authenticator_default.cc
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

#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "authenticator_default.h"
#include "user.h"

using namespace _ {;
using namespace std;
using namespace kt::id;

namespace _ {

AuthenticatorDefault::AuthenticatorDefault()
    : min_handle_length_(Handle::kDefaultMinLength),
      max_handle_length_(Handle::kMaxLength),
      min_password_length_(Password::kDefaultMinLength),
      max_password_length_(Password::kMaxLength) {
  // Nothing to do here. :-)
}

int AuthenticatorDefault::GetMinHandleLength() { return min_handle_length_; }

int AuthenticatorDefault::GetMaxHandleLength() { return max_handle_length_; }

int AuthenticatorDefault::GetMinPasswordLength() {
  return min_password_length_;
}

int AuthenticatorDefault::GetMaxPasswordLength() {
  return max_password_length_;
}

const char* AuthenticatorDefault::HandleIsInvalid(const char* input) {
  const char* result;

  if (input == nullptr) {
    result = "nil input";
    // cout << "\n| " << result;
    return result;
  }
  int length = StrandLength(input);
  // cout << "\n| Checking handle:\"" << input << "\"";

  if (length < min_handle_length_) {
    result = "Handle too short";
    // cout << "\n| " << result;
    return result;
  }
  // cout << '.';
  if (length > max_handle_length_) {
    result = "Handle too uint32_t";
    // cout << "\n| " << result;
    return result;
  }
  if (length == 0) {  //< min
    return nullptr;
  }
  // cout << ". ";
  for (--length; length > 0; --length) {
    // cout << length << ' ';
    if (isspace(input[length])) {
      result = "password can't contain whitespace.";
      // cout << "\n| " << result;
      return result;
    }
  }
  // cout << "\n| Handle is valid.";
  return nullptr;
}

const char* AuthenticatorDefault::PasswordIsInvalid(const char* input) {
  if (input == nullptr) {
    return "nil input";
  }
  const char* result;
  // cout << "\n| Checking password:\"" << input << "\"";
  int length = StrandLength(input);
  if (length < min_password_length_) {
    result = "Password too short";
    // cout << "\n| " << result;
    return result;
  }
  // cout << '.';
  if (length > max_password_length_) {
    result = "password too uint32_t";
    // cout << "\n| " << result;
    return result;
  }
  // cout << '.';
  // if (length == 0) { //< Min password length never less than 1.
  //    return nullptr;
  //}
  for (--length; length > 0; --length) {
    if (isspace(input[length])) {
      result = "password can't contain whitespace.";
      // cout << "\n| " << result;
      return result;
    }
  }
  // cout << "\n| Password is valid.";
  return nullptr;
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
