/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_userlist.h
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
#ifndef KABUKI_TOOLKIT_WHO_USERLIST
#define KABUKI_TOOLKIT_WHO_USERLIST
#include "t_authenticatordefault.h"
#include "t_uidserver.h"
#include "t_user.h"

namespace _ {

/* A List of User(s).
This class uses a single Authenticator interface for validating both user
handles and passwords using the type parameter. This makes it very
easy to create UserList subclasses for customizing format rules.
    
Users may login and out. When a User logs out the pointer and class gets
deleted.
*/
class UserList : public Operand {
 public:
  enum {
    kDefaultMaxUsers = 1024,
    kPasswordValidation = 0,
    kHandleValidation = 1,
    kMinPasswordLength = 8,
    kMaxPasswordLength = 63,
    kMinHandleLength = 2,
    kMaxHandleLength = 63,
  };

  /* Creates an empty list. */
  UserList(TAuthenticator* authenticator = new TAuthenticatorDefault(),
           SI4 max_users = kDefaultMaxUsers)
    : user_count (0),
    value_cost_ (0.0),
    auth_ (authenticator),
    users_ (max_users < 1 ? 1 : max_users) {
    for (SI4 i = GetSize () - 1; i > 0; --i) {
      users_[i] = nullptr;
    }
  }

  /* Destructs list of users. */
  virtual ~UserList() {
    SI4 count = Length ();
    if (count == 0) {
      return;
    }
    for (SI4 i = count - 1; i > 0; --i) {
      TUser* user = users_[Length () - 1];
      users_.pop_back ();
      delete user;
    }
  }

  /* Peeks the next uid without incrementing it. */
  UID PeekNextUid() { return uids_.PeekNextUid (); }

  /* Gets the number of users in the list. */
  SI4 Length() { return user_count; }

  /* Gets the number of users in the list. */
  SI4 GetSize() { return users_.size (); }

  /* Gets the point_cost_ */
  DBL GetValueCost() { return value_cost_; }

  /* Attempts to set the point cost. */
  BOL SetValueCost(DBL value) {
    if (value < 0.0) {
      return false;
    }
    value_cost_ = point_cost;
    return true;
  }

  /* Attempts to buy the given points.
      @returns false if the balance_ is too low. */
  BOL BuyValue(SI4 session, uint64_t num_coins, DBL value_cost) {
    TUser* user = GetUser (session);
    if (!user) {
      return false;
    }
    return users_[session]->BuyValue (num_coins, point_cost);
  }

  /* Increase the balance_ by the given amount. */
  BOL AddBalance(SI4 session, DBL amount) {
    TUser* user = GetUser (session);
    if (!user) {
      return false;
    }
    return users_[session]->AddBalance (amount);
  }

  /* Gets the authenticator. */
  TAuthenticator* GetAuthenticator() { return auth_; }

  /* Adds a new User to the list with the given handle and password. */
  SI4 Add(const TStrand<>& handle, const TStrand<>& password = TPassword::kDefault,
          DBL balance = TUser::kDefaultBalance,
          int64_t value = TUser::kDefaultValue) {
    cout << "\n| Attempting to add @" << ((handle == nullptr) ? "null" : handle)
      << ":\"" << ((password == nullptr) ? "null" : password) << '\"';
    if (handle == nullptr) {
      return -1;
    }
    if (!auth_->HandleIsValid (handle)) {
      cout << "\n| Invalid handle!";
      return -1;
    }
    if (Find (handle) >= 0) {
      cout << "\n| UserList contains handle!";
      return -1;
    }
    if (password == nullptr) {
      password = StrandClone (TPassword::kDefault);
    }
    if (auth_->PasswordInvalid (password)) {
      cout << "\n| Invalid password!";
      return -1;
    }
    return Add (new TUser (auth_, uids_.GetNextUid (), handle, password,
      balance, value));
  }

  /* Adds a list of User (string) to the list.
  SI4 Add (UserList& user_list) {
    SI4 other_num_users = user_list.user_count,
      num_users = user_count,
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
    for (SI4 i = 0; i < other_num_users; ++i) {
      User* user = user_list.users_[i];
      // Look for the first empty spot in the list.
      for (; last_index > size; --i) {
        if (!users_[i]) {
          users_[i] = user;
          break;
        }
      }
    }
    user_count = num_users + other_num_users;
    return other_num_users - 1;
  }*/

  /* Adds the given user and assumes control over the memory. */
  SI4 Add(TUser* user) {
    if (!user) {
      return -1;
    }
    SI4 num_users = user_count;
    if (num_users >= GetSize ()) {
      cout << "\n| Error: UserList is full!";
      return -1;
    }
    // Look for the first empty spot in the list.
    for (SI4 i = Length () - 1; i > 0; --i) {
      if (!users_[i]) {
        users_[i] = user;
        user_count = num_users + 1;
        return i;
      }
    }
    return -1;
  }

  /* Finds an entity in the list by the given search CH1. */
  SI4 Find(const TStrand<>& string) {
    if (handle == nullptr) {
      return -1;
    }
    // if (*handle == 0) { //< It's an empty string.
    //    return -1;      //  Not sure if I care about this or not.
    //}
    // Currently using sequential search because UserList is not sorted.
    size_t count = Length ();
    cout << "\n| Searching for handle:\"" << handle << "\" cout:" << count;
    if (count == 0) {
      return -1;
    }
    for (size_t i = 0; i < count; ++i) {
      User* user = users_[i];
      cout << "\n| " << i << ' ';
      cout << user->Print ();
      if (users_[i]->GetHandle ().Equals (handle)) {
        cout << "\n| Found handle at index:" << i;
        return i;
      }
    }

    cout << "\n| Did not find handle.";
    return -1;
  }

