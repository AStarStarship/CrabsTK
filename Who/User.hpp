// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_USER_DECL_DECL
#define CRABSTK_WHO_USER_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#include "Authenticator.hpp"
#include "Handle.hpp"
#include "Password.hpp"
namespace _ {

/* A user account on a computer.
A user does not necessarily have an account. Some accounts are and any user
can use. i.e. the guest account. */
class TUser {
 public:
  enum {
    kDefaultMinDislpayNameLength = 2,  //< Default max display name length.
    kMaxStatusLength = 63,             //< Default max display name length.
    kDefaultValue = 0,                 //< Default abstract user value.
  };
  static const FPD kDefaultBalance;  //< Default account balance.

  /* Creates a user with the given handle, password, and status. */
  TUser(TAuthenticator* authenticator, IUD uid = 0,
       const TString<>& handle = THandle::kDefault,
       const TString<>& password = TPassword::kDefault,
       FPD balance = kDefaultBalance, int64_t v-alue = kDefaultValue)
    : handle_ (authenticator, handle),
    status_ (StringClone ("")),
    password_ (authenticator, password),
    auth_ (authenticator),
    uid_ (uid),
    session_ (0),
    public_key_ (0),
    balance_ (balance),
    value_ (value),
    slot_ (nullptr) {}

  /* Constructor deep copies the other object. */
  TUser(const TUser& other)
    : handle_ (other.handle_),
    status_ (other.status_),
    password_ (other.password_),
    auth_ (other.auth_),
    session_ (other.session_),
    uid_ (other.uid_),
    public_key_ (other.public_key_),
    response_ (other.response_),
    balance_ (other.balance_),
    value_ (other.value_),
    slot_ (other.slot_) {}

  TUser& operator=(const TUser& other) {
    handle_ = other.handle_;
    status_ = other.status_;
    password_ = other.password_;
    auth_ = other.auth_;
    session_ = other.session_;
    uid_ = other.uid_;
    public_key_ = other.public_key_;
    response_ = other.response_;
    balance_ = other.balance_;
    value_ = other.value_;
    slot_ = other.slot_;
    return *this;
  }

  /* Virtual destructor. */
  virtual ~TUser() {}

  /* Gets the handle's key. */
  const TString<>& GetStatus() { return status_; }

  /* Gets the handle's key. */
  virtual const TString<>& SetStatus(const TString<>& name) {
    if (name == nullptr) {
      return "name can't be nil.";
    }
    status_ = StringClone (name);
    return nullptr;
  }

  /* Gets a reference to the handle_. */
  THandle& GetHandle() { return handle_; }

  /* Gets a reference to the handle_.GetKey (). */
  const TString<>& GetHandleKey() { return handle_.GetKey (); }

  /* Gets a reference to the password. */
  TPassword& GetPassword() { return password_; }

  /* Gets the user's uid. */
  IUD GetUid() { return uid_; }

  /* Gets the session uid. */
  int32_t GetSession() { return session_; }

  /* Sets the session uid. */
  virtual const TString<>& SetSession(int32_t session) {
    if (session == 0) return "Invalid session key";
    session_ = session;
    return nullptr;
  }

  /* Gets the session uid. */
  IUD GetSessionKey();

  /* Gets the abstract response code. */
  IUD GetResponse();

  /* Sets the abstract response code. */
  virtual const TString<>& SetResponse(IUD response) {
    response_ = response;
    return nullptr;
  }

  /* Gets the abstract balance. */
  FPD GetBalance() { return balance_; }

  /* Sets the abstract balance. */
  virtual const TString<>& SetBalance(FPD balance) {
    balance_ = balance;
    return nullptr;
  }

  /* Attempts to buy the given points.
      @returns false if the balance_ is too low. */
  BOL BuyValue(int64_t num_coins, FPD point_cost) {
    // Right now we're not checking how much money the player has.
    FPD cost = point_cost * (FPD)num_coins;

    if (cost > balance_) {
      cout << "\n| Can't buy coins because your ass is broke.";
      return false;
    }
    value_ += num_coins;
    balance_ -= cost;
    return true;
  }

  /* Increase the balance_ by the given amount. */
  BOL AddBalance(FPD amount) {
    FPD balance = balance_;
    balance_ = balance + amount;
    return balance;
  }

  /* Gets the abstract value. */
  int64_t GetValue() { return value_; }

