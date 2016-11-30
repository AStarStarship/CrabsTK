/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Library/KabukiSDK-Impl/_UI/TESTS/OSCControlTests.hpp
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

#include <_UI/OSCControl.hpp>

#if DEBUG

class OSCControlTests : public UnitTest
/** Unit test for the OSCControl class _KabukiSDK. */
{
    public:

    OSCControlTests () : UnitTest ("Controls::OSCControl") {}

    void runTest () 
    {
        LogMessage  (ConsoleLine ('-'));
        OSCControl controlA ("OSC Control A");

        LogMessage ("Done testing OSCControl class _KabukiSDK");
    }
    
    virtual const char* op (I2P::Terminal& slot, int index, int enq);
    /*< Inter-process subroutines. */
};
static OSCControlTests testUnit_DMXControl;
#endif // DEBUG