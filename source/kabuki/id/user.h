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

namespace kabuki { namespace id {

/** A user account on a computer.
    A user does not necessarily have an account. Some accounts are and any user
    can use. i.e. the guest account. */
class KABUKI User {
    public:

    enum {
        kValidation                  = 0,  //< Validator index.
        kDefaultMinDislpayNameLength = 2,  //< Default max display name length.
        kDefaultMaxDislpayNameLength = 63, //< Default max display name length.
    };

    /** Default User diplay_name_. */
    static const char kDefaultDislpayName[];

    /** Creates a user with the given char and password. */
    User (Validator* validator, const char* display_name, const char* handle,
          const char* password);

    /** Gets the handle's key. */
    const char* GetDisplayName ();

    /** Gets the handle's key. */
    const char* SetDisplayName (const char* name);

    /** Gets a reference to the char handle. */
    Handle& GetHandle ();

    /** Gets a reference to the password. */
    Password& GetPassword ();

    /** Gets the session uid. */
    uid_t GetSession ();

    /** Sets the session uid. */
    void SetSession (uid_t uid);

    /** Gets the session key. */
    uid_t GetSessionKey ();

    /** Sets the session key. */
    void SetSessionKey (uid_t jey);

    /** Checks to see if the given char and password are in the correct format. */
    virtual bool IsValid (const char* dislpay_name, const char* handle,
                          const char* password);

    /** Returns true if this user is the same as the given one.  */
    bool Equals (const User& user);

    /** Prints this object to a expression. */
    void Print ();

    private:

    Handle     handle_;       //< User's handle (i.e. key).
    char*      display_name_; //< User's display name.
    Password   password_;     //< User's password.
    Validator* validator_;    //< Handle and Password validator.
    uid_t      session_,      //< Session uid.
               session_key_;  //< Session key for the session_.

};      //< class User
}       //< namespace id
}       //< kabuki
#endif  //< HEADER_FOR_KABUKI_ID_USER
