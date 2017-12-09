/** kabuki::id
    @file    ~/source/kabuki/id/user.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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
 
#ifndef HEADER_FOR_KABUKI_ID_USER
#define HEADER_FOR_KABUKI_ID_USER

#include "handle.h"
#include "password.h"
#include "authenticator.h"

namespace kabuki { namespace id {

/** A user account on a computer.
    A user does not necessarily have an account. Some accounts are and any user
    can use. i.e. the guest account. */
class KABUKI User : _::Portal {
    public:

    enum {
        kDefaultMinDislpayNameLength = 2,  //< Default max display name length.
        kDefaultMaxDislpayNameLength = 63, //< Default max display name length.
        kDefaultValue                = 0,
    };

    /** Default User diplay_name_. */
    static const char kDefaultDislpayName[];

    static const double kDefaultBalance;

    /** Creates a user with the given handle, password, and status. */
    User (Authenticator* authenticator, uid_t uid = 0,
          const char* handle   = Handle::kDefault,
          const char* password = Password::kDefault,
          const char* status   = kDefaultDislpayName,
          double      balance  = kDefaultBalance,
          uint64_t    value    = kDefaultValue);

    /** Virtual destructor. */
    virtual ~User ();

    /** Gets the handle's key. */
    const char* GetStatus ();

    /** Gets the handle's key. */
    virtual const char* SetStatus (const char* name);

    /** Gets a reference to the char handle. */
    Handle& GetHandle ();

    /** Gets a reference to the password. */
    Password& GetPassword ();

    /** Gets the user's uid. */
    uid_t GetUid ();

    /** Gets the session uid. */
    int32_t GetSession ();

    /** Sets the session uid. */
    virtual const char* SetSession (int32_t session);

    /** Gets the session uid. */
    uid_t GetSessionKey ();

    /** Gets the abstract response code. */
    uid_t GetResponse ();

    /** Sets the abstract response code. */
    virtual const char* SetResponse (uid_t response);

    /** Gets the abstract balance. */
    double GetBalance ();

    /** Sets the abstract balance. */
    virtual const char* SetBalance (double balance);

    /** Attempts to buy the given points.
        @returns false if the balance_ is too low. */
    bool BuyCoins (uint64_t num_coins, double point_cost);

    /** Increase the balance_ by the given amount. */
    bool IncreaseBalance (double amount);

    /** Gets the abstract value. */
    uint64_t GetValue ();

    /** Sets the abstract value. */
    virtual const char* SetValue (uint64_t value);

    /** Returns true if this user is the same as the given one.  */
    bool Equals (User* user);

    /** Gets the Script Slot to send messages to this User. */
    _::Expression* GetSlot ();

    /** Gets the Script Slot to send messages to this User. */
    _::Window* GetWindow ();

    /** Returns true if this session and cypher match the same as the given
        one.  */
    bool IsAuthentic (int32_t session, uid_t session_key);

    /** Prints this object to a expression. */
    virtual void Print ();

    private:

    Handle         handle_;       //< User's handle (i.e. key).
    char*          status_;       //< User's status
    Password       password_;     //< User's password.
    Authenticator* authenticator_;//< Handle and Password authenticator.
    int32_t        session_;      //< Session index.
    uid_t          uid_,          //< Index of user in the UserList.
                   session_key_,  //< Fake session encryption key.
                   response_;     //< Gets user abstract response code.
    double         balance_;      //< Abstract user account balance or money.
    uint64_t       value_;        //< Abstract account value, points, or coins.
    _::Expression* slot_;         //< Portal to User's machine.

};      //< class User
}       //< namespace id
}       //< kabuki
#endif  //< HEADER_FOR_KABUKI_ID_USER
