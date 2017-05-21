/** Kabuki Theater
    @file    /.../Source-Impl/_Id/Password.cpp
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

#include <_Id/Password.hpp>

#include <regex>

namespace _Id {

Password::Password (const string& s)
{
    change (s);
}

Password::Password (const string& s, Grammer& aFormat)
:   format (aFormat)
{
    change (s);
}

Grammer& Password::getGrammer ()
{
    return format;
}

void Password::setGrammer (const Grammer& g)
{
    format = g;
}

bool Password::change (const string& newPassword)
{
    bool valid = isValid (newPassword);
    if (!valid) return false;
    password = newPassword;
    return false;
}

bool Password::isValid (const string& S)
{
    if (S.length () < compare || S.length () > MaxLength)
        return false;

    //regex r;
    //if (r.isMatch (S)) return true;

    return false;
}

string Password::encript ()
{
    return password;
}

bool Password::equals (const string& s)
{
    return password == s;
}

bool Password::equals (const Password& p)
{
    return password == p.password;
}

void Password::print (Terminal& io)
{
    slot.prints (password.c_str ());
}

}   //  _Id
