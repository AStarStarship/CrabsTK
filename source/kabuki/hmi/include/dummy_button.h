/** Kabuki Theater
    @file    ~/Source/hmi/Buttons/DummyButton.h
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License [here](
             http://www.apache.org/licenses/LICENSE-2.0).
             
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/
 
#pragma once

#include "../IButton.h"
#include "../ButtonEvent.h"

namespace hmi  { namespace Buttons {

class _HMI_API DummyButton : public hmi::Button
/*< A dummy Button that doesn't do anything. */
{
    public:

    DummyButton (const char* initLabel = "");
	//< Creates a DummyButton with the initLabel.

    void press (ButtonEvent buttonEvent);
	//< The action that gets performed when this button gets pressed.

    void depress (ButtonEvent buttonEvent);
	//< The action that gets performed when this button gets depressed.

    void doublePress (ButtonEvent buttonEvent);
	//< The action that gets performed when this button gets double pressed.
};

}   //< Buttons
}   //< hmi 
 
