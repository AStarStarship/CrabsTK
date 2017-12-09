/** kabuki::id
    @file    ~/source/kabuki/id/user_hist.h
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
 
#ifndef HEADER_FOR_KABUKI_ID_USERLIST
#define HEADER_FOR_KABUKI_ID_USERLIST

#include "authenticator_default.h"
#include "user.h"
#include "uid_server.h"

namespace kabuki { namespace id {

/** A List of User(s).
    This class uses a single Authenticator interface for validating both user 
    handles and passwords using the type parameter. This makes it very 
    easy to create UserList subclasses for customizing format rules.
    @todo This class needs to use a Script Dictionary. */
class KABUKI UserList {   
    public:

    enum {
        kDefaultMaxUsers    = 1024,
        kPasswordValidation = 0,
        kHandleValidation   = 1,
        kMinPasswordLength  = 8,
        kMaxPasswordLength  = 63,
        kMinHandleLength    = 2,
        kMaxHandleLength    = 63,
    };

    /** Creates an empty list. */
    UserList (Authenticator* authenticator = new AuthenticatorDefault (),
              int max_users = kDefaultMaxUsers);

    /** Destructs list of users. */
    virtual ~UserList ();

    /** Gets the authenticator. */
    Authenticator* GetAuthenticator ();

    /** Gets the number of Accounts in the List. */
    int GetSize ();

    /** Gets the count of the User(s). */
    int GetCount ();

    /** Adds a new User to the list with the given handle and password. */
    int Add (const char* handle, const char* password = Password::kDefault);

    /** Adds a list of User (string) to the list. */
    int Add (UserList* enities);

    /** Finds an entity in the list by the given search char. */
    int Find (const char* string);

    /** Returns the requested user_number.
        @return Returns nil if the user_number is invalid. */
    User* GetUser (uid_t user_number);

    /** Validates the input for correctness. */
    virtual const char* IsValid (const char* input, int type);

    /** Attempts to login to the given session and returns a session key. */
    virtual uid_t LogIn (const char* handle, const char* password);

    /** Attempts to login to the given session and returns a session key. */
    virtual uid_t LogIn (int session, const char* password);

    /** Prints this object to the log. */
    void Print ();
    
    private:
    
    Authenticator* authenticator_; //< Handle, & Password Authenticator.
    Array<User*>   users_;         //< User list.
    UidServer<>    uids_;          //< Unique Id Server
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ID_USERLIST
