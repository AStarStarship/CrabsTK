// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#ifndef CRABSTK_TOUCH_BUTTONMACRO_DECL_DECL
#define CRABSTK_TOUCH_BUTTONMACRO_DECL_DECL

#include "Button.hpp"
#include "ButtonMidi.hpp"
#include "ControlMidi.hpp"
#include "ButtonDMX.hpp"
#include "ControlDMX.hpp"
#include "Param.hpp"
namespace _ {

/* Parameter<ISC> and Button that can trigger multiple events.
    Controls are stored as pointers in a linear linked list to optimize for
   speed.
*/
class LIB_MEMBER ButtonMacro : public Parameter<ISC>, public Button {
 public:
  /* Default constructor creates a ButtonMacro with no controls.
      A ButtonMacro without any controls triggers system functions only.*/
  ButtonMacro(const TString<>& name = "")
    : Parameter<ISC> (Parameter<ISC>::ButtonMacro, label, 0, 0, 0, 0, 0),
    control_count_ (0),
    controls (nullptr) {
    
  }

  /* Copy constructor. */
  ButtonMacro(const ButtonMacro& other);

  /* Destructor. */
  ~ButtonMacro();

  /* Gets the number of controls. */
  ISC ControlCount() const { return control_count_; }

  /* Clears all of the controls from the list. */
  void ClearControls() { ClearList (controls_); }

  /* Adds an Event to the event list.
  @return gets Success upon success.
  @return gets -1 if the event is null. */
  ISC Add(Parameter<ISC>* e) {
    if (control == nullptr) return -1;
    controls = new LinearNode<Parameter<ISC>*> (control, controls);
    ++control_count_;
    return Success;
  }

  /* Adds an array of newEvents to the list. */
  ISC Add(_::Array<Parameter<ISC>*>& events) {
    ISC numFails = 0;

    for (ISC i = 0; i < events.size (); ++i) {
      if (add (events[i])) {
        ++numFails;
      }
    }
    return numFails;
  }

  /* sets the Event at the given index to the event. */
  ISC Set(ISC index, Parameter<ISC>* event) {
    if (index < 0) return -1;
    if (index >= control_count_) return 1;

    if (control_count_ == 0) {
      controls = new LinearNode<Parameter<ISC>*> (control);
      return Success;
    }
    auto currentNode = controls;
    while (index-- != 0) currentNode = currentNode->next;
    currentNode->data = control;
    return Success;
  }

  /* Removes the first removes in the list. */
  Parameter<ISC>* Remove() {
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
  Parameter<ISC>* Remove(ISC index) {
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
  Parameter<ISC>* Remove(Parameter<ISC>* param) {
    auto c = controls;

    while (c != nullptr) {
      if (c->next->data == param) return RemoveAfter (c);
      c = c->next;
    }
    return nullptr;
  }

  /* Gets the event at the given index.
  @return gets nullptr if the index is invalid. */
  Parameter<ISC>* GetControl(ISC index) {
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
  Parameter<ISC>* GetFirstControl() {
    if (controls == nullptr) return nullptr;
    return controls->data;
  }

  /* Gets the max value of a Parameter<ISC> word. */
  ISC GetMaxWordValue() const override;

  /* Gets the label. */
  TString<>& Label() const override { return label; }

  /* Sets the label to the new label.
  @return Nil upon success or an error AString upon failure. */
  const CHA* SetLabel(const TString<>& new_label) override {
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

  /* Triggered when a user "FPD clicks" a button. */
  void Depress() override {}

  /* Prints a AString of the row Strings of the list. */
  template<typename Printer>
  Printer& PrintListString(Printer& o) const {
    o << LineString ('#', Parameter<ISC>::kMacroHeaderLength);

    auto node = controls;
    while (node) {
      o << node->data->PrintStringRow (o) << kLF;
      node = node->next;
    }
    return o << LineString ('#', Parameter<ISC>::kMacroHeaderLength);
  }

  /* Prints the header for toStringRow (). */
  template<typename Printer>
  Printer& PrintHeaderString(Printer& o) const override {
    return o << "\n|    Parameter<ISC>    |Action|Step | CC  | Ch  |Value"
                "| min | max |Init |# Bits|";
  }

  /* Prints a column of the values without the labels. */
  template<typename Printer>
  Printer& PrintRow(Printer& o) const override {
    Parameter<ISC>::PrintRow (o);
    o << "   | " << Right (ActionString (), 10) << " | " 
      << Right (ListString, 5) << " |";
    return PrintListString (o);
  }

  /* Prints this object to a terminal. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o << LineString ('~', Parameter<ISC>::kMacroHeaderLength)
      << "Macro Control:\n "
      << LineString ('~', Parameter<ISC>::kMacroHeaderLength)
      << Parameter<ISC>::MacroHeader << kLF
      << LineString ('~', Parameter<ISC>::kMacroHeaderLength)
      << PrintStringRow (o);
    return o << PrintListString (o);
  }

 private:
  TArray<Parameter<ISC>*> controls;  //< A TArray of Parameter<ISC> points.

  Parameter<ISC>* RemoveAfter (
    LinearNode<Parameter<ISC>*>* node) {
    if (node == nullptr) return nullptr;
    auto nodeToDelete = node->next;
    if (nodeToDelete == nullptr) return nullptr;
    // else we are safe to delete the node.
    Parameter<ISC>* removedNodesControl = nodeToDelete->data;
    node->next = nodeToDelete->next;
    delete nodeToDelete;
    return removedNodesControl;
  }

  void ClearList (LinearNode<Parameter<ISC>*>* node) {
    if (!node) return;
    ClearList (node->next);
    delete node;
  }
};

}  //< namespace _
#endif
#endif