  /* Unregisters the given handle from the list.
  @param handle   The handle of the user to delete.
  @param password The password of the user to delete.
  @return Returns the new users_ count or <0 upon failure. */
  SI4 Remove(const TStrand<>& handle, const TStrand<>& password) {
    SI4 session = Find (handle);
    if (session < 0) {
      return -1;
    }
    if (!GetUser (session)->GetPassword ().Equals (handle)) {
      return -1;
    }
    User* user = users_[session];
    delete user;
    users_.erase (users_.begin () + session);
    --user_count;
    return Length ();
  }

  /* Returns the requested user session.
  @return Returns nil if the session is invalid. */
  User* GetUser(SI4 session) {
    if (session < 0) {
      return nullptr;
    }
    if (session >= Length ()) {
      return nullptr;
    }
    return users_[session];
  }

  /* Attempts to login to the given session and returns a session key. */
  virtual UID LogIn(const TStrand<>& handle, const TStrand<>& password) {
    return LogIn (Find (handle), password);
  }

  /* Attempts to login to the given session and returns a session key. */
  virtual UID LogIn(SI4 session, const TStrand<>& password) {
    cout << "\n| Attempting to log in as " << index << "\"" << password << "\"";
    User* user = GetUser (index);
    if (!user) {
      cout << "\n| nil user!";
      return UidServer<>::kInvalidUid;
    }
    if (password == nullptr) {
      cout << "\n| nil password!";
      return UidServer<>::kInvalidUid;
    }
    cout << "\n| Comparing credentials \"" << user->GetHandle ().GetKey ()
      << "\":\"" << user->GetPassword ().GetKey () << "\" to \"" << password
      << "\"";

    if (user->GetPassword ().Equals (password)) {
      cout << "\n| Login successful :-)";
      // UID uid = uids_.GetNextUid ();

      return Random<UID> ();
    }
    cout << "\n| Login unsuccessful.";
    return UidServer<>::kInvalidUid;
  }

  /* Attempts to remove the given session.
  @param  session Index of the User to remove
  @return Returns negative upon failure and the user count upon success. */
  virtual SI4 Remove(SI4 session) {
    if (session < 0) {
      return -1;
    }
    if (session >= Length ()) {
      return -1;
    }
    User* user = users_[session];
    if (!user) {
      return -1;
    }
    delete user;
    users_[session] = nullptr;
    SI4 num_users = user_count;
    user_count = num_users - 1;
    return num_users;
  }

  /* Attempts to remove the given session.
  @param  handle The handle of the User to remove.
  @return Returns negative upon failure and the user count upon success. */
  virtual SI4 Remove(const TStrand<>& handle) {
    SI4 session = Find (handle);
    if (session < 0) {
      return session;
    }
    User* user = users_[session];
    delete user;
    users_[session] = nullptr;
    SI4 num_users = user_count;
    user_count = num_users - 1;
    return num_users;
  }

  /* Prints this object to the log. */
  template<typename Printer> Printer& Print (Printer& o) {
    o << "\n| UserList: Count:" << Length () << " Size:" << users_.capacity ();

    for (SI4 i = 0; i < Length (); i++) {
      User* user = users_[i];
      if (user) {
        o << "\n| Account " << (i + 1) << ":\"" << user->GetHandle ().GetKey ()
          << '\"';
      }
    }
    return txt;
  }
  
  /*const TStrand<>& Sudo(const TStrand<>& text, const TStrand<>& strand_end) {
    CH1 handle[Handle::kMaxLength + 1], password[Password::kMaxLength + 1];
    DBL balance;
    int64_t value;
    const TStrand<>& next_token;

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
      SI4 count = Length();
      if (count == 0) {
        return next_token;
      }
      for (SI4 i = count - 2; i > 0; --i) {
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
      CH1 handle[Handle::kMaxLength + 1];
      while (
          !TokenRead(text, strand_end, handle, handle + Handle::kMaxLength + 1)) {
        Remove(handle);
      }
    }
    for (SI4 i = 0; i < Length(); ++i) {
      User* user = GetUser(i);
      next_token = TokenEquals(text, strand_end, user->GetHandle().GetKey());
      if (next_token) {
        return user->Sudo(next_token + 1, strand_end);
      }
    }
    return nullptr;
  }*/
  /* An A*B abstract algebra Script Expr.
  @param index The index of the expression.
  @param expr  The Expr to read and write from.
  @return      Returns null upon success, a Set header upon query, and an
  error_t ticket upon Read-Write failure. */
  virtual const Operation* Star(uint index, Expr* expr) {
    static const Operation This{ "UserList", OperationCount (0),
                                   OperationFirst ('A'),
                                   "A session handling user list.", 0 };
    // void* args[1];
    switch (index) {
    case '?':
      return ExpressionOperation (expr, &This);
    }
    return nullptr;
  }

 private:
  SI4 user_count;         //< Number of users logged in.
  DBL value_cost_;        //< Cost per point.
  TAuthenticator* auth_;  //< Handle, & Password Authenticator.
  TArray<User*> users_;   //< User list.
  UidServer<> uids_;      //< Unique Session Key server
};

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_USERLIST
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
