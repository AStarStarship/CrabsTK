/** Kabuki Starship
    @file    ~/Source/_hmi/Buttons/DeviceButton.h
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

#include <KabukiTheater-Config.h>

#include <memory.h>

#include "../AButton.h"

namespace kabuki { namespace theater { namespace hmi {

/*
class KTH_HMI_API MacroButton;
class KTH_HMI_API MacroEvent;
class KTH_HMI_API Device;
class KTH_HMI_API ControlsPage; */

/** A DeviceButton is a MacroButton that has the ability to change the Device 
    on a ControlSurfaces.
    A Device button is capable of changing the Device on a ControlSurface and
    triggering a MacroEvent. */
class KTH_HMI_API DeviceButton : public MacroButton
{
    public:
    
    /** Constructor. */
    DeviceButton (const char* HMIComponent = const char*::empty, const MacroEvent& initMacro = MacroEvent (),
        const Device& initDevice = Device::empty, const ControlsPage& initPage = ControlsPage::blank);

    /** Copy constructor. */
    DeviceButton (const DeviceButton& that);

    /** Destructor. */
    ~DeviceButton ();

    /** The function that is called when this Button gets pressed. */
    void Press (ButtonEvent buttonEvent);

    /** The function that is called when this Button gets double pressed. */
    void Depress (ButtonEvent buttonEvent);

    /** The function that is called when this Button gets depressed. */
    void Depress (ButtonEvent buttonEvent);

    /** The Device that gets loaded when this macro gets triggered. */
    Device* GetFocusDevice ();

    /** Gets a pointer to page to focus on. */
    ControlsPage* GetFocusPage ();

    /** Sets this Device's focus to thisDevice. */
    int SetDevice (Device *d);

    /** Sets the focusPage to thisPage. */
    int SetFocusPage (ControlsPage *thisPage);

    private:
    
    Device* focus_device_;      //< Device to load for this macro.
    ControlsPage* focus_page_;  //< Last saved page to switch between macros.
};
}   //< namespace hmi
}   //< namespace theater
}   //< namespace kabuki
