/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Library/KabukiSDK-Impl/_UI/TESTS/Module.hpp
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

namespace _UI {
    
#if _DebugKabukiSDK

class _KabukiSDK MIDIButtonTests : public UnitTest
{
    public:
    MIDIButtonTests () : UnitTest ("Controls::MIDIButton...\n" + ConsoleLine ('~')) {}

    void runTest ()
    {
        auto buttonA = MIDIButton ("MIDI Button A");

        beginTest ("Testing const char* print (I2P::Terminal& slot)");
        logMessage  (buttonA.print (I2P::Terminal& slot));

        logMessage ("Done testing Controls:::MIDIButton class _KabukiSDK");
    }
};
static MIDIButtonTests testUnit_MIDIButton;

#endif  //< _DebugKabukiSDK

}   //< namespace _UI
