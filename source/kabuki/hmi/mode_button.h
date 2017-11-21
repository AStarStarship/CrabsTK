/** Kabuki Starship
    @file    ~/Source/_hmi/Buttons/ModeButton.h
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

#include "../Button.h"

namespace _hmi { namespace Buttons {

class _HMI_API MacroButton;
class _HMI_API MacroEvent;
class _HMI_API Device;
class _HMI_API ControlsPage;

class _HMI_API ModeButton : public Button
/** A ModeButton is a MacroButton that has the ability to change the Device on a ControlSurfaces.
    A Device button is capable of changing the Device on a ControlSurface and triggering a MacroEvent. 
*/
{
    public:

    ModeButton  (const char* initLabel = "", const MacroEvent& initMacro = MacroEvent (),
        const Device& initDevice = Device::Dummy, const ControlsPage& initPage = ControlsPage::blank);
    /*< Constructor. */
        
    ModeButton  (const ModeButton& that);
    //< Copy constructor.
    ~ModeButton ();
    

    void Press  (ButtonEvent buttonEvent);
    //< The function that is called when this Button gets pressed.
    
    void Depress  (ButtonEvent buttonEvent);
    
    //< The function that is called when this Button gets double pressed.
    
    void Depress  (ButtonEvent buttonEvent);
    //< The function that is called when this Button gets depressed.

    Device *focusDevice ();
    //< The Device that gets loaded when this macro gets triggered.
    
    ControlsPage *focusPage ();
    //< Gets a pointer to page to focus on.

    int setDevice  (Device *thisDevice);
    //< Sets this Device's focus to thisDevice.
    
    int setFocusPage  (ControlsPage *thisPage);
    //< Sets the focusPage to thisPage.
    
    void print ();
    /*< Prints this object to a terminal. */

    private:

    Device *_focusDevice;                                   //< The Device to load for this macro.
    ControlsPage *_focusPage;                               //< The last saved page  (for switching between macros).
};

}   //< namespace Buttons
}   //< namespace _hmi

