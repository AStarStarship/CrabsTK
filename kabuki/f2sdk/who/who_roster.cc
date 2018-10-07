/* kabuki::pro
    @file    /library/kt/id/id_roster.cc
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
#include "roster.h"

namespace _ {

Roster::Roster() {}

int Roster::GetNumUsers() { return users_.GetCount(); }

bool Roster::Add(const User& user) {
  users_.Push(new User(user));
  return true;
}

bool Roster::Contains(const char* username) {
  for (int index = 0; index < users_.GetCount(); index++) {
    if (users_[index]->Equals(username)) return true;
  }
  return false;
}

bool Roster::Contains(const User& user) {
  for (int index = 0; index < users_.GetCount(); index++) {
    if (users_[index]->Equals(user)) return true;
  }
  return false;
}

User* Roster::Find(const char* username) {
  size_t length = strlen(username);
  if (length == 0) {
    return nullptr;  // static website guest account
  }

  for (int index = 0; index < users_.GetCount(); index++) {
    User* user = users_.Element(index);
    if (users_[index]->Equals(username)) {
      return user;
    }
  }
  return nullptr;  // static website guest account
}

int Roster::GetCount() { return users_.GetCount(); }

void Roster::Print(Log& log) {
  /// This method creates a char of the users.Print (Log& log)
  /// strings separated by spaces

  log << "Number of Users: " << users_.GetCount() << "\n";

  for (int index = 0; index < users_.GetCount(); index++) {
    /// Iterated through the indexes array and write the
    /// Print (Log& log) strings to the returnString
    log << "User " << (index + 1) << ": "
        << users_[index]->GetUsername().GetKey() << "\n";
  }
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
