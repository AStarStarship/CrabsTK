/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_device_button.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_HMI_DEVICEBUTTON
#define HEADER_FOR_KT_HMI_DEVICEBUTTON

#include "config.h"

#include "../button.h"

namespace _ {

/*
class API ButtonMacro;
class API MacroEvent;
class API Device;
class API ControlsPage; */

/* A DeviceButton is a ButtonMacro that has the ability to change the Device
    on a ControlSurfaces.
    A Device button is capable of changing the Device on a ControlSurface and
    triggering a MacroEvent. */
class API DeviceButton : public ButtonMacro {
 public:
  /* Constructor. */
  DeviceButton(const char* HmiComponent = const char* ::empty,
               const MacroEvent& initMacro = MacroEvent(),
               const Device& initDevice = Device::empty,
               const ControlsPage& initPage = ControlsPage::blank);

  /* Copy constructor. */
  DeviceButton(const DeviceButton& that);

  /* Destructor. */
  ~DeviceButton();

  /* Function that is called when this Button gets pressed. */
  void Press(ButtonEvent buttonEvent);

  /* Function that is called when this Button gets double pressed. */
  void Depress(ButtonEvent buttonEvent);

  /* Function that is called when this Button gets depressed. */
  void Depress(ButtonEvent buttonEvent);

  /* Device that gets loaded when this macro gets triggered. */
  Device* GetFocusDevice();

  /* Gets a pointer to page to focus on. */
  ControlsPage* GetFocusPage();

  /* Sets this Device's focus to device. */
  int SetDevice(Device* d);

  /* Sets the focusPage to page. */
  int SetFocusPage(ControlsPage* page);

 private:
  Device* focus_device_;      //< Device to load for this macro.
  ControlsPage* focus_page_;  //< Last saved page to switch between macros.
};                            //< class DeviceButton
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_DEVICEBUTTON
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
