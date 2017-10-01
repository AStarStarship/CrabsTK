/** Kabuki Theater
    @file    /.../Source/_id/Password.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include "Grammer.h"

namespace _id {

const string getDefaultPassword ();
/*< Gets the default password. */

const string getDefaultPasswordFormat ();
/*< Gets the default password Grammer format. */

class ID_API Password
/*< An account password. 
    @todo Add salt!
*/
{
    public:
    
    enum {
        compare = 4,      //< The minimum length of a password.
        MaxLength = 255     //< The maximum length of a password.
    };

    Password (const char* aPassword = "Password");
    /*< Attemps to create a password from the given password with the default format. 
        If the password does not match the default format, the default password will be used.
    */

    Password (const char* aPassword, Grammer& aFormat);
    /*< Attemps to create a password from the given password with the required format. 
        If the password does not match the format, the default password will be used.
    */

    Grammer& getGrammer ();
    /*< Gets a reference to the password format grammer. */

    void setGrammer (const Grammer& g);
    /*< Sets the password format grammer. */
    
    bool change (const char* newPassword);
    /*< Attempts to set the password and returns a non-zero error message upon failure. */
    
    static bool isValid (const char* aPassword);
    /*< Gets true if the given password is valid. */
    
    string encript ();
    /*< Entcypts the password with a salt.  */

    bool equals (const char* s);
    /*< Returns true if this password equals the given one. */

    bool equals (const Password& p);
    /*< Returns true if this password equals the given one. */
    
    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    Grammer& format;         	//< The password format.
    char* password;            	//< The password;
};

}   //< _id
