/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Password.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include "_Id/Password.hpp"

using namespace _Id;

Password::Password (const char* S = DefaultPassword, const char* aFormat = DefaultFormat)
{
    password = S;
}

void Password::Change (const char* newPassword)
{
    if (!IsValid (newPassword)) return;
    password = newPassword;
}

bool PasswordIsValid (const char* S)
{
    if (S.Length < MinLength || S.Length > MaxLength)
        return false;

    Regex r = new Regex ();
    if (r.IsMatch (S)) return true;

    return false;
}

const char* Password::Encript ()
{
    return password;
}

void Password::print (I2P::Terminal& slot)
{
    slot.prints (password);
}
