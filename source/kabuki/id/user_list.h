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
class KABUKI UserList : public _::Operand {   
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

    /** Peeks the next uid without incrementing it. */
    uid_t PeekNextUid ();

    /** Gets the number of users in the list. */
    int GetCount ();

    /** Gets the number of users in the list. */
    int GetSize ();

    /** Gets the point_cost_ */
    double GetPointCost ();

    /** Attempts to set the point cost. */
    bool SetPointCost (double point_cost);

    /** Attempts to buy the given points.
    @returns false if the balance_ is too low. */
    bool BuyValue (int session, uint64_t num_coins, double point_cost);

    /** Increase the balance_ by the given amount. */
    bool AddBalance (int session, double amount);

    /** Gets the authenticator. */
    Authenticator* GetAuthenticator ();

    /** Adds a new User to the list with the given handle and password. */
    int Register (const char* handle, const char* password = Password::kDefault,
                  double balance = User::kDefaultBalance,
                  int64_t value  = User::kDefaultValue);

    /** Adds a list of User (string) to the list. */
    int Register (UserList& users);

    /** Adds the given user and assumes control over the memory. */
    int Add (User* user);

    /** Finds an entity in the list by the given search char. */
    int Find (const char* string);

    /** Returns the requested user session.
        @return Returns nil if the session is invalid. */
    User* GetUser (int session);

    /** Unregisters the given handle from the list.
        @param handle   The handle of the user to delete.
        @param password The password of the user to delete.
        @return Returns the new users_ count or <0 upon failure. */
    int Unregister (const char* handle, const char* password);

    /** Attempts to login to the given session and returns a session key. */
    virtual uid_t LogIn (const char* handle, const char* password);

    /** Attempts to login to the given session and returns a session key. */
    virtual uid_t LogIn (int session, const char* password);

    /** Attempts to remove the given user_id.
        @return Returns negative upon failure and the user count upon
                success. */
    virtual int Remove (int user_id);

    /** Prints this object to the log. */
    void Print ();

    /** Handles Script Commands.
        @param text     Beginning of the Text buffer. 
        @param text_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* HandleText (const char* text,
                                    const char* text_end);
    
    /** An A*B abstract algebra Script Expression.
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);
    
    private:
    
    double             point_cost_;    //< Cost per point.
    Authenticator    * authenticator_; //< Handle, & Password Authenticator.
    std::vector<User*> users_;         //< User list.
    UidServer<>        uids_;          //< Unique Session Key server
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ID_USERLIST
