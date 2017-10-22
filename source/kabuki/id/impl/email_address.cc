/** Kabuki Starship
    @file    /.../Source-Impl/_Id/EmailAddress.cpp
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

#include <_Id/EmailAddress.h>

namespace _Id {

bool isValidEmailAddress (const string& s)
{
    /*
    invalid = false;
    if (s.length () < 6) return false;  //< Min email length is "a@b.c"

    /// Use IdnMapping class to convert Unicode domain names.
    try
    {
    s = regex_replace (a, " (@) (.+)$", this.DomainMapper, RegexOptions.None, TimeSpan.FromMilliseconds (200));
    }
    catch (RegexMatchTimeoutException) { return false; }

    if (invalid) return false;

    // Return true if a is in valid e-mail format.
    try
    {
    return Regex.IsMatch (a,
    "^ (? ("") ("".+? (?<!\\)""@)| (([0-9a-z] ((\. (?!\.))|[-!#\$%&'\*\+/=\?\^`\{\}\|~\w])*) (?<=[0-9a-z])@))" +
    " (? (\[) (\[ (\d{1,3}\.){3}\d{1,3}\])| (([0-9a-z][-\w]*[0-9a-z]*\.)+[a-z0-9][\-a-z0-9]{0,22}[a-z0-9]))$",
    RegexOptions.IgnoreCase, TimeSpan.FromMilliseconds (250));
    }
    catch (RegexMatchTimeoutException) { return false; }
    */
    return true;
}

EmailAddress::EmailAddress (const string& s)
{

}

string& EmailAddress::getAddress () { return address; }

void EmailAddress::setAddress (const string& s) { address = s; }

/* Old C# code to rewrite: 
string EmailAddressmapToDomain (Match match)
{
    // IdnMapping class with default property values.
    IdnMapping idn = new IdnMapping ();

    string domainName = match.Groups[2].Value;
    try
    {
        domainName = idn.GetAscii (domainName);
    }
    catch (ArgumentException) {
        invalid = true;
    }
    return match.Groups[1].getValue () + domainName;
}
    */

void EmailAddress::print (Expression& io)
{

}

}   //  _Id
