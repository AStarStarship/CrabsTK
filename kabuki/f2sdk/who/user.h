/* Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/user.h
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
#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_USER
#define HEADER_FOR_KT_ID_USER
#include "authenticator.h"
#include "handle.h"
#include "password.h"

namespace _ {

/* A user account on a computer.
    A user does not necessarily have an account. Some accounts are and any user
    can use. i.e. the guest account. */
class KABUKI User : Portal {
 public:
  enum {
    kDefaultMinDislpayNameLength = 2,  //< Default max display name length.
    kMaxStatusLength = 63,             //< Default max display name length.
    kDefaultValue = 0,                 //< Default abstract user value.
  };
  static const double kDefaultBalance;  //< Default account balance.

  /* Creates a user with the given handle, password, and status. */
  User(Authenticator* authenticator, uid_t uid = 0,
       const char* handle = Handle::kDefault,
       const char* password = Password::kDefault,
       double balance = kDefaultBalance, int64_t value = kDefaultValue);

  /* Constructor deep copies the other object. */
  User(const User& other);

  User& operator=(const User& other);

  /* Virtual destructor. */
  virtual ~User();

  /* Gets the handle's key. */
  const char* GetStatus();

  /* Gets the handle's key. */
  virtual const char* SetStatus(const char* name);

  /* Gets a reference to the handle_. */
  Handle& GetHandle();

  /* Gets a reference to the handle_.GetKey (). */
  const char* GetHandleKey();

  /* Gets a reference to the password. */
  Password& GetPassword();

  /* Gets the user's uid. */
  uid_t GetUid();

  /* Gets the session uid. */
  int32_t GetSession();

  /* Sets the session uid. */
  virtual const char* SetSession(int32_t session);

  /* Gets the session uid. */
  uid_t GetSessionKey();

  /* Gets the abstract response code. */
  uid_t GetResponse();

  /* Sets the abstract response code. */
  virtual const char* SetResponse(uid_t response);

  /* Gets the abstract balance. */
  double GetBalance();

  /* Sets the abstract balance. */
  virtual const char* SetBalance(double balance);

  /* Attempts to buy the given points.
      @returns false if the balance_ is too low. */
  bool BuyValue(int64_t num_coins, double point_cost);

  /* Increase the balance_ by the given amount. */
  bool AddBalance(double amount);

  /* Gets the abstract value. */
  int64_t GetValue();

  /* Increase the balance_ by the given amount. */
  int64_t AddValue(int64_t amount);

  /* Sets the abstract value. */
  virtual const char* SetValue(int64_t value);

  /* Returns true if this user is the same as the given one.  */
  bool Equals(User* user);

  /* Gets the Script Slot to send messages to this User. */
  Expression* GetSlot();

#if SCRIPT_USING_MIRROR
  /* Gets the Script Slot to send messages to this User. */
  Mirror* GetMirror();
#endif  //< SCRIPT_USING_MIRROR

  /* Returns true if this session and cypher match the same as the given
      one.  */
  bool IsAuthentic(int32_t session, uid_t public_key);

  /* Prints this object to a expression. */
  virtual Text& Out(Text& txt = Text());

  /* Handles Text input.
      @param text     Beginning of the Text buffer.
      @param strand_end End of the Text buffer.
      @return Returns nil upon success and an error string upon failure. */
  virtual const char* Sudo(const char* text, const char* strand_end);

  /* Abstract Script Operation(s).
      @param index The index of the expression.
      @param expr  The Expression to read and write from.
      @return      Returns null upon success, a Set header upon query, and an
                   error_t ticket upon Read-Write failure. */
  virtual const Operation* Star(uint index, Expression* expr);

 private:
  Handle handle_;                 //< User's handle (i.e. key).
  char* status_;                  //< User's status
  Password password_;             //< User's password.
  Authenticator* authenticator_;  //< Handle and Password authenticator.
  int32_t session_;               //< Session index.
  uid_t uid_,                     //< Index of user in the UserList.
      public_key_,                //< Fake session encryption key.
      response_;                  //< Gets user abstract response code.
  double balance_;                //< Abstract user account balance or money.
  int64_t value_;                 //< Abstract account value, points, or coins.
  Expression* slot_;           //< Portal to User's machine.

};  //< class User
}       // namespace _
#endif  //< HEADER_FOR_KT_ID_USER
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
