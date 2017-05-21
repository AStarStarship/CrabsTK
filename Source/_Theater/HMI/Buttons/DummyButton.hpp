/** Kabuki Theater
    @file    /.../Source/_Theater/HMI/Buttons/UnitTest.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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


#include <KabukiTheater-Config.hpp>

#include "../Button.hpp"
#include "../ButtonEvent.hpp"

namespace _Theater { namespace HMI  { namespace Buttons {

/* A dummy Button that doesn't do anything. */
class _KabukiTheater_ DummyButton : public Controls::Button
{
    public:

    DummyButton  (const char* initLabel = "");     //< Creates a DummyButton with the initLabel.

    void Press  (ButtonEvent buttonEvent);           //< The action that gets performed when this button gets pressed.
    void Depress  (ButtonEvent buttonEvent);         //< The action that gets performed when this button gets depressed.
    void Depress  (ButtonEvent buttonEvent);     //< The action that gets performed when this button gets double pressed.
};

#ifdef DEBUG
class _KabukiTheater_ Controls__Buttons__DummyButtonTests : public UnitTest
{
    public:
    Controls__Buttons__DummyButtonTests () : UnitTest ("Testing Controls::Buttons::DummyButton") {}

    void runTest ()
    {
        beginTest ("Part 1");

        expect  (false);
        expect  (true);

        beginTest ("Part 2");

        expect  (false);
        expect  (true);
    }
};
#endif // DEBUG

}   //< namespace Buttons
}   //< namespace _Theater { namespace HMI 
 
