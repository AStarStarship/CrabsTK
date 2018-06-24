/* Kabuki Toolkit
@file    ~/source/hmi/hmi_/ButtonMacro.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#pragma once
#include <stdafx.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_KT_HMI_BUTTONMACRO
#define HEADER_FOR_KT_HMI_BUTTONMACRO

#include <vector>
#include "button.h"
#include "button_midi.h"
#include "control_midi.h"
#include "dmx_button.h"
#include "dmx_control.h"
#include "parameter.h"

namespace _ {

/* Parameter<int> and Button that can trigger multiple events.
    Controls are stored as pointers in a linear linked list to optimize for
   speed.
*/
class KABUKI ButtonMacro : public Parameter<int>, public Button {
 public:
  /* Default constructor creates a ButtonMacro with no controls.
      A ButtonMacro without any controls triggers system functions only.*/
  ButtonMacro(const string& name = "");

  /* Copy constructor. */
  ButtonMacro(const ButtonMacro& other);

  /* Destructor. */
  ~ButtonMacro();

  /* Gets the number of controls. */
  int GetNumControls() const;

  /* Clears all of the controls from the list. */
  void ClearControls();

  /* Adds an Event to the event list.
      @return gets Success upon success.
      @return gets -1 if the newEvent is null. */
  int Add(Parameter<int>* newEvent);

  /* Adds an array of newEvents to the list. */
  int Add(_::Array<Parameter<int>*>& newEvents);

  /* sets the Event at the given index to the newEvent. */
  int Set(int index, Parameter<int>* newEvent);

  /* Removes the first removes in the list. */
  Parameter<int>* Remove();

  /* Removes the oldEvent from the list.
      @return gets nullptr if the index was out of
          bounds. */
  Parameter<int>* Remove(int index);

  /* Removes the oldEvent from the list.
      @return gets nullptr if the oldEvent was null or if
          it not in the list. */
  Parameter<int>* Remove(Parameter<int>* oldEvent);

  /* Gets the event at the given index.
      @return gets nullptr if the index is invalid. */
  Parameter<int>* GetControl(int index);

  /* Gets the first event in the list.
      @return gets nullptr if list is empty. */
  Parameter<int>* GetFirstControl();

  /* Gets the max value of a Parameter<int> word. */
  int GetMaxWordValue() const override;

  /* Gets the label. */
  string& GetLabel() const override;

  /* Sets the label to the new label.
      @return gets 0 upon success and 1 if the newLabel is
      too uint32_t.
      @see    HmiComponent::isValid (char). */
  int SetLabel(const string& newLabel) override;

  /* Toggles the state of the Button if type is latching. */
  void Toggle() override;

  /* Triggers all of this macros functions. */
  void Trigger() override;

  /* Triggered when a mode button is pressed. */
  void Press() override {}

  /* Triggered when a mode button is depressed. */
  void Depress() override {}

  /* Triggered when a user "double clicks" a button. */
  void Depress() override {}

  /* Prints a string of the row strings of the list. */
  void PrintListString() const;

  /* Prints the header for toStringRow (). */
  void PrintHeaderString() const override;

  /* Prints a column of the values without the labels. */
  void PrintRow() const override;

  /* Prints this object to a terminal. */
  _::Printer& Print(_::Printer& print) const;

 private:
  _::Array<Parameter<int>*> controls;  //< A vector of Parameter<int> points.
};

}       //< namespace _
#endif  //< #ifndef HEADER_FOR_KT_HMI_BUTTONMACRO
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
