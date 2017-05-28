/** Kabuki Theater
    @file    /.../Source/_HMI/Buttons/DummyButton.hpp
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

#include "../Button.hpp"
#include "../ButtonEvent.hpp"

namespace _HMI  { namespace Buttons {

class _KabukiTheater_ DummyButton : public _HMI::Button
/*< A dummy Button that doesn't do anything. */
{
    public:

    DummyButton  (const char* initLabel = "");     //< Creates a DummyButton with the initLabel.

    void press       (ButtonEvent buttonEvent);     //< The action that gets performed when this button gets pressed.

    void depress     (ButtonEvent buttonEvent);     //< The action that gets performed when this button gets depressed.

    void doublePress (ButtonEvent buttonEvent);     //< The action that gets performed when this button gets double pressed.
};

}   //< Buttons
}   //< _HMI 
 
