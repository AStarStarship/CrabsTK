/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/Password.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

namespace _Id
{
/**  */
class _KabukiSDK Password
{
    const int MinLength = 4,
        MaxLength = 12;

    const char* DefaultFormat = "^[a-zA-Z0-9]*$",
        DefaultPassword = "password";

    /** . */
    Password  (const char* aPassword = DefaultPassword, const char* aFormat = DefaultFormat)
    {
        password = aPassword;
    }
    
    /**  */
    void Change  (const char* newPassword)
    {
        if (!IsValid  (newPassword)) return;
        password = newPassword;
    }
    
    /** Gets true if the given password is valid. */
    static bool IsValid  (const char* aPassword)
    {
        if  (aPassword.Length < MinLength || aPassword.Length > MaxLength)
            return false;

        Regex r = new Regex ();
        if  (r.IsMatch (aPassword)) return true;

        return false;
    }
    
    /**  */
    const char* Encript ()
    {
        return password;
    }
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */
    
    /**  */
    const char* print (I2P::Terminal& slot)
    {
        return password;
    }

    const char* rules,
        password;
};
}
