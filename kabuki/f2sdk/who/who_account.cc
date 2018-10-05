/* kabuki::pro
    @file    $kabuki-toolkit/library/kt/id/id_account.cc
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
#include "account.h"

namespace _ {

Account::Account(const char* username) : name_(username) {}

Account::Account(const char* username, const char* password)
    : name_(username), password_(password) {
  // role = new Role ();
}

bool Account::IsValid() {
  /*
  if (password == nullptr)
  {
      // The account does not require a password
      if (!name.IsValid ())
          return false;

      return true;
  }
  else // we have to test the password
  if (!name.IsValid () || !password.IsValid ())
      return false;
  */
  return true;
}

const char* Account::GetName() { return name_.GetKey(); }

bool Account::SetName(const char* string) { return name_.SetKey(string); }

bool Account::RequiresPassword() { return password_.Equals(""); }

void Account::SetPassword(const char* string) { password_.SetKey(string); }

Roster& Account::GetUsers() { return users_; }

void Account::AddUser(User& user) { users_.Add(user); }

void Account::Print(Log& log) {
  log << "Account Name: " << name_.GetKey()
      << " Password: " << password_.GetKey();
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
