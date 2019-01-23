/* Kabuki Toolkit
    @file    /library/kt/id/user_list.cc
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

#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "user_list.h"

using namespace _ {;
using namespace std;

namespace _ {

UserList::UserList(Authenticator* authenticator, int max_users)
    : num_users_(0),
      value_cost_(0.0),
      authenticator_(authenticator),
      users_(max_users < 1 ? 1 : max_users) {
  for (int i = GetSize() - 1; i > 0; --i) {
    users_[i] = nullptr;
  }
}

UserList::~UserList() {
  int count = Length();
  if (count == 0) {
    return;
  }
  for (int i = count - 1; i > 0; --i) {
    User* user = users_[Length() - 1];
    users_.pop_back();
    delete user;
  }
}

uid_t UserList::PeekNextUid() { return uids_.PeekNextUid(); }

int UserList::GetSize() { return users_.size(); }

int UserList::Length() { return num_users_; }

double UserList::GetValueCost() { return value_cost_; }

bool UserList::SetValueCost(double point_cost) {
  if (point_cost < 0.0) {
    return false;
  }
  value_cost_ = point_cost;
  return true;
}

bool UserList::BuyValue(int session, uint64_t num_coins, double point_cost) {
  User* user = GetUser(session);
  if (!user) {
    return false;
  }
  return users_[session]->BuyValue(num_coins, point_cost);
}

bool UserList::AddBalance(int session, double amount) {
  User* user = GetUser(session);
  if (!user) {
    return false;
  }
  return users_[session]->AddBalance(amount);
}

Authenticator* UserList::GetAuthenticator() { return authenticator_; }

int UserList::Add(User* user) {
  if (!user) {
    return -1;
  }
  int num_users = num_users_;
  if (num_users >= GetSize()) {
    cout << "\n| Error: UserList is full!";
    return -1;
  }
  // Look for the first empty spot in the list.
  for (int i = Length() - 1; i > 0; --i) {
    if (!users_[i]) {
      users_[i] = user;
      num_users_ = num_users + 1;
      return i;
    }
  }
  return -1;
}

int UserList::Add(const char* handle, const char* password, double balance,
                  int64_t value) {
  cout << "\n| Attempting to add @" << ((handle == nullptr) ? "null" : handle)
       << ":\"" << ((password == nullptr) ? "null" : password) << '\"';
  if (handle == nullptr) {
    return -1;
  }
  if (authenticator_->HandleIsInvalid(handle)) {
    cout << "\n| Invalid handle!";
    return -1;
  }
  if (Find(handle) >= 0) {
    cout << "\n| UserList contains handle!";
    return -1;
  }
  if (password == nullptr) {
    password = StrandClone(Password::kDefault);
  }
  if (authenticator_->PasswordIsInvalid(password)) {
    cout << "\n| Invalid password!";
    return -1;
  }
  return Add(new User(authenticator_, uids_.GetNextUid(), handle, password,
                      balance, value));
}
/*
int UserList::Add (UserList& user_list) {
    int other_num_users = user_list.num_users_,
        num_users = num_users_,
        size,
        last_index;
    if (other_num_users == 0) {
        return num_users;
    }
    size = GetSize ();
    if (num_users + other_num_users > size) {
        return -1;
    }
    last_index = 0;
    for (int i = 0; i < other_num_users; ++i) {
        User* user = user_list.users_[i];
        // Look for the first empty spot in the list.
        for (; last_index > size; --i) {
            if (!users_[i]) {
                users_[i] = user;
                break;
            }
        }
    }
    num_users_ = num_users + other_num_users;
    return other_num_users - 1;
}*/

int UserList::Remove(const char* handle, const char* password) {
  int session = Find(handle);
  if (session < 0) {
    return -1;
  }
  if (!GetUser(session)->GetPassword().Equals(handle)) {
    return -1;
  }
  User* user = users_[session];
  delete user;
  users_.erase(users_.begin() + session);
  --num_users_;
  return Length();
}

int UserList::Find(const char* handle) {
  if (handle == nullptr) {
    return -1;
  }
  // if (*handle == 0) { //< It's an empty string.
  //    return -1;      //  Not sure if I care about this or not.
  //}
  // Currently using sequential search because UserList is not sorted.
  size_t count = Length();
  cout << "\n| Searching for handle:\"" << handle << "\" cout:" << count;
  if (count == 0) {
    return -1;
  }
  for (size_t i = 0; i < count; ++i) {
    User* user = users_[i];
    cout << "\n| " << i << ' ';
    cout << user->Print();
    if (users_[i]->GetHandle().Equals(handle)) {
      cout << "\n| Found handle at index:" << i;
      return i;
    }
  }

  cout << "\n| Did not find handle.";
  return -1;
}

