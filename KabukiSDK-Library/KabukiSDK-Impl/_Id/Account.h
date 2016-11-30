/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Id/Account.hpp
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

#include <vector>
using namespace std;

namespace _Id
{
/** An account with a username and password. */
class Account
{
	public:
	
    /** Default constructor. **/
    Account (const string& AUsername = "New user");

    Account (const string& AUsername, const string& APassword);
    
    bool IsValid ();
    
    string GetName ();
    
    bool SetName (const string& S);

    bool RequiresPassword ();
    
    void SetPassword (const string& S);

    //
    UserList Users ();

    //
    void AllowUser (User& U);

    //bool RequestNewAccount (Network.Address request_source, string User_Name, string Password,
    //    string First_Name, string Last_Name, string Adress1, string Adress2, string Zip_Code);

    //
    bool Login (User U);


    //
    string print (I2P::Terminal& slot);

    private:
    
    Username username;
    Password password;

    vector<User> users;
};
}
