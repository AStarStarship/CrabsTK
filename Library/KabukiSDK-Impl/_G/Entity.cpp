/** Underscore
    @file       /.../Source/KabukiSDK-Impl/_Id/Entity.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include "_Id/Entity.hpp"
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

string& Entity::getName () { return name; }

int Entity::setName (const string& S) { name = S; }

bool Entity::Contains (string query)
{
    for_each (tags.begin(), tags.end (), [](string &S) 
    { 
        if (S == query) return true; 
    });
    
    for_each (addresses.begin (), addresses.end(), [](Address& A)
    { 
        if (S == query) return true; 
    });
    for_each (emailAddresses.begin (), emailAddresses.end(), [](EmailAddress& A) 
    {
        if (a == query) return true;
    });
    for_each (profiles.begin (), profiles.end(), Profile& P)
    {
        if (a == query) return true; 
    });
    for_each (tags.begin (), tags.end(), [](String& S)
    {
        if (a == query) return true; 
    });
    
    return false;
}
