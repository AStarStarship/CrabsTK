/** Kabuki Theater
    @file    /.../Source/_Theater/HMI/Buttons/DeviceButton.hpp
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

#include <memory.hpp>

#include "../AButton.hpp"

namespace _Dev { namespace Buttons {

class _KabukiTheater_ MacroButton;
class _KabukiTheater_ MacroEvent;
class _KabukiTheater_ Device;
class _KabukiTheater_ ControlsPage;

/** A DeviceButton is a MacroButton that has the ability to change the Device on a ControlSurfaces.
    A Device button is capable of changing the Device on a ControlSurface and triggering a MacroEvent. */


class _KabukiTheater_ DeviceButton : public MacroButton
{
    public:
    
    DeviceButton  (const char* Label = const char*::empty, const MacroEvent& initMacro = MacroEvent (),
        const Device& initDevice = Device::empty, const ControlsPage& initPage = ControlsPage::blank);
    /*< Constructor. */

    DeviceButton  (const DeviceButton& that);
    /*< Copy constructor. */

    ~DeviceButton ();
    /*< Destructor. */

    void press  (ButtonEvent buttonEvent);
    /*< The function that is called when this Button gets pressed. */

    void depress  (ButtonEvent buttonEvent);
    /*< The function that is called when this Button gets double pressed. */

    void depress  (ButtonEvent buttonEvent);
    /*< The function that is called when this Button gets depressed. */

    Device *getFocusDevice ();
    /*< The Device that gets loaded when this macro gets triggered. */

    ControlsPage *getFocusPage ();
    /*< Gets a pointer to page to focus on. */

    int setDevice  (Device *d);
    /*< Sets this Device's focus to thisDevice. */

    int setFocusPage  (ControlsPage *thisPage);
    /*< Sets the focusPage to thisPage. */

    private:
    
    Device* focusDevice;        //< The Device to load for this macro.
    ControlsPage* focusPage;    //< The last saved page  (for switching between macros).
};
}   //< namespace Controls
}   //< namespace _Dev
