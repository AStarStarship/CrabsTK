/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Library/KabukiSDK-Impl/_HMI/TESTS/Module.hpp
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

#pragma once

namespace _HMI {

#if _DebugKabukiSDK
class _KabukiSDK LabelUnitTest : public UnitTest
{
    public:
    LabelUnitTest () : UnitTest ("Controls::Label Class.\n")
    {
    }

    void runTest ()
    {
        const char* invalidString = "Invalid label ~",
            validString1 = "Valid label 1",
            validString2 = "Valid label !@%^",
            validString3 = "Valid label &* ()";

        auto validLabel1 = Label  (validString1),
            validLabel2  = Label  (validString1),
            validLabel3  = Label  (validString3),
            invalidLabel = Label  (invalidString);

        beginTest ("Testing valid string input.");

        beginTest ("Testing int isValid  (const char* &)");
        LogMessage  (validString1 + ": " + const char*  (Label::IsValid  (validString1)));
        expect  (Label::IsValid  (validString1) == Valid);
        beginTest ("Testing invalid string input.");
        expect  (Label::IsValid  (invalidString) != Valid);
        expect  (Label::IsValid ("") != 0);
        expect  (Label::IsValid ("123456789012345678901") != 0);

        beginTest ("Testing int compare  (const char* &)\n" + invalidLabel.print (I2P::Terminal& slot));
        LogMessage ("Testing valid comparisons...");
        expect (!validLabel1.Compare  (validString1));
        LogMessage ("Testing invalid comparisons...");
        expect  (validLabel1.Compare  (validString2) != Comperable);
        expect  (validLabel2.Compare  (validString3) != Comperable);

        beginTest ("Attempting to setLabel to the invalidString");
        //auto expectedLabel = "Test string 1";
        validLabel1.SetLabel  (invalidString);
        expect (!validLabel1.Compare  (invalidLabel));
        beginTest ("Setting the label to validString3");
        validLabel1.SetLabel  (validString3);
        expect  (validLabel1.GetLabel () == validString3);

        LogMessage ("Done testing Controls::Label class _KabukiSDK.");
    }
};
static LabelUnitTest testUnit_Label;
#endif  //< _DebugKabukiSDK
    
}   //< namespace _HMI