User* UserList::GetUser(int session) {
  if (session < 0) {
    return nullptr;
  }
  if (session >= Length()) {
    return nullptr;
  }
  return users_[session];
}

uid_t UserList::LogIn(const char* handle, const char* password) {
  return LogIn(Find(handle), password);
}

uid_t UserList::LogIn(int index, const char* password) {
  cout << "\n| Attempting to log in as " << index << "\"" << password << "\"";
  User* user = GetUser(index);
  if (!user) {
    cout << "\n| nil user!";
    return UidServer<>::kInvalidUid;
  }
  if (password == nullptr) {
    cout << "\n| nil password!";
    return UidServer<>::kInvalidUid;
  }
  cout << "\n| Comparing credentials \"" << user->GetHandle().GetKey()
       << "\":\"" << user->GetPassword().GetKey() << "\" to \"" << password
       << "\"";

  if (user->GetPassword().Equals(password)) {
    cout << "\n| Login successful :-)";
    // uid_t uid = uids_.GetNextUid ();

    return Random<uid_t>();
  }
  cout << "\n| Login unsuccessful.";
  return UidServer<>::kInvalidUid;
}

int UserList::Remove(int session) {
  if (session < 0) {
    return -1;
  }
  if (session >= Length()) {
    return -1;
  }
  User* user = users_[session];
  if (!user) {
    return -1;
  }
  delete user;
  users_[session] = nullptr;
  int num_users = num_users_;
  num_users_ = num_users - 1;
  return num_users;
}

int UserList::Remove(const char* handle) {
  int session = Find(handle);
  if (session < 0) {
    return session;
  }
  User* user = users_[session];
  delete user;
  users_[session] = nullptr;
  int num_users = num_users_;
  num_users_ = num_users - 1;
  return num_users;
}

Text& UserList::Print(Text& txt) {
  txt << "\n| UserList: Count:" << Length() << " Size:" << users_.capacity();

  for (int i = 0; i < Length(); i++) {
    User* user = users_[i];
    if (user) {
      txt << "\n| Account " << (i + 1) << ":\"" << user->GetHandle().GetKey()
          << '\"';
    }
  }
  return txt;
}

const char* UserList::Sudo(const char* text, const char* strand_end) {
  char handle[Handle::kMaxLength + 1], password[Password::kMaxLength + 1];
  double balance;
  int64_t value;
  const char* next_token;

  if (!text) {
    return nullptr;
  }
  if (text > strand_end) {
    return nullptr;
  }
  // This algorithm's job is to figure out white user to route the message
  // too. Users are not sorted right so we're doing it the slow way.
  // @todo Update to hash table.
  // @all routes messages to everyone in the UserList.
  if (next_token = TokenEquals(text, strand_end, "all")) {
    int count = Length();
    if (count == 0) {
      return next_token;
    }
    for (int i = count - 2; i > 0; --i) {
      if (!users_[i]->Sudo(next_token + 1, strand_end)) {
        return nullptr;
      }
    }
    return users_[0]->Sudo(next_token + 1, strand_end);
  }
  if (next_token = TokenEquals(text, strand_end, "add")) {
    next_token =
        TokenRead(text, strand_end, handle, handle + Handle::kMaxLength + 1);
    if (!next_token) {
      return nullptr;
    }
    next_token = TokenRead(text, strand_end, password,
                           password + Password::kMaxLength + 1);
    if (!next_token) {
      return nullptr;
    }
    if (!(next_token = TextRead(next_token + 1, strand_end, balance))) {
      return nullptr;
    }
    if (!(next_token = TextRead(next_token + 1, strand_end, value))) {
      return nullptr;
    }
    return next_token;
  }
  if (next_token = TokenEquals(text, strand_end, "remove")) {
    char handle[Handle::kMaxLength + 1];
    while (
        !TokenRead(text, strand_end, handle, handle + Handle::kMaxLength + 1)) {
      Remove(handle);
    }
  }
  for (int i = 0; i < Length(); ++i) {
    User* user = GetUser(i);
    next_token = TokenEquals(text, strand_end, user->GetHandle().GetKey());
    if (next_token) {
      return user->Sudo(next_token + 1, strand_end);
    }
  }
  return nullptr;
}

const Operation* UserList::Star(uint index, Expression* expr) {
  static const Operation This{"UserList", OperationCount(0),
                                 OperationFirst('A'),
                                 "A session handling user list.", 0};
  // void* args[1];
  switch (index) {
    case '?':
      return ExpressionOperation(expr, &This);
  }
  return nullptr;
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