  /* Increase the balance_ by the given amount. */
  int64_t AddValue(int64_t amount) {
    int64_t value = value_;
    value_ = value + amount;
    return value;
  }

  /* Sets the abstract value. */
  virtual const TString<>& SetValue(int64_t value) {
    value_ = value;
    return nullptr;
  }

  /* Returns true if this user is the same as the given one.  */
  BOL Equals(TUser* user) {
    if (user == nullptr) {
      return false;
    }
    if (!handle_.Equals (handle_.GetKey ())) {
      return false;
    }
    return password_.Equals (user->password_);
  }

  /* Gets the Script Slot to send messages to this User. */
  Expr* GetSlot() { return slot_; }

#if SCRIPT_USING_MIRROR
  /* Gets the Script Slot to send messages to this User. */
  Mirror* GetMirror();
#endif  //< SCRIPT_USING_MIRROR

  /* Returns true if this session and cypher match the same as the given
      one.  */
  BOL IsAuthentic(int32_t session, IUD public_key) {
    if (session_ != session) {
      return false;
    }
    if (public_key_ != public_key) {
      return false;
    }
    return true;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
    Printer& Print (Printer& o) {
    return o << "\n| User: @" << handle_.GetKey () << " password:\""
             << password_.GetKey () << "\" balance:" << balance_
             << " value:" << value_;
  }

  /* @todo Convert to Operation.
  const TString<>& Sudo(const TString<>& text, const TString<>& String_end) {
    enum {
      kMessageLength = 141,
    };
    const TString<>& token;
    FPD balance;
    int64_t value;
    CHA input[kMessageLength];

    text = TextSkipSpaces(text, String_end);
    if (!text) {
      return nullptr;
    }
    if (TokenCompare(text, String_end, "AddBalance")) {
      token = TextRead(text, String_end, balance);
      if (!token) {
        return nullptr;
      }
      balance_ += balance;
    } else if (TokenCompare(text, String_end, "AddValue")) {
      token = TextRead(text, String_end, value);
      if (!token) {
        return nullptr;
      }
      value_ += value;
    } else if (TokenCompare(text, String_end, "SetStatus")) {
      token = TextRead(text, String_end, input, input + kMessageLength);
      if (!token) {
        return nullptr;
      }
      SetStatus(input);
    } else if (TokenCompare(text, String_end, "SetHandle")) {
      token = TextRead(text, String_end, input, input + kMessageLength);
      if (!token) {
        return nullptr;
      }
      if (!auth_->HandleIsValid(input)) {
        return "\n| Error: Password in invalid format!";
      }
      handle_.SetKey(input);
    } else if (TokenCompare(text, String_end, "SetPassword")) {
      token = TextRead(text, String_end, input, input + 141);
      if (!token) {
        return nullptr;
      }
      if (auth_->PasswordInvalid(input)) {
        return "\n| Error: Password in invalid format!";
      }
      password_.SetKey(input);
    } else if (TokenCompare(text, String_end, "print")) {
      cout << Print();
      return text;
    }

    token = TextRead(text, String_end, input, input + kMessageLength);
    if (!token) {
      return nullptr;
    }
    return token;
  }*/

  /* Abstract Script Operation(s).
  @param index The index of the expression.
  @param expr  The Expr to read and write from.
  @return      Returns null upon success, a Set header upon query, and an
  error_t ticket upon Read-Write failure. */
  virtual const Op* Star(uint index, Expr* expr) {
    static const Op cThis{ "User", OperationCount (0), OperationFirst ('A'),
                         "A user account.", 0 };
    switch (index) {
    case '?':
      return ExpressionOperand (expr, &cThis);
    }
    return nullptr;
  }

 private:
  THandle handle_;        //< User's handle (i.e. key).
  TString<>& status_;     //< User's status
  TPassword password_;    //< User's password.
  TAuthenticator* auth_;  //< Handle and Password authenticator.
  int32_t session_;       //< Session index.
  IUD uid_,               //< Index of user in the UserList.
      public_key_,        //< Fake session encryption key.
      response_;          //< Gets user abstract response code.
  FPD balance_;           //< Abstract user account balance or money.
  int64_t value_;         //< Abstract account value, points, or coins.
  Expr* slot_;            //< Portal to User's machine.

};
}       //< namespace _
#endif
#endif
