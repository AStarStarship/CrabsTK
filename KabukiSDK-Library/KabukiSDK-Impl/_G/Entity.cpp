/** Underscore
    @file       /.../Source/Kabuki_SDK-Impl/_Id/Entity.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D.Vector_f interface.
*/

#include "_Id/Entity.h"
using namespace _Id;

Entity::Entity (const string& anEmailAdress = "", const string& aFirstName = "", const string& aLastName = "", 
    const string& aPrimaryPhoneNum = "", const string& aStreetAdress1 = "", const string& aZipCode1 = "", 
    const string& aStreetAdress2 = "", const string& aZipCode2 = "")
{
    firstName = aFirstName;
    lastName = aLastName;
    phoneNumber = aPrimaryPhoneNum;
    emailAdress = anEmailAdress;
    streetAdress1 = aStreetAdress1;
    zipCode1 = aZipCode1;
}

string Entity::GetName () { return name; }

int Entity::SetName (const string& S) { name = S; }

bool Entity::Contains (string queery)
{
    for_each (tags.begin(), tags.end (), [](string &S) 
    { 
        if (S == queery) return true; 
    });
    
    for_each (addresses.begin (), addresses.end(), [](Address& A)
    { 
        if (S == queery) return true; 
    });
    for_each (emailAddresses.begin (), emailAddresses.end(), [](EmailAddress& A) 
    {
        if (a == queery) return true;
    });
    for_each (profiles.begin (), profiles.end(), Profile& P)
    {
        if (a == queery) return true; 
    });
    for_each (tags.begin (), tags.end(), [](String& S)
    {
        if (a == queery) return true; 
    });
    
    return false;
}
