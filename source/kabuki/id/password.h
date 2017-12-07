/** kabuki::id
    @file    ~/source/kabuki/id/password.h
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

#ifndef HEADER_FOR_KABUKI_ID_PASSWORD
#define HEADER_FOR_KABUKI_ID_PASSWORD

#include "validator.h"

namespace kabuki { namespace id {

/** Interface for a class that can validate a password. */
struct PasswordValidator {
    /** Function validates the password for correctness. */
    virtual const char* IsValid (const char* password) = 0;
};

/** An account password Validator . 
    @todo Add salt!
*/
class KABUKI Password {
    public:
    
    enum {
        kValidation       = 2,   //< Validation type.
        kDefaultMinLength = 4,   //< The minimum length of a password.
        kDefaultMaxLength = 255, //< The maximum length of a password.
    };

    static const char kDefault[]; //< Default password.

    /** Attempts to create a password from the given password with the default
        format. 
        If the password does not match the default format, the default password
        will be used.
    */
    Password (Validator* validator,
              const char* password = "Password");

    /** Destructs the password. */
    ~Password ();

    /** Gets the password key. */
    const char* GetKey ();

    /** Attempts to set the password and returns a non-zero error message upon failure. */
    bool SetKey (const char* password);

    /** Gets true if the given password is valid. */
    static bool IsValid (const char* password);

    /** Returns true if this password equals the given one. */
    bool Equals (const char* string);

    /** Returns true if this password equals the given one. */
    bool Equals (const Password& p);

    /** Prints this object to a expression. */
    void Print ();

    private:

    char     * key_;       //< Unencrypted password.
    Validator* validator_; //< Password validator.
    int        type_;      //< The validation type.
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ID_PASSWORD
