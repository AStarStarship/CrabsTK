/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_ButtonMacro.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_BUTTONMACRO
#define KABUKI_TOOLKIT_AV_BUTTONMACRO

#include "t_button.h"
#include "t_button_midi.h"
#include "t_control_midi.h"
#include "t_dmx_button.h"
#include "t_dmx_control.h"
#include "t_param.h"

namespace _ {

/* Parameter<SI4> and Button that can trigger multiple events.
    Controls are stored as pointers in a linear linked list to optimize for
   speed.
*/
class SDK ButtonMacro : public Parameter<SI4>, public Button {
 public:
  /* Default constructor creates a ButtonMacro with no controls.
      A ButtonMacro without any controls triggers system functions only.*/
  ButtonMacro(const TStrand<>& name = "")
    : Parameter<SI4> (Parameter<SI4>::ButtonMacro, label, 0, 0, 0, 0, 0),
    control_count_ (0),
    controls (nullptr) {
    // Nothing to do here! :-)
  }

  /* Copy constructor. */
  ButtonMacro(const ButtonMacro& other);

  /* Destructor. */
  ~ButtonMacro();

  /* Gets the number of controls. */
  SI4 ControlCount() const { return control_count_; }

  /* Clears all of the controls from the list. */
  void ClearControls() { ClearList (controls_); }

  /* Adds an Event to the event list.
  @return gets Success upon success.
  @return gets -1 if the event is null. */
  SI4 Add(Parameter<SI4>* e) {
    if (control == nullptr) return -1;
    controls = new LinearNode<Parameter<SI4>*> (control, controls);
    ++control_count_;
    return Success;
  }

  /* Adds an array of newEvents to the list. */
  SI4 Add(_::Array<Parameter<SI4>*>& events) {
    SI4 numFails = 0;

    for (SI4 i = 0; i < events.size (); ++i) {
      if (add (events[i])) {
        ++numFails;
      }
    }
    return numFails;
  }

  /* sets the Event at the given index to the event. */
  SI4 Set(SI4 index, Parameter<SI4>* event) {
    if (index < 0) return -1;
    if (index >= control_count_) return 1;

    if (control_count_ == 0) {
      controls = new LinearNode<Parameter<SI4>*> (control);
      return Success;
    }
    auto currentNode = controls;
    while (index-- != 0) currentNode = currentNode->next;
    currentNode->data = control;
    return Success;
  }

  /* Removes the first removes in the list. */
  Parameter<SI4>* Remove() {
    if (controls == nullptr) return nullptr;
    --control_count_;
    auto temp = controls->next;
    delete controls;
    controls = temp;
    return temp->data;
  }

  /* Removes the oldEvent from the list.
  @return gets nullptr if the index was out of
  bounds. */
  Parameter<SI4>* Remove(SI4 index) {
    if (index < 0 || index >= control_count_ || control_count_ == 0) 
      return nullptr;

    if (control_count_ == 1) {
      delete controls;
      control_count_ = 1;
      return nullptr;
    }

    auto currentNode = controls;
    while (index-- != 1) currentNode = currentNode->next;

    return removeAfter (currentNode);
  }

  /* Removes the oldEvent from the list.
      @return gets nullptr if the oldEvent was null or if
          it not in the list. */
  Parameter<SI4>* Remove(Parameter<SI4>* param) {
    auto c = controls;

    while (c != nullptr) {
      if (c->next->data == param) return RemoveAfter (c);
      c = c->next;
    }
    return nullptr;
  }

  /* Gets the event at the given index.
  @return gets nullptr if the index is invalid. */
  Parameter<SI4>* GetControl(SI4 index) {
    if (control_count_ == 0) return nullptr;

    if (index < 0 || index >= control_count_) return nullptr;

    auto currentNode = controls;
    while (index != 0) {
      currentNode = currentNode->next;
      --index;
    }
    return currentNode->data;
  }

  /* Gets the first event in the list.
  @return gets nullptr if list is empty. */
  Parameter<SI4>* GetFirstControl() {
    if (controls == nullptr) return nullptr;
    return controls->data;
  }

  /* Gets the max value of a Parameter<SI4> word. */
  SI4 GetMaxWordValue() const override;

  /* Gets the label. */
  TStrand<>& Label() const override { return label; }

  /* Sets the label to the new label.
  @return Nil upon success or an error string upon failure. */
  const CH1* SetLabel(const TStrand<>& new_label) override {
    return label_.Set (new_label);
  }

  /* Toggles the state of the Button if type is latching. */
  void Toggle() override;

  /* Triggers all of this macros functions. */
  void Trigger() override {
    auto currentNode = controls;
    while (currentNode != nullptr) {
      currentNode->data->trigger ();
      currentNode = currentNode->next;
    }
  }

  /* Triggered when a mode button is pressed. */
  void Press() override {}

  /* Triggered when a mode button is depressed. */
  void Depress() override {}

  /* Triggered when a user "DBL clicks" a button. */
  void Depress() override {}

  /* Prints a string of the row strings of the list. */
  template<typename Printer>
  Printer& PrintListString(Printer& o) const {
    o << LineStrand ('#', Parameter<SI4>::kMacroHeaderLength);

    auto node = controls;
    while (node) {
      o << node->data->PrintStringRow (o) << '\n';
      node = node->next;
    }
    return o << LineStrand ('#', Parameter<SI4>::kMacroHeaderLength);
  }

  /* Prints the header for toStringRow (). */
  template<typename Printer>
  Printer& PrintHeaderString(Printer& o) const override {
    return o << "\n|    Parameter<SI4>    |Action|Step | CC  | Ch  |Value"
                "| min | max |Init |# Bits|";
  }

  /* Prints a column of the values without the labels. */
  template<typename Printer>
  Printer& PrintRow(Printer& o) const override {
    Parameter<SI4>::PrintRow (o);
    o << "   | " << Right (ActionString (), 10) << " | " 
      << Right (ListString, 5) << " |";
    return PrintListString (o);
  }

  /* Prints this object to a terminal. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o << LineStrand ('~', Parameter<SI4>::kMacroHeaderLength)
      << "Macro Control:\n "
      << LineStrand ('~', Parameter<SI4>::kMacroHeaderLength)
      << Parameter<SI4>::MacroHeader << '\n'
      << LineStrand ('~', Parameter<SI4>::kMacroHeaderLength)
      << PrintStringRow (o);
    return o << PrintListString (o);
  }

 private:
  TArray<Parameter<SI4>*> controls;  //< A TArray of Parameter<SI4> points.

  Parameter<SI4>* RemoveAfter (
    LinearNode<Parameter<SI4>*>* node) {
    if (node == nullptr) return nullptr;
    auto nodeToDelete = node->next;
    if (nodeToDelete == nullptr) return nullptr;
    // else we are safe to delete the node.
    Parameter<SI4>* removedNodesControl = nodeToDelete->data;
    node->next = nodeToDelete->next;
    delete nodeToDelete;
    return removedNodesControl;
  }

  void ClearList (LinearNode<Parameter<SI4>*>* node) {
    if (!node) return;
    ClearList (node->next);
    delete node;
  }
};

}  // namespace _
#endif  //< #ifndef KABUKI_TOOLKIT_AV_BUTTONMACRO
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
