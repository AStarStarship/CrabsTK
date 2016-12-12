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


#if DEBUG
class _KabukiSDK ControlArrayTests : public UnitTest
{
    public:

    ControlArrayTests () : UnitTest ("Controls::ControlArray class _KabukiSDK\n" + ConsoleLine ('-') + "\n") {}

    void runTest ()
    {
        auto buttonA = ControlArray ();

        beginTest ("Testing const char* print (I2P::Terminal& slot)...");
        LogMessage  (buttonA.print (I2P::Terminal& slot));

        auto testControl1 = DMXControl ("Control 1", 50, 33);
        auto testControl2 = MIDIControl ("Control 2", 11, 102);

        beginTest ("testing void setControl  (Control*)");

        buttonA.setControl  (0, &testControl1);
        buttonA.setControl  (1, &testControl2);

        LogMessage ("Added 2 controls to buttonA:\n" + buttonA.print (I2P::Terminal& slot));

        auto testControl3 = DMXControl ("Control 3", 27, 66);
        auto testControl4 = MIDIControl ("Control 4", 62, 24);

        buttonA.addControl (&testControl3);
        buttonA.addControl (&testControl4);

        LogMessage ("Attempted to add 2 more controls buttonA:\n" + buttonA.print (I2P::Terminal& slot));

        auto testControl5 = DMXButton ("Control 5", 27, 66);
        auto testControl6 = MIDIButton ("Control 6", 62, 24);
        auto testControl7 = MacroButton ("Control 7");
        testControl7.add (&testControl1);
        testControl7.add (&testControl2);
        testControl7.add (&testControl3);
        testControl7.add (&testControl4);
        testControl7.add (&testControl5);
        testControl7.add (&testControl6);

        buttonA.addControl (&testControl5);
        buttonA.addControl (&testControl6);
        buttonA.addControl (&testControl7);

        LogMessage ("Attempted to add some buttons:\n" + buttonA.print (I2P::Terminal& slot));

        LogMessage ("Controls::ControlArray tests completed successfully. :-)");
    }
};
static ControlArrayTests testUnit_ControlArray;
#endif // DEBUG