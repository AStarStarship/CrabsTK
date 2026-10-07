// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_USERLIST_DECL_DECL
#define CRABSTK_WHO_USERLIST_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#include "AuthenticatorDefault.hpp"
#include "UIDAssigner.hpp"
#include "User.hpp"
namespace _ {

/* A List of User(s).
This class uses a single Authenticator interface for validating both user
handles and passwords using the type parameter. This makes it very
easy to create UserList subclasses for customizing format rules.
    
Users may login and out. When a User logs out the pointer and class gets
deleted. */
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
           ISC max_users = kDefaultMaxUsers)
    : user_count (0),
    value_cost_ (0.0),
    auth_ (authenticator),
    users_ (max_users < 1 ? 1 : max_users) {
    for (ISC i = GetSize () - 1; i > 0; --i) {
      users_[i] = nullptr;
    }
  }

  /* Destructs list of users. */
  virtual ~UserList() {
    ISC count = Length ();
    if (count == 0) {
      return;
    }
    for (ISC i = count - 1; i > 0; --i) {
      TUser* user = users_[Length () - 1];
      users_.pop_back ();
      delete user;
    }
  }

  /* Peeks the next uid without incrementing it. */
  IUD PeekNextUid() { return uids_.PeekNextUid (); }

  /* Gets the number of users in the list. */
  ISC Length() { return user_count; }

  /* Gets the number of users in the list. */
  ISC GetSize() { return users_.size (); }

  /* Gets the point_cost_ */
  FPD GetValueCost() { return value_cost_; }

  /* Attempts to set the point cost. */
  BOL SetValueCost(FPD value) {
    if (value < 0.0) {
      return false;
    }
    value_cost_ = point_cost;
    return true;
  }

  /* Attempts to buy the given points.
      @returns false if the balance_ is too low. */
  BOL BuyValue(ISC session, uint64_t num_coins, FPD value_cost) {
    TUser* user = GetUser (session);
    if (!user) {
      return false;
    }
    return users_[session]->BuyValue (num_coins, point_cost);
  }

  /* Increase the balance_ by the given amount. */
  BOL AddBalance(ISC session, FPD amount) {
    TUser* user = GetUser (session);
    if (!user) {
      return false;
    }
    return users_[session]->AddBalance (amount);
  }

  /* Gets the authenticator. */
  TAuthenticator* GetAuthenticator() { return auth_; }

  /* Adds a new User to the list with the given handle and password. */
  ISC Add(const TString<>& handle, const TString<>& password = TPassword::kDefault,
          FPD balance = TUser::kDefaultBalance,
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
      password = StringClone (TPassword::kDefault);
    }
    if (auth_->PasswordInvalid (password)) {
      cout << "\n| Invalid password!";
      return -1;
    }
    return Add (new TUser (auth_, uids_.GetNextUid (), handle, password,
      balance, value));
  }

  /* Adds a list of User (AString) to the list.
  ISC Add (UserList& user_list) {
    ISC other_num_users = user_list.user_count,
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
    for (ISC i = 0; i < other_num_users; ++i) {
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
  ISC Add(TUser* user) {
    if (!user) {
      return -1;
    }
    ISC num_users = user_count;
    if (num_users >= GetSize ()) {
      cout << "\n| Error: UserList is full!";
      return -1;
    }
    // Look for the first empty spot in the list.
    for (ISC i = Length () - 1; i > 0; --i) {
      if (!users_[i]) {
        users_[i] = user;
        user_count = num_users + 1;
        return i;
      }
    }
    return -1;
  }

  /* Finds an entity in the list by the given search CHA. */
  ISC Find(const TString<>& AString) {
    if (handle == nullptr) {
      return -1;
    }
    // if (*handle == 0) { //< It's an empty AString.
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
  ISC Remove(const TString<>& handle, const TString<>& password) {
    ISC session = Find (handle);
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
  User* GetUser(ISC session) {
    if (session < 0) {
      return nullptr;
    }
    if (session >= Length ()) {
      return nullptr;
    }
    return users_[session];
  }

  /* Attempts to login to the given session and returns a session key. */
  virtual IUD LogIn(const TString<>& handle, const TString<>& password) {
    return LogIn (Find (handle), password);
  }

  /* Attempts to login to the given session and returns a session key. */
  virtual IUD LogIn(ISC session, const TString<>& password) {
    cout << "\n| Attempting to log in as " << index << "\"" << password << "\"";
    User* user = GetUser (index);
    if (!user) {
      cout << "\n| nil user!";
      return UidServer<>::cInvalidUid;
    }
    if (password == nullptr) {
      cout << "\n| nil password!";
      return UidServer<>::cInvalidUid;
    }
    cout << "\n| Comparing credentials \"" << user->GetHandle ().GetKey ()
      << "\":\"" << user->GetPassword ().GetKey () << "\" to \"" << password
      << "\"";

    if (user->GetPassword ().Equals (password)) {
      cout << "\n| Login successful :-)";
      // IUD uid = uids_.GetNextUid ();

      return Random<IUD> ();
    }
    cout << "\n| Login unsuccessful.";
    return UidServer<>::cInvalidUid;
  }

  /* Attempts to remove the given session.
  @param  session Index of the User to remove
  @return Returns negative upon failure and the user count upon success. */
  virtual ISC Remove(ISC session) {
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
    ISC num_users = user_count;
    user_count = num_users - 1;
    return num_users;
  }

  /* Attempts to remove the given session.
  @param  handle The handle of the User to remove.
  @return Returns negative upon failure and the user count upon success. */
  virtual ISC Remove(const TString<>& handle) {
    ISC session = Find (handle);
    if (session < 0) {
      return session;
    }
    User* user = users_[session];
    delete user;
    users_[session] = nullptr;
    ISC num_users = user_count;
    user_count = num_users - 1;
    return num_users;
  }

  /* Prints this object to the log. */
  template<typename Printer> Printer& Print (Printer& o) {
    o << "\n| UserList: Count:" << Length () << " Size:" << users_.capacity ();

    for (ISC i = 0; i < Length (); i++) {
      User* user = users_[i];
      if (user) {
        o << "\n| Account " << (i + 1) << ":\"" << user->GetHandle ().GetKey ()
          << '\"';
      }
    }
    return txt;
  }
  
  /*const TString<>& Sudo(const TString<>& text, const TString<>& String_end) {
    CHA handle[Handle::kMaxLength + 1], password[Password::kMaxLength + 1];
    FPD balance;
    int64_t value;
    const TString<>& next_token;

    if (!text) {
      return nullptr;
    }
    if (text > String_end) {
      return nullptr;
    }
    // This algorithm's job is to figure out white user to route the message
    // too. Users are not sorted right so we're doing it the slow way.
    // @todo Update to hash table.
    // @all routes messages to everyone in the UserList.
    if (next_token = TokenEquals(text, String_end, "all")) {
      ISC count = Length();
      if (count == 0) {
        return next_token;
      }
      for (ISC i = count - 2; i > 0; --i) {
        if (!users_[i]->Sudo(next_token + 1, String_end)) {
          return nullptr;
        }
      }
      return users_[0]->Sudo(next_token + 1, String_end);
    }
    if (next_token = TokenEquals(text, String_end, "add")) {
      next_token =
          TokenRead(text, String_end, handle, handle + Handle::kMaxLength + 1);
      if (!next_token) {
        return nullptr;
      }
      next_token = TokenRead(text, String_end, password,
                             password + Password::kMaxLength + 1);
      if (!next_token) {
        return nullptr;
      }
      if (!(next_token = TextRead(next_token + 1, String_end, balance))) {
        return nullptr;
      }
      if (!(next_token = TextRead(next_token + 1, String_end, value))) {
        return nullptr;
      }
      return next_token;
    }
    if (next_token = TokenEquals(text, String_end, "remove")) {
      CHA handle[Handle::kMaxLength + 1];
      while (
          !TokenRead(text, String_end, handle, handle + Handle::kMaxLength + 1)) {
        Remove(handle);
      }
    }
    for (ISC i = 0; i < Length(); ++i) {
      User* user = GetUser(i);
      next_token = TokenEquals(text, String_end, user->GetHandle().GetKey());
      if (next_token) {
        return user->Sudo(next_token + 1, String_end);
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
  ISC user_count;         //< Number of users logged in.
  FPD value_cost_;        //< Cost per point.
  TAuthenticator* auth_;  //< Handle, & Password Authenticator.
  TArray<User*> users_;   //< User list.
  UidServer<> uids_;      //< Unique Session Key server
};

}       //< namespace _
#endif
#endif
